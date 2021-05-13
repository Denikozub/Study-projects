#ifndef PARSER_HPP
#define PARSER_HPP

#include "lex_analyzer.hpp"
#include "poliz.hpp"
#include <set>
#include <stack>
using std::set;
using std::stack;

enum OpType {
    Boolean,
    Int,
    Str
};

class Parser {
    LexAnalyzer scanner;
    Poliz poliz;
    set<string> text_fields, long_fields;
    bool is_text_field();
    bool is_long_field();
    string curr_lex;
    Type lex_type;
    stack<OpType> op_types;
    void get_lex();

    void REQUEST();
    void SELECT();
    void INSERT();
    void UPDATE();
    void DELETE();
    void CREATE();
    void DROP();
    void FIELDS();
    void VALUES();
    void DESCR_LIST();
    void FIELD_DESCR();
    void CLAUSE();
    void WHEREEXPR();
    void LOGINLIKE();
    void LOGIN();
    void LOGTREL();
    void NOTINLIKE();
    void INLIKE();
    void CONST();
    void LEXPR();
    void LS();
    void LM();
    void LVAL();
    void TEXPR();
    void LOGEXPR();
    void LOGS();
    void LOGM();
    void REL();
    void TREL();
    void LREL();
    void CMP();
public:
    Parser(string, const set<string>& = {}, const set<string>& = {});
    vector<string> parse();
};

#endif
