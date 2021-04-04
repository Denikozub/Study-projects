//by Denis Kozub

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "recursive_descent.h"

int fd[2];

void int_hndlr(int s)
{
    close(fd[0]);
    close(fd[1]);
    while (wait(NULL) > 0);
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT);
}

int
main(void)
{
    signal(SIGINT, int_hndlr);
    int res, no_wait;
    pid_t pid;
    if (pipe(fd) == -1) {
        perror("pipe");
        return errno;
    }
    while (1) {

        if ((pid = fork()) == -1)
            kill(getpid(), SIGINT);

        //new command line
        if (pid == 0) {
            close(fd[0]);
            res = descent_rpn(fd[1]);
            close(fd[1]);
            exit(res);
        }

        read(fd[0], &no_wait, sizeof(no_wait));

        //if EOF found
        if (no_wait == 2)
            kill(getpid(), SIGINT);

        //waiting for process if there is no '&'
        if (no_wait == 0)
            waitpid(pid, NULL, 0);

        //waiting for processes which were not waited for after '&'
        while(waitpid(-1, NULL, WNOHANG) > 1);
        
        fflush(stdin);
    }
    return 0;
}
