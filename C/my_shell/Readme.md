# Shell
_by Denis Kozub_

### 1. Main points:
- operators: __||__, __&&__, __|__, __&__, __()__, __<__, __>__, __>>__
- __&__ is possible only in the end of the line
- stdin only  

### 2. Algorithm:
- input string parsing using recursive descent creating reverse polish notation (rpn)
- each element of rpn consists of its size, pointer to a function working with it and string data
- string data can be either a command or an operator
- function operation depends on type of string data of each element and type of operator in particular

### 3. Header files:
- calc_elem.h - function operating rpn elements
- recursive_descent.h - function parsing input string creating rpn
- rpn.h - functions reading rpn elements one by one and calling calc_elem function for each
- stack.h - stack implementation for commands and their utilities with command argument parser

##### calc_elem.h
~~~
int calculate(size_t *curr_size, size_t rpn_size, const void *data,
        Stack_p *head, int start_inp, int start_out);
~~~
1) arguments:  
- size_t *curr_size - current position in rpn
- size_t rpn_size - size of rpn
- const void *data - string of rpn
- Stack_p *head - address of head pointer
- int start_inp - input file descriptor (may be != 0 for brackets)
- int start_out - output file descriptor (may be != 1 for brackets)  

2) working scenarios:
- command -> push
- __()__ -> find where brackets start and end, push '(' with this info; set current read position of rpn (curr_size) to the end of brackets
- __|__ -> pop first operator (second from head), pipe, set i/o and run, no waiting
- __<__ -> change input of head
- __>__ and __>>__ -> change output of head
- pop first operator (second from head), run, wait, remember result
- __||__, __&&__ -> pop first operator (second from head), run, wait, remember result in stack  

3) return value:
- 0 if correct
- ERR_COMMAND_NOT_FOUND = 1
- ERR_EMPTY_COMMAND = 2
- ERR_ALLOC = 3
- ERR_STACK = 4

##### recursive_descent.h
~~~
int descent_rpn(int pipe_write);
~~~
1) arguments:
- int pipe_write - file descriptor for writing flag_no_wait parameter to a pipeline - info about __&__ operator  

2) static functions:
- static void next_char() - reading char from stdin
- static void add_command() - adding to rpn when non-operator found
- static void add_outstr(char *str, const El_size size_str) - creating and adding an element to rpn
- static void convert() - start descent
- static void begin() - handle __||__, __&&__, __|__, __&__ - all same lowest priority
- static void redirect() - handle __<__, __>__, __>>__ - higher priority
- static void symbol() - handle commands and __()__ - max priority  

3) return value:
- 0 if correct
- ERR_BRACKET = 1
- ERR_WRONG_OPERAND = 2
- ERR_WRONG_COMMAND_LINE = 3
- ERR_ALLOC = 4
- FOUND_EOF = 5  

##### rpn.h
~~~
typedef struct
{
    void *data;
    size_t size;
} Rpn;
~~~
~~~
void free_rpn(Rpn *rpn);
~~~
Frees all allocated memory for rpn
~~~
int rpn_bracket(size_t begin, size_t end, size_t rpn_size,
    const void *data, Stack_p *head, int start_inp, int start_out);
~~~
Run all elements of rpn in brackets  
1) arguments:  
- size_t begin - brackets start position in rpn
- size_t end - brackets end position in rpn
- size_t rpn_size - size of rpn
- const void *data - string of rpn
- Stack_p *head - address of head pointer
- int start_inp - input file descriptor (may be != 0 for brackets)
- int start_out - output file descriptor (may be != 1 for brackets)  

2) return value:
- 0 if correct
- calculate return value in case of error  

~~~
int calculate_rpn(Rpn *rpn);
~~~
Run all elements of rpn from start till end  
1) return value:
- 0 if correct
- ERR_COMMAND_NOT_FOUND = 1
- ERR_EMPTY_COMMAND = 2
- ERR_FORK = 3
- NOT_RUN = 4  

##### stack.h
1) return value:
- 0 if correct
- 1 in case of error  

~~~
typedef struct Stack_struct {
    char **prog;      // 2-dim array of name & arguments
    int input;        // input file descriptor
    int output;       // ouput file descriptor
    int forbidden;    // result of previous command
    size_t br_begin;  // start of brackets
    size_t br_end;    // end of brackets
    struct Stack_struct *next;
} Stack_struct;
typedef Stack_struct * Stack_p;
~~~
~~~
int init_stack(Stack_p *head);
~~~
Takes address of head  
Initialize *head with zero - needed for realloc
~~~
int push_stack(Stack_p *head, char *prog_line,
        int input, int output, int forbidden, size_t br_begin, size_t br_end);
~~~
Char *prog_line is a string of a command name and all parameters  
It is parsed to a 2-dim array and pushed (head->prog)  
This is done for an easy call of execvp
~~~
int pop_second_stack(Stack_p *head, char ***prog_p, int *input,
        int *output, int *forbidden, size_t *br_begin, size_t *br_end);

int pop_stack(Stack_p *head, char ***prog_p, int *input,
        int *output, int *forbidden, size_t *br_begin, size_t *br_end);
~~~
Pop functions for upper and second after upper elements of stack  
Second element of stack keeps info about first command in rpn  
Pop_stack() and pop_second_stack() do not free memory at char \***prog
~~~
void delete_prog (char ***a);
~~~
Frees allocated memory at head->prog
~~~
void final_stack(Stack_p *head);
~~~
Takes address of head  
Frees all allocated memory

### 4. Problems and upgrades:
The only problem is that construction (p1 p2 .. pn) | prog does not run correctly. Only pn and prog are ran in parallel. The rest are writing to a pipeline and no one is reading from it. This may cause pipeline overflow and deadlock. A solution is to run all commands in brackets in a new process, but I faced several mistaces concerning return result and a desision was made to leave everything as it is due to lack of time.
