//by Denis Kozub
//stack with pop_second function for retrieving rpn first operand
//command parser for execvp inplemented into pop function
//push_stack() allocates memory
//pop_stack() and pop_second_stack() do not free memory at char ***prog

#ifndef H_STACK
#define H_STACK

typedef struct Stack_struct {
    char **prog;
    int input;
    int output;
    int forbidden;
    size_t br_begin;
    size_t br_end;
    struct Stack_struct *next;
} Stack_struct;

typedef Stack_struct * Stack_p;

int init_stack(Stack_p *head);

int push_stack(Stack_p *head, char *prog_line,
        int input, int output, int forbidden, size_t br_begin, size_t br_end);

int pop_second_stack(Stack_p *head, char ***prog_p, int *input,
        int *output, int *forbidden, size_t *br_begin, size_t *br_end);

int pop_stack(Stack_p *head, char ***prog_p, int *input,
        int *output, int *forbidden, size_t *br_begin, size_t *br_end);

void delete_prog (char ***a);

void final_stack(Stack_p *head);

#endif
