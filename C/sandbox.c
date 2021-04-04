#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/syscall.h>
#include <string.h>

#include <string.h>

/**
Requires valid from &path[0] to &path[size - 1]
Returns true iff `path' means a relative non-empty path
*/
int is_relative(const char *path, size_t size)
{
    if (size == 0 || path[0] == '/') {
        return 0;
    }

    int depth = 0;
    const char *p1 = path;
    /*loop invariant:
        p1 points to the character which previous character is /
                            or above &path[size]
        depth == depth of the <&path[0]..p1>*/
    while (p1 - path < size) {
        // p2 points to the nearest / or equals to NULL
        const char *p2 = memchr(p1, '/', path - p1 + size);
        if (p2 == 0) {
            p2 = path + size;
        }
        if (p2 == p1) {
            // don't change depth
        } else if (p2 == p1 + 1 && p1[0] == '.') {
            // don't change depth
        } else if (p2 == p1 + 2 && p1[0] == '.' && p1[1] == '.') {
            -- depth;
        } else {
            ++ depth;
        }
        if (depth < 0) {
            return 0;
        }
        p1 = p2 + 1;
    }

    return 1;
}

#if __DEBUG__
#include <assert.h>
int main(void)
{
    static const char *positives[] = { ".", ".a", "./a", "./.", "././",
    ".//", "a/.", "a/./", "a//", "a//.", "a/./b/c", "a/.//./c",
    "./a/..", "./a/../", "a/..", "a/../", "a/b/..", "a/b/../c",
    "a//../b", "a/b/../../", "a/b/.././../", "a/b/.././..", "abc", "./ab" };
    static const char *negatives[] = { "", "/", "/a", "..", "../",
    "../.", "./..", "./../", "a/../..", "a//..//../" };

    for (size_t i = 0; i < sizeof positives / sizeof positives[0]; ++i) {
        assert(is_relative(positives[i], strlen(positives[i])));
    }

    for (size_t i = 0; i < sizeof negatives / sizeof negatives[0]; ++i) {
        assert(!is_relative(negatives[i], strlen(negatives[i])));
    }
}
#endif

#if __FUZZER__
int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    // initial corpus contains tests from main()
    is_relative(Data, Size);
    return 0;  // Non-zero return values are reserved for future use.
}
#endif


//program for 64x architechture

void child(int argc, char **argv) {
    mkdir(argv[1], 0777);
    chdir(argv[1]);
    close(0);
    close(1);
    close(2);
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    execv(argv[1], &(argv[1]));
    perror("execv");
}

void parent(pid_t pid){
    //should check
    //~ int a = SYS_creat;
    //~ int b = SYS_mkdir;
    //~ int c = SYS_open;
    //~ int d = SYS_unlink;
    //~ int e = SYS_rmdir;

    //ban
    //~ int f = SYS_symlink;
    //~ int g = SYS_chdir;
    //~ int h = SYS_getcwd;
    int status;
    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    while (!WIFEXITED(status)) {
        struct user_regs_struct state;
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        pid_t elif;
        elif = wait(&status);
        //Не копить зомби
        //elif can be "-1"
        if (elif == -1){
            return;
        }
        //~ if (WIFSIGNALED(status) || WIFEXITED(status)){
            
        //~ }
        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, pid, 0, &state);
            if ((state.orig_rax == SYS_execve) ||(state.orig_rax == SYS_symlink) ||
            (state.orig_rax == SYS_getcwd)||(state.orig_rax == SYS_chdir)){
                //execve
                printf("banned operator here!\n");
                state.orig_rax = SYS_kill;
                state.rdi = pid;
                state.rsi = SIGKILL;
                ptrace(PTRACE_SETREGS, pid, 0, &state);
                ptrace(PTRACE_CONT, pid, 0, 0);
                waitpid(pid, &status, 0);
                continue;
            }
            if (state.orig_rax == SYS_openat) {
                union my_union{
                    unsigned short num;
                    char chars[2];
                } un;
                ptrace(PTRACE_GETREGS, pid, 0, &state);
                size_t cur = state.rsi;
                char buf[1024];
                size_t size = 0;
                while(1){
                    un.num = ptrace(PTRACE_PEEKDATA, pid, cur, 0);
                    if(un.chars[0] == '\0'){
                        buf[size++] = '\0';
                        break;
                    }
                    buf[size++] = un.chars[0];
                    buf[size++] = un.chars[1];
                    cur += 2;
                }
                const char *exception1 = "/etc/ld.so.cache\0";
                const char *exception2 = "/lib/x86_64-linux-gnu/libc.so.6\0";
                if ((strcmp(buf, exception1) != 0) && (strcmp(buf, exception2) != 0)){
                    printf("process %d tried to open a file %s\n", pid, buf);
                    if(!is_relative(buf, size)){
                        // kill process!
                        printf("KILL!\n");
                        state.orig_rax = SYS_kill;
                        state.rdi = pid;
                        state.rsi = SIGKILL;
                        ptrace(PTRACE_SETREGS, pid, 0, &state);
                        ptrace(PTRACE_CONT, pid, 0, 0);
                        waitpid(pid, NULL, 0);
                        continue;
                    }
                }
            }
            if ((state.orig_rax == SYS_rmdir) ||(state.orig_rax == SYS_creat) ||
            (state.orig_rax == SYS_unlink) ||(state.orig_rax == SYS_mkdir))  {
                //union for reading word from
                union my_union{
                    unsigned short num;
                    char chars[2];
                } un;
                ptrace(PTRACE_GETREGS, pid, 0, &state);
                char buf[1024];
                size_t cur = state.rsi;
                size_t size = 0;
                while(1){
                    un.num = ptrace(PTRACE_PEEKDATA, pid, cur, 0);
                    if(un.chars[0] == '\0'){
                        buf[size++] = '\0';
                        break;
                    }
                    buf[size++] = un.chars[0];
                    buf[size++] = un.chars[1];
                    cur += 2;
                }
                printf("process %d tried to do smth in other dir %s\n", pid, buf);
                if(!is_relative(buf, size)){
                    // kill process!
                    printf("KILL!\n");
                    state.orig_rax = SYS_kill;
                    state.rdi = pid;
                    state.rsi = SIGKILL;
                    ptrace(PTRACE_SETREGS, pid, 0, &state);
                    ptrace(PTRACE_CONT, pid, 0, 0);
                    waitpid(pid, NULL, 0);
                    continue;
                }
            }
            // skip after syscall
            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            waitpid(pid, &status, 0);
            if (WIFSIGNALED(status) || WIFEXITED(status)){
                continue;
            }
            if (state.orig_rax == SYS_clone){
                ptrace(PTRACE_GETREGS, pid, 0, &state);
                //fork or clone
                pid_t res = state.rax;
                printf("this is fork %d\n", res);
                if ( fork()== 0){
                    ptrace(PTRACE_ATTACH, res, 0, 0);
                    waitpid(res, &status, 0);
                    ptrace(PTRACE_SETOPTIONS, res, 0, PTRACE_O_TRACESYSGOOD);
                    pid = res;
                }
            }
        }
    }
}

int 
main(int argc, char **argv)
{
    //~ printf("fork = %d\n", SYS_fork);
    //~ printf("execve = %d\n", SYS_execve);
    pid_t pid = fork();
    if (pid)
        parent(pid);
    else
        child(argc,argv);
    return 0;
}

