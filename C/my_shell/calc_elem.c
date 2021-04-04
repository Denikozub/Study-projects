//by Denis Kozub

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "stack.h"
#include "calc_elem.h"
#include "rpn.h"

enum {
    ERR_COMMAND_NOT_FOUND = 1,
    ERR_EMPTY_COMMAND = 2,
    ERR_ALLOC = 3,
    ERR_STACK = 4
};

static int
is_operator(const char c)
{
    if (c == '|' || c == '&' || c == '(' || c == ')' || c == '<' || c == '>') {
        return 1;
    }
    return 0;
}

int calculate(size_t *curr_size, size_t rpn_size,
        const void *data, Stack_p *head, int start_inp, int start_out)
{
    #define SAFE_CALL(call, mes) do {                           /**/\
                                     res = call;               /**/  \
                                     if (res == -1) {         /**/    \
                                         perror(mes);        /**/      \
                                         delete_prog(&a);   /**/        \
                                         return errno;     /**/          \
                                     }                    /**/            \
                                 } while (0)

    const char *elem = &((char*)data)[*curr_size + sizeof (El_size)];
    El_size elem_len = ((char*)data)[*curr_size];

    El_size size_str = elem_len - sizeof(Calculate_elem);
    char *str = malloc(size_str + 1);
    if (str == NULL) {
        fprintf(stderr, "Alloc memory error in calculate()\n");
        return ERR_ALLOC;
    }
    memcpy(str, (char*)(elem + sizeof(Calculate_elem)), size_str);
    *(str + size_str) = '\0';

    //NOT OPERATOR
    //push
    char c = str[0];
    if (!is_operator(c)) {
        if (push_stack(head, str, start_inp, start_out, 0, 0, 0)) {
            free(str);
            return ERR_STACK;
        }
        free(str);
        return 0;
    }

    char **a = NULL;
    int input, output, forbidden, res;
    size_t br_begin, br_end;

    //'()'
    //find where brackets start and end, push '(' with this info
    //set current read position of rpn (curr_size) to the end of brackets
    if ((size_str == 1) && (c == '(')) {
        size_t temp_size = *curr_size;
        int temp = 1;
        while (temp_size + sizeof(El_size) +
                ((char*)data)[temp_size] < rpn_size) {
            temp_size += sizeof(El_size) + ((char*)data)[temp_size];
            elem = &((char*)data)[temp_size + sizeof (El_size)];
            elem_len = ((char*)data)[temp_size];
            size_str = elem_len - sizeof(Calculate_elem);
            str = realloc(str, size_str + 1);
            if (str == NULL) {
                fprintf(stderr, "Alloc memory error in calculate()\n");
                free(str);
                return ERR_ALLOC;
            }
            memcpy(str, (char*)(elem + sizeof(Calculate_elem)), size_str);
            *(str + size_str) = '\0';
            if (!strcmp(str, "(")) {
                temp++;
            }
            if (!strcmp(str, ")")) {
                temp--;
                if (temp == 0) {
                    break;
                }
            }
        }
        br_begin = *curr_size + sizeof(El_size) + ((char*)data)[*curr_size];
        br_end = temp_size;
        free(str);
        char *br = "(";
        if (push_stack(head, br, 0, 1, 0, br_begin, br_end)) {
            return ERR_STACK;
        }

        *curr_size = temp_size;
        return 0;
    }

    //OPERATOR '|'
    //pop first operator (second from head), pipe, set i/o and run, no waiting
    if ((size_str == 1) && (c == '|')) {
        free(str);
        if (pop_second_stack(head, &a, &input,
                &output, &forbidden, &br_begin, &br_end)) {
            return ERR_STACK;
        }
        if (a[0] == NULL) {
            free(a);
            return ERR_EMPTY_COMMAND;
        }
        //if || or && was ran before
        if (forbidden) {
            if (input != start_inp) {
                close(input);
            }
            if (output != start_out) {
                close(output);
            }
            delete_prog(&a);
            (*head)->forbidden = forbidden;
            return 0;
        }
        int fd[2];
        SAFE_CALL(pipe(fd), "pipe");
        while (a[0][0] == '(') {
            delete_prog(&a);
            rpn_bracket(br_begin, br_end, rpn_size, data, head, input, fd[1]);

            //get last operator from brackets
            if (pop_stack(head, &a, &input,
                    &output, &forbidden, &br_begin, &br_end)) {
                return ERR_STACK;
            }
            if (a[0] == NULL) {
                delete_prog(&a);
                return ERR_EMPTY_COMMAND;
            }
            //if || or && was ran before
            if (forbidden > 0) {
                delete_prog(&a);
                if (input != start_inp) {
                    close(input);
                }
                if (output != start_out) {
                    close(output);
                }
                (*head)->forbidden = forbidden;
                return 0;
            }
        }
        (*head)->input = fd[0];
        pid_t pid = fork();
        SAFE_CALL(pid, "fork");
        if (pid == 0) {
            SAFE_CALL(dup2(fd[1], 1), "dup2");
            if (input != 0) {
                SAFE_CALL(dup2(input, 0), "dup2");
                if (input != start_inp) {
                    close(input);
                }
            }
            close(fd[0]);
            close(fd[1]);
            execvp(a[0], a);
            fprintf(stderr, "%s : command not found\n", a[0]);
            delete_prog(&a);
            return ERR_COMMAND_NOT_FOUND;
        }
        close(fd[1]);
        delete_prog(&a);
        return 0;
    }

    //OPERATOR '<'
    //change input of head
    if ((size_str == 1) && (c == '<')) {
        free(str);
        if (pop_stack(head, &a, NULL, NULL, NULL, NULL, NULL)) {
            return ERR_STACK;
        }
        if (a[0] == NULL) {
            free(a);
            return ERR_EMPTY_COMMAND;
        }
        int fd_inp = open(a[0], O_RDONLY);
        SAFE_CALL(fd_inp, a[0]);
        (*head)->input = fd_inp;
        delete_prog(&a);
        return 0;
    }

    //OPERATOR '>' and '>>'
    //change output of head
    if ((size_str == 1 && c == '>') || (size_str == 2 && !strcmp(str, ">>"))) {
        free(str);
        if (pop_stack(head, &a, NULL, NULL, NULL, NULL, NULL)) {
            return ERR_STACK;
        }
        if (a[0] == NULL) {
            free(a);
            return ERR_EMPTY_COMMAND;
        }
        int fd_out;
        if (size_str == 1) {
            fd_out = open(a[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
        } else {
            fd_out = open(a[0], O_WRONLY | O_CREAT | O_APPEND, 0664);
        }
        SAFE_CALL(fd_out, a[0]);
        (*head)->output = fd_out;
        delete_prog(&a);
        return 0;
    }

    //OPERATOR '||' and '&&'
    //pop first operator (second from head), run, wait, remember result
    if (size_str == 2 && (!strcmp(str, "||") || !strcmp(str, "&&"))) {
        free(str);
        if (pop_second_stack(head, &a,
                &input, &output, &forbidden, &br_begin, &br_end)) {
            return ERR_STACK;
        }
        if (a[0] == NULL) {
            free(a);
            return ERR_EMPTY_COMMAND;
        }
        //if || or && was ran before
        if (forbidden > 0) {
            delete_prog(&a);
            if (input != start_inp) {
                close(input);
            }
            if (output != start_out) {
                close(output);
            }
            if ((forbidden == 1 && c == '&') || (forbidden == 2 && c == '|')) {
                (*head)->forbidden = 0;
            } else {
                (*head)->forbidden = forbidden;
            }
            return 0;
        }
        while (a[0][0] == '(') {
            delete_prog(&a);
            rpn_bracket(br_begin, br_end, rpn_size, data, head, input, output);

            //get last operator from brackets
            if (pop_stack(head, &a, &input,
                    &output, &forbidden, &br_begin, &br_end)) {
                return ERR_STACK;
            }
            if (a[0] == NULL) {
                delete_prog(&a);
                return ERR_EMPTY_COMMAND;
            }
            //if || or && was ran before
            if (forbidden > 0) {
                delete_prog(&a);
                if (input != start_inp) {
                    close(input);
                }
                if (output != start_out) {
                    close(output);
                }
                if ((forbidden == 1 && c == '&') ||
                        (forbidden == 2 && c == '|')) {
                    (*head)->forbidden = 0;
                } else {
                    (*head)->forbidden = forbidden;
                }
                return 0;
            }
        }
        pid_t pid;
        pid = fork();
        SAFE_CALL(pid, "fork");
        if (pid == 0) {
            if (output != 1) {
                SAFE_CALL(dup2(output, 1), "dup2");
                if (output != start_out) {
                    close(output);
                }
            }
            if (input != 0) {
                SAFE_CALL(dup2(input, 0), "dup2");
                if (input != start_inp) {
                    close(input);
                }
            }
            execvp(a[0], a);
            fprintf(stderr, "%s : command not found\n", a[0]);
            delete_prog(&a);
            return ERR_COMMAND_NOT_FOUND;
        }
        //set permissions to next operator
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            if (c == '|') {
                if (WEXITSTATUS(status) == 0) {
                    (*head)->forbidden = 1;
                } else {
                    (*head)->forbidden = 0;
                }
            } else {
                if (WEXITSTATUS(status) != 0) {
                    (*head)->forbidden = 2;
                } else {
                    (*head)->forbidden = 0;
                }
            }
        }
        delete_prog(&a);
        return 0;
    }

    return 0;

    #undef SAFE_CALL
}
