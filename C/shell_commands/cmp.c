//by Denis Kozub
//two arguments only
//both have to be existing files

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

enum {
    ERR_MISS_1 = 1,
    ERR_MISS_2 = 2,
    ERR_MANY_ARGS = 3,
    ERR_NON_EXIST = 4,
    ERR_WRITE = 5
};

enum {BUF_SIZE = 4096};

typedef unsigned long ul;

ssize_t
min(ssize_t n1, ssize_t n2)
{
    return n1 < n2 ? n1 : n2;
}

int
open_file(int *fd, char *name)
{
    *fd = open(name, O_RDONLY);
    if (*fd == -1) {
        char *non_exist = malloc(sizeof(*non_exist) * (strlen(name) + 23));
        sprintf(non_exist, "cmp: '%s': No such file\n", name);
        if (write(2, non_exist, strlen(non_exist)) != strlen(non_exist)) {
            free(non_exist);
            return ERR_WRITE;
        }
        free(non_exist);
        return ERR_NON_EXIST;
    }
    return 0;
}

int
num_symbols(ul num)
{
    int count = 0;
    do {
        num = num / 10;
        ++count;
    } while (num != 0);
    return count;
}

//print where files are different
int
diff(ul byte_num, ul line_num, int *fd1, int *fd2, char *s1, char *s2)
{
    char *differ = malloc(sizeof(*differ) * (strlen(s1) + strlen(s2) +
            num_symbols(byte_num) + num_symbols(line_num) + 24));
    sprintf(differ, "%s %s differ: byte %lu, line %lu\n",
            s1, s2, byte_num, line_num);
    if (write(2, differ, strlen(differ)) != strlen(differ)) {
        free(differ);
        close(*fd1);
        close(*fd2);
        return ERR_WRITE;
    }
    free(differ);
    close(*fd1);
    close(*fd2);
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        const char *miss_1 = "cmp: missing operand after 'cmp'\n";
        if (write(2, miss_1, strlen(miss_1)) != strlen(miss_1)) {
            return ERR_WRITE;
        }
        return ERR_MISS_1;
    }
    if (argc == 2) {
        const char *miss_2 = "cmp: missing second operand\n";
        if (write(2, miss_2, strlen(miss_2)) != strlen(miss_2)) {
            return ERR_WRITE;
        }
        return ERR_MISS_2;
    }
    if (argc > 3) {
        const char *many_args = "cmp: too many arguments\n";
        if (write(2, many_args, strlen(many_args)) != strlen(many_args)) {
            return ERR_WRITE;
        }
        return ERR_MANY_ARGS;
    }
    int fd1, fd2, res;
    if ((res = open_file(&fd1, argv[1])) != 0) {
        return res;
    }
    if ((res = open_file(&fd2, argv[2])) != 0) {
        close(fd1);
        return res;
    }

    char buf1[BUF_SIZE], buf2[BUF_SIZE];
    ul byte_num = 1, line_num = 1;
    ssize_t n1, n2;
    while ((n1 = read(fd1, buf1, BUF_SIZE)) > 0 &&
            (n2 = read(fd2, buf2, BUF_SIZE)) > 0) {
        for (int i = 0; i < min(n1, n2); i++) {
            if ((char)buf1[i] != (char)buf2[i]) {
                return diff(byte_num, line_num, &fd1, &fd2, argv[1], argv[2]);
            }
            ++byte_num;
            if ((char)buf1[i] == '\n')
                ++line_num;
        }
        if (n1 != n2) {
            return diff(byte_num + 1, line_num, &fd1, &fd2, argv[1], argv[2]);
        }
    }
    if ((n1 == 0 && (n2 = read(fd2, buf2, BUF_SIZE)) > 0) ||
            (n2 == 0 && n1 > 0)) {
        return diff(byte_num, line_num, &fd1, &fd2, argv[1], argv[2]);
    }

    close(fd1);
    close(fd2);
    return 0;
}
