//by Denis Kozub
//

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <errno.h>
#include "recursive_descent.h"
#include "polis.h"
#include "stack.h"

//ignore spaces
#define SKIP_SPACE while (curr_char == ' ') \
                             next_char() \

enum {
    ERR_BRACKET = 1,
    ERR_MISS_OPERAND = 2,
    ERR_MISS_OPERATION = 3,
    ERR_WRONG_VARIABLE = 4,
    ERR_WRONG_NUMBER = 5,
    ERR_ALLOC = 6,
    ERR_STACK = 7,
    ERR_DIVIDE_BY_Z = 8,
    ERR_SIZE_MATCH = 9,
    ERR_NO_VAR = 10
};

enum {MAX_VAR_SYMBOLS = 6};

static Polis polis = {NULL, 0};
static int ind = 0, instr_len = 0;
static char curr_char;
static jmp_buf env;

//first symbol of variable
static int first_symbol_correct(const char c);
//second to last symbols of num
static int var_symbol_incorrect(const char c);
static int is_digit(const char c);
static int is_operator(const char c);
//if entered variable is correct add it to polis
//if incorrect send an error message and terminate
static void check_var();
//works like check_var() for numbers
static void check_num();
//function which knows what to do with data
//working scenarios depend on whether element is operator or a variable/number
//is added before each element when adding to polis
static int calculate(const void *elem, const Size_elem elem_len,
        Stack_p *head, const Vars *table);
//adds size of element and an element, which consists of pointer to calculate()
//...and data itself, to the array of polis
static void add_outstr(const char *str, const Size_elem size);
//reading next symbol
static void next_char();

//recursive descent algirithm:

//function that starts recursion
static void convert();
//divide expression into A +- B etc
static void begin();
//divide expression into A */ B etc
static void mult_div();
//work with symbols: (, ), number, variable
static void symbol();

static int
first_symbol_correct(const char c)
{
    if (((c >= 'a') && (c <= 'z')) ||
            ((c >= 'A') && (c <= 'Z')) || (c == '_')) {
        return 1;
    }
    return 0;
}

static int
var_symbol_incorrect(const char c)
{
    if (((c >= 'a') && (c <= 'z')) || ((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'Z')) || (c == '_')){
        return 0;
    }
    return 1;
}

static int
is_digit(const char c)
{
    if ((c >= '0') && (c <= '9')) {
        return 1;
    }
    return 0;
}

static int
is_operator(const char c)
{
    if ((c == '+') || (c == '-') || (c == '*') || (c == '/')) {
        return 1;
    }
    return 0;
}

static void
check_var()
{
    char *var = NULL;
    Size_elem n = 1;
    while ((curr_char != ' ') && (curr_char != ')') && (curr_char != '\0') &&
            (!is_operator(curr_char)) && (n <= MAX_VAR_SYMBOLS)) {
        if (var_symbol_incorrect(curr_char)) {
            free(var);
            fprintf(stderr, "Wrong variable entered\n");
            longjmp(env, ERR_WRONG_NUMBER);
        }
        var = (char *)realloc(var, n * sizeof(curr_char));
        if (var == NULL) {
            fprintf(stderr, "Alloc memory error in check_var()\n");
            longjmp(env, ERR_ALLOC);
        }
        var[n - 1] = curr_char;
        ++n;
        next_char();
    }
    add_outstr(var, n - 1);
    free(var);
}

static void
check_num()
{
    char *num = NULL;
    Size_elem n = 1;
    while ((curr_char != ' ') && (curr_char != ')') &&
            (!is_operator(curr_char)) && (curr_char != '\0')) {
        if (!is_digit(curr_char)) {
            free(num);
            fprintf(stderr, "Wrong number entered\n");
            longjmp(env, ERR_WRONG_NUMBER);
        }
        num = (char *)realloc(num, n * sizeof(curr_char));
        if (num == NULL) {
            fprintf(stderr, "Alloc memory error in check_num()\n");
            longjmp(env, ERR_ALLOC);
        }
        num[n - 1] = curr_char;
        ++n;
        next_char();
    }
    add_outstr(num, n - 1);
    free(num);
}

static int
calculate(const void *elem, const Size_elem elem_len,
        Stack_p *head, const Vars *table)
{
    Size_elem size_str = elem_len - sizeof(Calculate_elem);
    char *str = malloc(size_str + 1);
    
    if (str == NULL) {
        fprintf(stderr, "Alloc memory error in calculate()\n");
        return(ERR_ALLOC);
    }
    memcpy(str, (char*)(elem + sizeof(Calculate_elem)), size_str);
    *(str + size_str) = '\0';
    
    //if data is an operator
    //pop, pop, operator, push
    if ((size_str == 1) && is_operator((char)(*str))) {
        int *a, *b, res;
        size_t size_a, size_b;
        if (pop_stack(head, (void**)(&a), &size_a)) {
            free(str);
            return(ERR_STACK);
        }
        if (pop_stack(head, (void**)(&b), &size_b)) {
            free(str);
            return(ERR_STACK);
        }
        if (size_a != size_b) {
            free(str);
            fprintf(stderr, "Sizes do not match in calculate()\n");
            return(ERR_SIZE_MATCH);
        }
        char c = (char)(*str);
        switch(c) {
        case '+':
            res = *b + *a;
            //printf("%d+%d=%d\n", *b, *a, res);
            break;
        case '-':
            res = *b - *a;
            //printf("%d-%d=%d\n", *b, *a, res);
            break;
        case '*':
            res = *b * *a;
            //printf("%d*%d=%d\n", *b, *a, res);
            break;
        case '/':
            if (*a == 0) {
                fprintf(stderr, "Division by zero in calculate()\n");
                free(str);
                free(a);
                free(b);
                return(ERR_DIVIDE_BY_Z);
            }
            res = *b / *a;
            //printf("%d/%d=%d\n", *b, *a, res);
            break;
        }
        free(a);
        free(b);
        if (push_stack(head, (void*)(&res), sizeof(res))) {
            free(str);
            return(ERR_STACK);
        }
    } else {
        //if data is number: push
        //if data is a variable: check in var_table and push value
        if (is_digit((char)(*str))) { //if a number
            int num;
            num = atoi(str);
            //printf("push %d\n", num);
            if (push_stack(head, (void*)(&num), sizeof(num))) {
                free(str);
                return(ERR_STACK);
            }
        } else { //if a variable
            int num, set = 0;
            for (int i = 0; i < table->number; ++i) {
                if (!strcmp(table->name[i], str)) {
                    num = table->value[i];
                    set = 1;
                }
            }
            if (set == 0) {
                fprintf(stderr, "Variable %s does not exist\n", str);
                free(str);
                return(ERR_NO_VAR);
            }
            //printf("push %d\n", num);
            if (push_stack(head, (void*)(&num), sizeof(num))) {
                free(str);
                return(ERR_STACK);
            }
        }
    }
    free(str);
    return 0;
}

static void
add_outstr(const char *str, const Size_elem size_str)
{
    Size_elem elem_len = size_str + sizeof(Calculate_elem);
    polis.size += elem_len + sizeof(Size_elem);
    polis.data = (char*)realloc((char*)polis.data, polis.size);
    if ((char*)polis.data == NULL) {
        fprintf(stderr, "Alloc memory error in add_outstr()\n");
        longjmp(env, ERR_ALLOC);
    }
    Calculate_elem func = calculate;
    //size + func + data
    memcpy((char*)(polis.data + polis.size - elem_len - sizeof(Size_elem)),
            &elem_len, sizeof(elem_len));
    memcpy((char*)(polis.data + polis.size - elem_len), &func, sizeof(func));
    memcpy((char*)(polis.data + polis.size - size_str), str, size_str);
}

static void
next_char()
{
    int inp_symbol;
    ++ind;
    if (((inp_symbol = getchar()) == '\n') || (inp_symbol == EOF)) {
        curr_char = '\0';
    } else {
        curr_char = (char)inp_symbol;
        ++instr_len;
    }
}

static void
convert()
{
    next_char();
    SKIP_SPACE;
    begin();
    if ((ind != (instr_len + 1)) || (curr_char != '\0')) {
        fprintf(stderr, "Operation missing\n");
        longjmp(env, ERR_MISS_OPERATION);
    }
}

static void
begin()
{
    mult_div();
    while ((curr_char == '+') || (curr_char == '-')) {
        char temp = curr_char;
        next_char();
        SKIP_SPACE;
        mult_div();
        add_outstr(&temp, sizeof(curr_char));
    }
}

static void
mult_div()
{
    symbol();
    while ((curr_char == '*') || (curr_char == '/')) {
        char temp = curr_char;
        next_char();
        SKIP_SPACE;
        symbol();
        add_outstr(&temp, sizeof(curr_char));
    }
}

static void
symbol()
{
    if (curr_char == '(') {
        next_char();
        SKIP_SPACE;
        begin();
        if (curr_char != ')') {
            fprintf(stderr, "Brackets imbalance\n");
            longjmp(env, ERR_BRACKET);
        } else {
            next_char();
            SKIP_SPACE;
        }
    } else {
        SKIP_SPACE;
        if (is_digit(curr_char)) { //if a number
            check_num();
            SKIP_SPACE;
        } else if (first_symbol_correct(curr_char)) { //if a variable
            check_var();
            SKIP_SPACE;
        } else {
            fprintf(stderr, "Operand missing\n");
            longjmp(env, ERR_MISS_OPERAND);
        }
    }
}

int
descent_polis(Polis *return_polis)
{
    int val = setjmp(env);
    if(val) {
        free_polis(&polis);
        return val;
    }
    convert();
    *return_polis = polis;
    return 0;
}
