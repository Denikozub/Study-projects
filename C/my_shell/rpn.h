//by Denis Kozub
//calculate_rpn to run each rpn element
//rpn_bracket ro run elements from begin to end of brackets

#ifndef H_RPN
#define H_RPN

#include "stack.h"

typedef struct
{
    void *data;
    size_t size;
} Rpn;

typedef unsigned short El_size; //2 bytes

void free_rpn(Rpn *rpn);

int rpn_bracket(size_t begin, size_t end, size_t rpn_size,
    const void *data, Stack_p *head, int start_inp, int start_out);

int calculate_rpn(Rpn *rpn);

#endif
