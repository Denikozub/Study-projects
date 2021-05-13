#ifndef CALCULATE_HPP
#define CALCULATE_HPP

#include "table.hpp"
#include <vector>
#include <set>
#include <stack>
#include <string>
using std::vector;
using std::set;
using std::stack;
using std::string;

set<unsigned> calculate_all(Table& table);

set<unsigned> calculate_like(Table& table, vector<string>::const_iterator begin);

void calculate_in(stack<bool>& bool_ops, const stack<double>& double_ops,
        const stack<string>& string_ops, FieldType last_push, bool reverse,
        vector<string>::const_iterator begin, vector<string>::const_iterator end);

void calculate_not(stack<bool>& bool_ops);

void calculate_andor(stack<bool>& bool_ops, string operation);

void calculate_arithm(stack<double>& double_ops, string operation);

void calculate_comp(stack<bool>& bool_ops, stack<double>& double_ops,
        stack<string>& string_ops, FieldType last_push, string operation);

void push_operand(const set<string>& text_fields, const set<string>& long_fields,
        stack<double>& double_ops, stack<string>& string_ops,
        FieldType& last_push, LineIterator line, string operand);

set<unsigned> calculate_where(Table& table,
        vector<string>::const_iterator begin, vector<string>::const_iterator end,
        const set<string>& text_fields, const set<string>& long_fields);

#endif
