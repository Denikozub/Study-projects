#include "poliz.hpp"

void Poliz::add_var(string lex) {
    poliz.push_back(lex);
}

void Poliz::add_word(string lex) {
    poliz.push_back(lex); // or operations.push(lex);
}

void Poliz::add_comma() {
    bool found_br = false;
    while (!operations.empty()) {
        string elem = operations.top();
        if (elem == "(") {
            found_br = true;
            break;
        }
        operations.pop();
        poliz.push_back(elem);
    }
    if (!found_br)
        throw (string)"syntax error: wrong delimeter or bracket mismatch\n";
}

int Poliz::get_priority(string lex) {
    if (lex == "OR")
        return 1;
    if (lex == "AND")
        return 2;
    if (lex == "NOT")
        return 3;
    if (lex == "=" or lex == "<" or lex == ">" or
        lex == "<=" or lex == ">=" or lex == "!=")
        return 4;
    if (lex == "+" or lex == "-")
        return 5;
    if (lex == "*" or lex == "/" or lex == "%")
        return 6;
    return 0;
}

void Poliz::add_operation(string lex) {
    while (!operations.empty() && get_priority(lex) <= get_priority(operations.top())) {
        string elem = operations.top();
        operations.pop();
        poliz.push_back(elem);
    }
    operations.push(lex);
}

void Poliz::add_open_br() {
    operations.push("(");
}

void Poliz::add_close_br() {
    bool found_br = false;
    while (!operations.empty()) {
        string elem = operations.top();
        operations.pop();
        if (elem == "(") {
            found_br = true;
            break;
        }
        poliz.push_back(elem);
    }
    if (!found_br)
        throw (string)"syntax error: wrong delimeter or bracket mismatch\n";
    // or check function
}

void Poliz::pop() {
    while (!operations.empty()) {
        string elem = operations.top();
        operations.pop();
        if (get_priority(elem) == 0)
            throw (string)"syntax error: bracket mismatch\n";
        poliz.push_back(elem);
    }
}

vector<string> Poliz::return_poliz() {
    pop();
    return poliz;
}

