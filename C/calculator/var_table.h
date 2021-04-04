//by Denis Kozub
//IF INPUT_VAR_TABLE() RUNS CORRECTLY MEMORY AT TABLE IS NOT FREED
//IF AN ERROR HAPPENS ALL MEMORY IS FREED

#ifndef H_VAR_TABLE
#define H_VAR_TABLE

typedef struct Vars {
    char **name;
    int *value;
    int number;
} Vars;

int init_var_table(Vars *table);

int input_var_table(Vars *table);

void free_var_table(Vars *table);

#endif
