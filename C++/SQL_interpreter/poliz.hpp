#ifndef POLIZ_HPP
#define POLIZ_HPP

#include <string>
#include <vector>
#include <stack>
using std::string;
using std::vector;
using std::stack;


class Poliz {
    stack<string> operations;
    vector<string> poliz;
    int get_priority(string);
public:
    void add_var(string);
    void add_word(string);
    void add_comma();
    void add_operation(string);
    void add_open_br();
    void add_close_br();
    void pop();
    vector<string> return_poliz();
};

#endif
