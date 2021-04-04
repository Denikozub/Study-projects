//by Denis Kozub
//one or three arguments possible
//if only one then it has to be an existing file
//if three then first one has to be -n flag
//...second one: +NUM for printing all starting from NUM line
//...and NUM or -NUM for printing last NUM lines
//...Third one: existing file

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>

enum {
    ERR_MISS_OP = 1,
    ERR_WRONG_ARG = 2,
    ERR_MANY_ARGS = 3,
    ERR_NON_EXIST = 4,
    ERR_DIAPASON = 5,
    ERR_WRONG_NUM = 6,
    ERR_WRITE = 7,
    ERR_LSEEK = 8
};

enum {BUF_SIZE = 4096};

typedef unsigned long ul;

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
print_tail(int from_end, ul num, int *fd)
{
    char buf[BUF_SIZE];
    ssize_t n;
    if (from_end == 1) { //if +NUM
        ul lines = 1; //find num - number of last lines
        while ((n = read(*fd, buf, BUF_SIZE)) > 0) {
            for (int i = 0; i < n; i++) {
                if ((char)buf[i] == '\n')
                    lines++;
            }
        }
        num = lines - num;
        if (lseek(*fd, 0, SEEK_SET) == -1) {
            close(*fd);
            return ERR_LSEEK;
        }
    }
    ul count = 1;
    int can_write = 0;
    while ((n = read(*fd, buf, BUF_SIZE)) > 0) {
        if (can_write == 0 && num > 1) for (int i = 0; i < n; i++) {
            if ((char)buf[i] == '\n')
                count++;
            if (count == num) {
                can_write = 1;
                if (write(1, buf + i + 1, n - i - 1) != n - i - 1) {
                    close(*fd);
                    return ERR_WRITE;
                }
                break;
            }
        } else if (write(1, buf, n) != n) {
            close(*fd);
            return ERR_WRITE;
        }
    }
    close(*fd);
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        const char *miss_op = "tail: missing file operand\n";
        if (write(2, miss_op, strlen(miss_op)) != strlen(miss_op)) {
            return ERR_WRITE;
        }
        return ERR_MISS_OP;
    }
    if ((argc == 4 && strcmp(argv[1], "-n")) || argc == 3) {
        const char *wrong_arg = "tail: wrong argument list\n";
        if (write(2, wrong_arg, strlen(wrong_arg)) != strlen(wrong_arg)) {
            return ERR_WRITE;
        }
        return ERR_WRONG_ARG;
    }
    if (argc > 4) {
        const char *many_args = "cp: too many arguments\n";
        if (write(2, many_args, strlen(many_args)) != strlen(many_args)) {
            return ERR_WRITE;
        }
        return ERR_MANY_ARGS;
    }
    int fd, res;
    if (argc == 2) {
        if ((res = open_file(&fd, argv[1])) != 0) {
            return res;
        }
    } else if ((res = open_file(&fd, argv[3])) != 0) {
        return res;
    }
    if (argc == 4) {
        int from_end = !(argv[2][0] == '+');
        char *end;
        if (argv[2][0] == '-') {
            argv[2][0] = '0';
        }
        ul num = strtoul(argv[2], &end, 10);
        if (num == ULONG_MAX) {
            const char *diapason = "tail: number of lines out of diapason\n";
            if (write(2, diapason, strlen(diapason)) != strlen(diapason)) {
                close(fd);
                return ERR_WRITE;
            }
            close(fd);
            return ERR_DIAPASON;
        }
        if (*end != '\0') {
            const char *wrong_num = "tail: invalid number of lines\n";
            if (write(2, wrong_num, strlen(wrong_num)) != strlen(wrong_num)) {
                close(fd);
                return ERR_WRITE;
            }
            close(fd);
            return ERR_WRONG_NUM;
        }
        return print_tail(from_end, num, &fd);
    }
    return print_tail(1, 10, &fd);
}
