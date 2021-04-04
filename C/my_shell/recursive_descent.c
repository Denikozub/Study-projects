//by Denis Kozub

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>
#include "recursive_descent.h"
#include "rpn.h"
#include "calc_elem.h"

enum {
    ERR_BRACKET = 1,
    ERR_WRONG_OPERAND = 2,
    ERR_WRONG_COMMAND_LINE = 3,
    ERR_ALLOC = 4
};

enum {FOUND_EOF = 5};

static Rpn rpn;
static int ind, instr_len, flag_no_wait;
static char curr_char;
static jmp_buf env;

static void add_outstr(char *str, const El_size size_str);
static void convert();
static void begin();
static void symbol();
static void redirect();

static int
is_operator(const char c)
{
    if (c == '|' || c == '&' || c == '(' || c == ')' || c == '<' || c == '>') {
        return 1;
    }
    return 0;
}

static void
next_char()
{
    int inp_symbol;
    ++ind;
    if ((inp_symbol = getchar()) == EOF) {
        longjmp(env, FOUND_EOF);
    }
    if ((flag_no_wait) || (inp_symbol == '\n')) {
        curr_char = '\0';
    } else {
        curr_char = (char)inp_symbol;
        ++instr_len;
    }
}

//when non-operator found
static void
add_command()
{
    char *command = NULL;
    El_size n = 1;
    while ((curr_char != ')') && (curr_char != '\0') &&
            (!is_operator(curr_char))) {
        command = (char *)realloc(command, n * sizeof(curr_char));
        if (command == NULL) {
            fprintf(stderr, "Alloc memory error in check_var()\n");
            longjmp(env, ERR_ALLOC);
        }
        command[n - 1] = curr_char;
        ++n;
        next_char();
    }
    add_outstr(command, n - 1);
    free(command);
}

static void
add_outstr(char *str, const El_size size_str)
{
    El_size elem_len = size_str + sizeof(Calculate_elem);
    rpn.size += elem_len + sizeof(El_size);
    rpn.data = (char*)realloc((char*)rpn.data, rpn.size);
    if ((char*)rpn.data == NULL) {
        fprintf(stderr, "Alloc memory error in add_outstr()\n");
        free(str);
        longjmp(env, ERR_ALLOC);
    }
    Calculate_elem func = calculate;
    memcpy((char*)(rpn.data + rpn.size - elem_len - sizeof(El_size)),
            &elem_len, sizeof(elem_len));
    memcpy((char*)(rpn.data + rpn.size - elem_len), &func, sizeof(func));
    memcpy((char*)(rpn.data + rpn.size - size_str), str, size_str);
}

//start descent
static void
convert()
{
    rpn.data = NULL;
    rpn.size = 0;
    ind = 0;
    instr_len = 0;
    flag_no_wait = 0;

    next_char();
    begin();
    if ((ind != (instr_len + 1)) || (curr_char != '\0')) {
        fprintf(stderr, "Wrong command line\n");
        longjmp(env, ERR_WRONG_COMMAND_LINE);
    }
}

//handle ||, &&, |, & - all same lowest priority
static void
begin()
{
    redirect();
    while ((curr_char == '|') || (curr_char == '&')) {
        char temp = curr_char;
        next_char();
        if (curr_char == '\0' && temp == '&') {
            flag_no_wait = 1;
            return;
        } else if (curr_char == ' ') {
            redirect();
            add_outstr(&temp, sizeof(temp));
        } else if ((curr_char == '|') || (curr_char == '&')) {
            next_char();
            char *buf;
            if (temp == '&') {
                buf = "&&";
            } else {
                buf = "||";
            }
            redirect();
            add_outstr(buf, 2);
        } else {
            fprintf(stderr, "Wrong operand\n");
            longjmp(env, ERR_WRONG_OPERAND);
        }
    }
}

//handle <, >, >> - higher priority
static void
redirect()
{
    symbol();
    while ((curr_char == '<') || (curr_char == '>')) {
        char temp = curr_char;
        next_char();
        if (curr_char == ' ') {
            symbol();
            add_outstr(&temp, sizeof(temp));
        } else if (curr_char == '>') {
            next_char();
            char *buf = ">>";
            symbol();
            add_outstr(buf, 2);
        } else {
            fprintf(stderr, "Wrong operand\n");
            longjmp(env, ERR_WRONG_OPERAND);
        }
    }
}

//handle commands and () - max priority
static void
symbol()
{
    while (curr_char == ' ') {
        next_char();
    }
    if (curr_char == '(') {
        add_outstr(&curr_char, sizeof(curr_char));
        next_char();
        begin();
        if (curr_char != ')') {
            fprintf(stderr, "Brackets imbalance\n");
            longjmp(env, ERR_BRACKET);
        } else {
            add_outstr(&curr_char, sizeof(curr_char));
            next_char();
            while (curr_char == ' ') {
                next_char();
            }
        }
    } else {
        add_command();
    }
}

int
descent_rpn(int pipe_write)
{
    int val = setjmp(env);
    if (val) {
        flag_no_wait = 2;
        write(pipe_write, &flag_no_wait, sizeof(flag_no_wait));
        free_rpn(&rpn);
        return val;
    }
    convert();
    write(pipe_write, &flag_no_wait, sizeof(flag_no_wait));
    return calculate_rpn(&rpn);
}
