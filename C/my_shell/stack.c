//by Denis Kozub

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define SAFE_ALLOC(res) if (res == NULL) { \
                            perror("Alloc"); \
                            final_stack(head); \
                            return 1; \
                        }

int
init_stack(Stack_p *head)
{
    if (head == NULL) {
        fprintf(stderr, "Empty head pointer in init_stack\n");
        return 1;
    }
    *head = NULL;
    return 0;
}

int
push_stack(Stack_p *head, char *prog_line, int input,
    int output, int forbidden, size_t br_begin, size_t br_end)
{
    if (head == NULL || prog_line == NULL) {
        fprintf(stderr, "Wrong arguments in push_stack()\n");
        return 1;
    }
    Stack_p temp = malloc(sizeof(*temp));
    SAFE_ALLOC(temp);

    temp->next = *head;
    temp->prog = NULL;
    temp->input = input;
    temp->output = output;
    temp->forbidden = forbidden;
    temp->br_begin = br_begin;
    temp->br_end = br_end;

    //parser
    char sep = ' ', *str = NULL;
    int count = 0;
    str = strtok(prog_line, &sep);
    while (str != NULL) {
        temp->prog = realloc(temp->prog, ++count * sizeof(char*));
        SAFE_ALLOC(temp->prog);
        (temp->prog)[count - 1] = malloc(strlen(str) + 1);
        SAFE_ALLOC((temp->prog)[count - 1]);
        strcpy((temp->prog)[count - 1], str);
        str = strtok(NULL, &sep);
    }
    temp->prog = realloc(temp->prog, ++count * sizeof(char*));
    SAFE_ALLOC(temp->prog);
    (temp->prog)[count - 1] = NULL;

    *head = temp;
    return 0;
}

//get second element after head
int
pop_second_stack(Stack_p *head, char ***prog_p,
        int *input, int *output, int *forbidden, size_t *br_begin, size_t *br_end)
{
    if (head == NULL || prog_p == NULL) {
        fprintf(stderr, "Wrong arguments in pop_stack()\n");
        return 1;
    }
    if (*head == NULL) {
        return 1;
    }
    if ((*head)->next == NULL) {
        return 1;
    }
    if (input != NULL) {
        *input = (*head)->next->input;
    }
    if (output != NULL) {
        *output = (*head)->next->output;
    }
    if (forbidden != NULL) {
        *forbidden = (*head)->next->forbidden;
    }
    if (br_begin != NULL) {
        *br_begin = (*head)->next->br_begin;
    }
    if (br_end != NULL) {
        *br_end = (*head)->next->br_end;
    }
    memcpy(prog_p, &((*head)->next->prog), sizeof(*prog_p));
    Stack_p temp = (*head)->next;
    (*head)->next = temp->next;
    free(temp);
    return 0;
}

int
pop_stack(Stack_p *head, char ***prog_p,
        int *input, int *output, int *forbidden, size_t *br_begin, size_t *br_end)
{
    if (head == NULL || prog_p == NULL) {
        fprintf(stderr, "Wrong arguments in pop_stack()\n");
        return 1;
    }
    if (*head == NULL) {
        return 1;
    }
    if (input != NULL)  {
        *input = (*head)->input;
    }
    if (output != NULL) {
        *output = (*head)->output;
    }
    if (forbidden != NULL) {
        *forbidden = (*head)->forbidden;
    }
    if (br_begin != NULL) {
        *br_begin = (*head)->br_begin;
    }
    if (br_end != NULL) {
        *br_end = (*head)->br_end;
    }
    memcpy(prog_p, &(*head)->prog, sizeof(*prog_p));
    Stack_p temp = *head;
    *head = (*head)->next;
    free(temp);
    temp = NULL;
    return 0;
}

//free allocated memory at stack element
void
delete_prog (char ***a)
{
    if (*a == NULL) {
        return;
    }
    int temp = -1;
    while((*a)[++temp] != NULL) {
        free((*a)[temp]);
        (*a)[temp] = NULL;
    }
    free(*a);
    *a = NULL;
}

//free all allocated memory
void
final_stack(Stack_p *head)
{
    char **temp_res = NULL;
    while (!pop_stack(head, &temp_res, NULL, NULL, NULL, NULL, NULL));
    delete_prog(&temp_res);
}

#undef SAFE_ALLOC
