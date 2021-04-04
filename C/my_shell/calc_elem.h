//by Denis Kozub
//a function which knows what to do with each element of rpn

#ifndef H_CALC_ELEM
#define H_CALC_ELEM


typedef int (*Calculate_elem)(size_t *curr_size, size_t rpn_size,
        const void *data, Stack_p *head, int start_inp, int start_out);

int calculate(size_t *curr_size, size_t rpn_size,
        const void *data, Stack_p *head, int start_inp, int start_out);

#endif
