//by Denis Kozub

#include <stdlib.h>
#include <stdio.h>
#include "polis.h"
#include "recursive_descent.h"
#include "var_table.h"

#define SAFE(call) do { \
                       res = call; \
                       if (res) { \
                           goto FINISH; \
                       } \
                   } while(0)

int
main(void)
{
    int res;
    
    Vars table;
    Polis polis = {NULL, 0};
    
    SAFE(init_var_table(&table));
    printf("Enter variables ('stop' to finish): \n");
    SAFE(input_var_table(&table));
    
    printf("Enter expression: \n");
    SAFE(descent_polis(&polis));
    
    int *resp;
    size_t size_res;
    SAFE(calculate_polis(&polis, (void**)&resp, &size_res, &table));
    printf("Answer is: %d\n", *resp);
    free(resp);
    
FINISH:
    free_polis(&polis);
    free_var_table(&table);
    return 0;
}
