//by Denis Kozub
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

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
push_stack(Stack_p *head, const void *data_p, const size_t size_data)
{
    if (head == NULL || data_p == NULL || size_data <= 0) {
        fprintf(stderr, "Wrong arguments in push_stack()\n");
        return 1;
    }
    Stack_p temp = malloc(sizeof(*temp));
    if (temp == NULL) {
        fprintf(stderr, "Alloc error in push_stack()\n");
        return 1;
    }
    temp->next = *head;
    temp->data_p = malloc(size_data);
    memcpy(temp->data_p, data_p, size_data);
    temp->size_data = size_data;
    *head = temp;
    return 0;
}

int
pop_stack(Stack_p *head, void **res_p, size_t *size_res)
{
    if (head == NULL || res_p == NULL || size_res == NULL) {
        fprintf(stderr, "Wrong arguments in pop_stack()\n");
        return 1;
    }
    if (*head == NULL) {
        return 1;
    }
    memcpy(res_p, &(*head)->data_p, sizeof(*res_p));
    *size_res = (*head)->size_data;
    Stack_p temp = *head;
    *head = (*head)->next;
    free(temp);
    return 0;
}

void
final_stack(Stack_p *head)
{
    void *temp_res = NULL;
    size_t temp_size;
    while (!pop_stack(head, &temp_res, &temp_size));
    free(temp_res);
}
