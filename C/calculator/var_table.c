//by Denis Kozub
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "var_table.h"

enum {
    ERR_EMPTY_TABLE = 1,
    ERR_END_INPUT = 2,
    ERR_FIRST_SYMBOL = 3,
    ERR_WRONG_SYMBOL = 4,
    ERR_ALLOC = 5,
    ERR_REACH_MAX = 6,
    ERR_WRONG_DIGIT = 7,
    ERR_EXTRA_SPACE = 8,
    ERR_LONE_MINUS = 9
};

enum {MAX_VAR_SYMBOLS = 6};

static int
first_symbol_incorrect(const char c)
{
    if (((c >= 'a') && (c <= 'z')) ||
            ((c >= 'A') && (c <= 'Z')) || (c == '_')) {
        return 0;
    }
    return 1;
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
is_not_digit(const char c)
{
    if ((c >= '0') && (c <= '9')) {
        return 0;
    }
    return 1;
}

int
init_var_table(Vars *table)
{
    if (table == NULL) {
        fprintf(stderr, "Empty table in init_var_table()\n");
        return ERR_EMPTY_TABLE;
    }
    table->name = NULL;
    table->value = NULL;
    table->number = 0;
    return 0;
}

int
input_var_table(Vars *table)
{
    const char *stop_phrase = "stop";
    while(1) {
        if (table == NULL) {
            fprintf(stderr, "Empty table in init_var_table()\n");
            return ERR_EMPTY_TABLE;
        }
        //NAME OF VARIABLE INPUT
        char *var = NULL;
        int var_length;
        int c, n = 0;
        while (((c = getchar()) != '=') && ((n == 0) || (c != '\n')) &&
            ((n == 0) || (c != ' ')) && (c != EOF) && (n <= MAX_VAR_SYMBOLS)) {
            if ((c == ' ') && (n == 0)) { //ignore spaces
                continue;
            }
            if ((c == '\n') && (n == 0)) { //ignore enters
                continue;
            }
            if ((n == 0) && (first_symbol_incorrect(c))) {
                fprintf(stderr, "Wrong first symbol\n");
                return ERR_FIRST_SYMBOL;
            }
            if (var_symbol_incorrect(c)) {
                free(var);
                fprintf(stderr, "Incorrect symbol\n");
                return ERR_WRONG_SYMBOL;
            }
            var = (char *)realloc(var, (++n + 1) * sizeof(*var));
            if (var == NULL) {
                fprintf(stderr, "Alloc memory error in input_var()\n");
                return ERR_ALLOC;
            }
            var[n - 1] = (char)c;
            var[n] = '\0'; //to be able to compare
        }
        if ((n > 0) && (!strcmp(var, stop_phrase))) { //if stop_phrase entered
            free(var);
            return 0;
        }
        if ((c == '\n') || (c == EOF)) {
            free(var);
            fprintf(stderr, "Input stopped too early\n");
            return ERR_END_INPUT;
        }
        if (n > MAX_VAR_SYMBOLS) { //check length
            free(var);
            fprintf(stderr, "Too many symbols in variable\n");
            return ERR_REACH_MAX;
        }
        var_length = n;
        //if '=' or ' ' found
        while (c == ' ') { //ignore spaces
            c = getchar();
            if ((c != ' ') && (c != '=')) {
                if (c == EOF) {
                    free(var);
                    fprintf(stderr, "Input stopped too early\n");
                    return ERR_END_INPUT;
                }
                free(var);
                fprintf(stderr, "Extra space\n");
                return ERR_EXTRA_SPACE;
            }
        }
        //VALUE OF VARIABLE INPUT
        char *num = NULL;
        n = 0;
        while (((c = getchar()) != '\n') && (c != EOF) &&
                ((n == 0) || (c != ' '))) {
            if ((c == ' ') && (n == 0)) { //ignore spaces
                continue;
            }
            if (is_not_digit(c) && ((c != '-') || (n != 0))) { //allow minus
                free(var);
                free(num);
                fprintf(stderr, "Incorrect digit\n");
                return ERR_WRONG_DIGIT;
            }
            num = (char *)realloc(num, (++n + 1) * sizeof(*num));
            if (num == NULL) {
                free(var);
                fprintf(stderr, "Alloc memory error in input_var()\n");
                return ERR_ALLOC;
            }
            num[n - 1] = (char)c;
            num[n] = '\0'; //to be able to compare
        }
        if (c == EOF) {
            free(var);
            free(num);
            fprintf(stderr, "Input stopped too early\n");
            return ERR_END_INPUT;
        }
        if ((n == 0) && (c == '\n')) {
            free(var);
            free(num);
            fprintf(stderr, "Input stopped too early\n");
            return ERR_END_INPUT;
        }
        if ((n == 1) && ((char)(*num) == '-')) { //only minus entered
            free(var);
            free(num);
            fprintf(stderr, "Lone minus left\n");
            return ERR_LONE_MINUS;
        }
        while (c == ' ') { //ignore spaces
            c = getchar();
            if ((c != ' ') && (c != '\n')) {
                if (c == EOF) {
                    fprintf(stderr, "Input stopped too early\n");
                    return ERR_END_INPUT;
                }
                fprintf(stderr, "Extra space\n");
                return ERR_EXTRA_SPACE;
            }
        }
        int value;
        value = atoi(num);
        free(num);
        //fill struct
        ++(table->number);
        table->value = realloc(table->value,
                table->number * sizeof(*table->value));
        if (table->value == NULL) {
            free(var);
            fprintf(stderr, "Alloc memory error in input_var()\n");
            return ERR_ALLOC;
        }
        table->value[table->number - 1] = value;
        table->name = (char**)realloc(table->name,
                (table->number + 1) * sizeof(*table->name));
        if (table->name == NULL) {
            free(var);
            free(table->value);
            table->value = NULL;
            fprintf(stderr, "Alloc memory error in input_var()\n");
            return ERR_ALLOC;
        }
        table->name[table->number] = NULL;
        table->name[table->number - 1] = malloc(var_length * sizeof(char) + 1);
        if (table->name[table->number - 1] == NULL) {
            free(var);
            free(table->value);
            table->value = NULL;
            for(int i = 0; i < table->number - 1; i++) {
                free(table->name[i]);
                table->name[i] = NULL;
            }
            free(table->name);
            table->name = NULL;
            fprintf(stderr, "Alloc memory error in input_var()\n");
            return ERR_ALLOC;
        }
        strcpy(table->name[table->number - 1], var);
        table->name[table->number] = '\0';
        free(var);
    }
    return 0;
}

void
free_var_table(Vars *table)
{
    if (table == NULL) {
        return;
    }
    free(table->value);
    table->value = NULL;
    for(int i = 0; i < table->number; i++) {
        free(table->name[i]);
        table->name[i] = NULL;
    }
    free(table->name);
    table->name = NULL;
}
