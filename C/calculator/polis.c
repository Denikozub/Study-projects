//by Denis Kozub
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "polis.h"
#include "var_table.h"

#define SAFE(call) do { \
                       res = call; \
                       if (res) { \
                           final_stack(&head); \
                           return res; \
                       } \
                   } while(0)

int
calculate_polis(const Polis *polis, void **resp,
        size_t *size_res, const Vars *table)
{
    int res;
    Stack_p head;
    SAFE(init_stack(&head));
    for (size_t curr_size = 0; curr_size < polis->size; curr_size +=
            sizeof(Size_elem) + ((char *)polis->data)[curr_size]) {
        void *elem = &((char *)polis->data)[curr_size + sizeof (Size_elem)];
        Calculate_elem func;
        memcpy(&func, (Calculate_elem*)elem, sizeof(Calculate_elem));
        SAFE(func(elem, ((char *)polis->data)[curr_size], &head, table));
    }
    SAFE(pop_stack(&head, resp, size_res));
    
    final_stack(&head);
    return 0;
}

void
free_polis(Polis *polis)
{
    if (polis != NULL) {
        free(polis->data);
        polis->data = NULL;
    }
}
