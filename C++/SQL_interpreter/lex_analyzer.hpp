#ifndef LEX_ANALYZER_HPP
#define LEX_ANALYZER_HPP

#include <string>
#include <sstream>
using std::string;
using std::stringstream;
using std::pair;

enum Type {
    String,
    Ident,
    LongInt,
    Reserved,
    Empty
};

class LexAnalyzer {
    string curr_word;
    stringstream sstr = {};
    bool is_ident(string);
    bool is_string(string);
    string unquote(string);
    bool is_long(string);
public:
    LexAnalyzer(string);
    pair<string, Type> get_lex();
};

#endif
