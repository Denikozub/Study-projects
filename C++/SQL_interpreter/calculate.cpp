#include "calculate.hpp"
#include <regex>

const double epsilon = 0.0000000001;

set<unsigned> calculate_all(Table& table) {
    set<unsigned> good_lines;
    for (auto line = table.line_begin(); !line.is_end(); ++line)
        good_lines.insert(line.number);
    return good_lines;
}

set<unsigned> calculate_like(Table& table, vector<string>::const_iterator begin) {
    set<unsigned> good_lines;
    string example = *(begin + 1) == "LIKE" ? *(begin + 2) : *(begin + 3);
    example = std::regex_replace(example, std::regex("%"), ".*");
    example = std::regex_replace(example, std::regex("_"), ".");
    const std::regex r(example);
    for (auto line = table.line_begin(); !line.is_end(); ++line) {
        string value = line.get<string>(*begin);
        if (*(begin + 1) == "LIKE" && std::regex_match(value, r))
            good_lines.insert(line.number);
        if (*(begin + 1) == "NOT" && !std::regex_match(value, r))
            good_lines.insert(line.number);
    }
    return good_lines;
}

void calculate_in(stack<bool>& bool_ops, const stack<double>& double_ops,
        const stack<string>& string_ops, FieldType last_push, bool reverse,
        vector<string>::const_iterator begin, vector<string>::const_iterator end) {
    for (auto it = begin; it != end; ++it) {
        if (last_push == Long && abs(double_ops.top() - stol(*it)) < epsilon) {
            bool_ops.push(!reverse);
            return;
        }
        if (last_push == Text && string_ops.top() == *it) {
            bool_ops.push(!reverse);
            return;
        }
    }
    bool_ops.push(reverse);
}

void calculate_not(stack<bool>& bool_ops) {
    bool op = bool_ops.top();
    bool_ops.pop();
    bool_ops.push(!op);
}

void calculate_andor(stack<bool>& bool_ops, string operation) {
    bool op1 = bool_ops.top();
    bool_ops.pop();
    bool op2 = bool_ops.top();
    bool_ops.pop();
    if (operation == "AND") bool_ops.push(op2 && op1);
    if (operation == "OR") bool_ops.push(op2 || op1);
}

void calculate_arithm(stack<double>& double_ops, string operation) {
    double op1 = double_ops.top();
    double_ops.pop();
    double op2 = double_ops.top();
    double_ops.pop();
    if (operation == "+") double_ops.push(op2 + op1);
    if (operation == "-") double_ops.push(op2 - op1);
    if (operation == "*") double_ops.push(op2 * op1);
    if (operation == "%") double_ops.push((double)((long)op2 % (long)op1));
    if (operation == "/") {
        if (abs(op1) < epsilon) throw (string)"division by zero\n";
        double_ops.push(op2 / op1);
    }
}

void calculate_comp(stack<bool>& bool_ops, stack<double>& double_ops,
        stack<string>& string_ops, FieldType last_push, string operation) {
    if (last_push == Text) {
        string op1 = string_ops.top();
        string_ops.pop();
        string op2 = string_ops.top();
        string_ops.pop();
        if (operation == ">") bool_ops.push(op2 > op1);
        if (operation == "<") bool_ops.push(op2 < op1);
        if (operation == "=") bool_ops.push(op2 == op1);
        if (operation == "!=") bool_ops.push(op2 != op1);
        if (operation == ">=") bool_ops.push(op2 >= op1);
        if (operation == "<=") bool_ops.push(op2 <= op1);
    } else {
        double op1 = double_ops.top();
        double_ops.pop();
        double op2 = double_ops.top();
        double_ops.pop();
        if (operation == ">") bool_ops.push(op2 > op1);
        if (operation == "<") bool_ops.push(op2 < op1);
        if (operation == "=") bool_ops.push(abs(op2 - op1) < epsilon);
        if (operation == "!=") bool_ops.push(abs(op2 - op1) > epsilon);
        if (operation == ">=") bool_ops.push(op2 >= op1);
        if (operation == "<=") bool_ops.push(op2 <= op1);
    }
}

void push_operand(const set<string>& text_fields, const set<string>& long_fields,
        stack<double>& double_ops, stack<string>& string_ops,
        FieldType& last_push, LineIterator line, string operand) {
    if (long_fields.find(operand) != long_fields.end()) {
        double_ops.push((double)line.get<long>(operand));
        last_push = Long;
    } else if (text_fields.find(operand) != text_fields.end()) {
        string_ops.push(line.get<string>(operand));
        last_push = Text;
    } else try {
        double_ops.push((double)stol(operand));
        last_push = Long;
    } catch(...) {
        string_ops.push(operand);
        last_push = Text;
    }
}

set<unsigned> calculate_where(Table& table,
        vector<string>::const_iterator begin, vector<string>::const_iterator end,
        const set<string>& text_fields, const set<string>& long_fields) {
        
    if (*begin == "ALL") {
        return calculate_all(table);
    }
    
    if (*(begin + 1) == "LIKE" || *(begin + 2) == "LIKE") {
        return calculate_like(table, begin);
    }
    
    set<unsigned> good_lines;
    set<string> operations = { "+", "-", "*", "/", "%", "AND", "OR",
            "NOT", "<", ">", "=", "!=", "<=", ">=", "IN" };
    
    for (auto line = table.line_begin(); !line.is_end(); ++line) {
    
        stack<bool> bool_ops;
        stack<double> double_ops;
        stack<string> string_ops;
        FieldType last_push = Text;
        
        for (auto it = begin; it != end; ++it) {
        
            // if current lexem is an operation
            if (operations.find(*it) != operations.end()) {
            
                // IN expression
                if (*it == "IN") {
                    calculate_in(bool_ops, double_ops, string_ops,
                            last_push, false, it + 1, end);
                    break;
                }
                
                // NOT expression
                else if (*it == "NOT") {
                
                    // NOT IN expression
                    if (*(it + 1) == "IN") {
                        calculate_in(bool_ops, double_ops, string_ops,
                                last_push, true, it + 2, end);
                        break;
                    }
                    
                    // NOT operation
                    else calculate_not(bool_ops);
                }
                
                // AND OR expression
                else if (*it == "AND" || *it == "OR") calculate_andor(bool_ops, *it);
                
                // arithmetic operation
                else if (*it == "+" || *it == "-" ||
                        *it == "*" || *it == "/" || *it == "%")
                    calculate_arithm(double_ops, *it);
                
                // comparison
                else calculate_comp(bool_ops, double_ops, string_ops, last_push, *it);
            }
            
            // if current lexem is an operand
            else push_operand(text_fields, long_fields, double_ops,
                    string_ops, last_push, line, *it);
        }
        
        // add corresponding line
        if (bool_ops.top() == true) good_lines.insert(line.number);
    }
    return good_lines;
}
