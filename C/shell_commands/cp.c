//by Denis Kozub
//two arguments only
//first one has to be an existing file
//second one has to be a file
//if second one exists, it will be rewritten
//if it does not exist, it will be created with the same rights as the first one

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

enum {
    ERR_MISS_OP = 1,
    ERR_MISS_DEST = 2,
    ERR_MANY_ARGS = 3,
    ERR_NON_EXIST = 4,
    ERR_WRITE = 5, 
    ERR_STAT = 6,
    ERR_OPEN = 7
};

enum {BUF_SIZE = 4096};

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        const char *miss_op = "cp: missing file operand\n";
        if (write(2, miss_op, strlen(miss_op)) != strlen(miss_op)) {
            return ERR_WRITE;
        }
        return ERR_MISS_OP;
    }
    if (argc == 2) {
        const char *miss_dest = "cp: missing destination file operand\n";
        if (write(2, miss_dest, strlen(miss_dest)) != strlen(miss_dest)) {
            return ERR_WRITE;
        }
        return ERR_MISS_DEST;
    }
    if (argc > 3) {
        const char *many_args = "cp: too many arguments\n";
        if (write(2, many_args, strlen(many_args)) != strlen(many_args)) {
            return ERR_WRITE;
        }
        return ERR_MANY_ARGS;
    }
    int fd_from = open(argv[1], O_RDONLY);
    if (fd_from == -1) {
        char *non_exist = malloc(sizeof(*non_exist) * (strlen(argv[1]) + 34));
        sprintf(non_exist, "cp: cannot stat '%s': No such file\n", argv[1]);
        if (write(2, non_exist, strlen(non_exist)) != strlen(non_exist)) {
            free(non_exist);
            return ERR_WRITE;
        }
        free(non_exist);
        return ERR_NON_EXIST;
    }
    struct stat rights;
    if (stat(argv[1], &rights) == -1) {
        close(fd_from);
        return ERR_STAT;
    }
    int fd_to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, rights.st_mode);
    if (fd_to == -1) {
        close(fd_from);
        return ERR_OPEN;
    }
    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(fd_from, buf, BUF_SIZE)) > 0) {
        if (write (fd_to, buf, n) != n) {
            close(fd_from);
            close(fd_to);
            return ERR_WRITE;
        }
    }

    close(fd_from);
    close(fd_to);
    return 0;
}
