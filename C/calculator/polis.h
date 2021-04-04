//by Denis Kozub
//IF CALCULATE_POLIS() RUNS CORRECTLY MEMORY AT RESP IS NOT FREED
//OTHERWISE MEMORY IS NOT ALLOCATED

#ifndef H_POLIS
#define H_POLIS

#include <stddef.h>
#include "stack.h"
#include "var_table.h"

typedef struct
{
    void *data;
    size_t size;
} Polis;

typedef char Size_elem; //1 byte

typedef int (*Calculate_elem)(const void *elem,
        const Size_elem size, Stack_p *head, const Vars *table);

int calculate_polis(const Polis *polis, void **resp,
        size_t *size_res, const Vars *table);

void free_polis(Polis *polis);

#endif
