//by Denis Kozub

#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "rpn.h"
#include "stack.h"
#include "calc_elem.h"

enum {
    ERR_COMMAND_NOT_FOUND = 1,
    ERR_EMPTY_COMMAND = 2,
    ERR_FORK = 3
};

enum {NOT_RUN = 4};

void free_rpn(Rpn *rpn)
{
    if (rpn != NULL) {
        free(rpn->data);
        rpn->data = NULL;
    }
    rpn->size = 0;
}

int rpn_bracket(size_t begin, size_t end, size_t rpn_size,
    const void *data, Stack_p *head, int start_inp, int start_out)
{
    #define SAFE(call) do {                       \
                           res = call;             \
                           if (res) {               \
                               final_stack(head);    \
                               return res;            \
                           }                           \
                       } while (0)

    int res;
    for (size_t curr_size = begin; curr_size < end; curr_size +=
            sizeof(El_size) + ((char*)data)[curr_size]) {
        void *elem = &((char*)data)[curr_size + sizeof (El_size)];
        Calculate_elem func;
        memcpy(&func, (Calculate_elem*)elem, sizeof(Calculate_elem));
        SAFE(func(&curr_size, rpn_size, data, head, start_inp, start_out));
    }
    return 0;

    #undef SAFE
}

int calculate_rpn(Rpn *rpn)
{
    #define CLEAR delete_prog(&a);                            /**/\
                  final_stack(&head);                        /**/  \
                  free_rpn(rpn)

    #define SAFE_CALL(call, mes) do {                         /**/\
                                     res = call;             /**/  \
                                     if (res == -1) {       /**/    \
                                         perror(mes);      /**/      \
                                         delete_prog(&a); /**/        \
                                         return errno;   /**/          \
                                     }                  /**/            \
                                 } while (0)

    #define SAFE(call) do {                                   /**/\
                           res = call;                       /**/  \
                           if (res) {                       /**/    \
                               final_stack(&head);         /**/      \
                               free_rpn(rpn);             /**/        \
                               return res;               /**/          \
                           }                            /**/            \
                       } while (0)

    int res;
    Stack_p head;
    SAFE(init_stack(&head));

    for (size_t curr_size = 0; curr_size < rpn->size; curr_size +=
            sizeof(El_size) + ((char *)rpn->data)[curr_size]) {
        void *elem = &((char *)rpn->data)[curr_size + sizeof (El_size)];
        Calculate_elem func;
        memcpy(&func, (Calculate_elem*)elem, sizeof(Calculate_elem));
        SAFE(func(&curr_size, rpn->size, rpn->data, &head, 0, 1));
    }

    //runs last command of whole line which is on the top of stack
    char **a = NULL;
    int input, output, forbidden;
    size_t br_begin, br_end;

    SAFE(pop_stack(&head, &a, &input, &output, &forbidden, &br_begin, &br_end));
    if (a[0] == NULL) {
        CLEAR;
        return ERR_EMPTY_COMMAND;
    }
    //check if || or && was run before
    if (forbidden) {
        if (input != 0)
            close(input);
        if (output != 1)
            close(output);
        CLEAR;
        if (forbidden == 1)
            return 0;
        return NOT_RUN;
    }
    while (a[0][0] == '(') {
        delete_prog(&a);
        rpn_bracket(br_begin, br_end,
                rpn->size, rpn->data, &head, input, output);

        //get last command of brackets
        SAFE(pop_stack(&head, &a,
                &input, &output, &forbidden, &br_begin, &br_end));
        if (a[0] == NULL) {
            CLEAR;
            return ERR_EMPTY_COMMAND;
        }
        //check if || or && was run before
        if (forbidden) {
            if (input != 0)
                close(input);
            if (output != 1)
                close(output);
            CLEAR;
            if (forbidden == 1)
                return 0;
            return NOT_RUN;
        }
    }
    pid_t pid;
    if ((pid = fork()) == -1) {
        perror("fork");
        CLEAR;
        return ERR_FORK;
    }
    if (pid == 0) {
        if (output != 1) {
            SAFE_CALL(dup2(output, 1), "dup2");
            close(output);
        }
        if (input != 0) {
            SAFE_CALL(dup2(input, 0), "dup2");
            close(input);
        }
        execvp(a[0], a);
        fprintf(stderr, "%s : command not found\n", a[0]);
        CLEAR;
        return ERR_COMMAND_NOT_FOUND;
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        CLEAR;
        return WEXITSTATUS(status);
    }

    while (wait(NULL) > 0);
    CLEAR;
    return 0;

    #undef SAFE
    #undef CLEAR
    #undef SAFE_CALL
}
