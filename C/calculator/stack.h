//by Denis Kozub
//IF POP_STACK() RUNS CORRECTLY MEMORY AT RES_P IS NOT FREED
//OTHERWISE MEMORY IS NOT ALLOCATED

#ifndef H_STACK
#define H_STACK

#include <stddef.h>

typedef struct Stack_struct {
    void *data_p;
    size_t size_data;
    struct Stack_struct *next;
} Stack_struct;

typedef Stack_struct * Stack_p;

int init_stack(Stack_p *head);

int push_stack(Stack_p *head, const void *data_p, const size_t size_data);

int pop_stack(Stack_p *head, void **res_p, size_t *size_res);

void final_stack(Stack_p *head);

#endif
