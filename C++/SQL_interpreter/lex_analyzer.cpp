#include "lex_analyzer.hpp"
using std::min;


bool LexAnalyzer::is_ident(string lex) {
    if (lex.empty())
        return false;
    if (!std::isalpha(lex[0]) and lex[0] != '_')
        return false;
    for (auto i : lex)
        if (!std::isalpha(i) && !std::isdigit(i) && i != '_')
            return false;
    return true;
}

bool LexAnalyzer::is_string(string lex) {
    return lex[0] == '\'' && lex[lex.length() - 1] == '\'';
}

string LexAnalyzer::unquote(string lex) {
    return lex.substr(1, lex.length() - 2);
}

bool LexAnalyzer::is_long(string lex) {
    try { stol(lex); return true; }
    catch (...) { return false; }
}

LexAnalyzer::LexAnalyzer(string str) {
    sstr << str;
    sstr >> curr_word;
}

pair<string, Type> LexAnalyzer::get_lex() {
    if (curr_word.empty())
        sstr >> curr_word;
    if (curr_word.empty())
        return { curr_word, Empty };
    size_t comma = curr_word.find(",");
    size_t open_br = curr_word.find("(");
    size_t close_br = curr_word.find(")");
    size_t closest = min(comma, min(open_br, close_br));
    string answer;
    if (closest == 0) {
        answer = curr_word.substr(0, 1);
        curr_word.erase(0, 1);
        return { answer, Reserved };
    }
    if (closest == curr_word.npos) {
        answer = curr_word;
        curr_word.clear();
    } else {
        answer = curr_word.substr(0, closest);
        curr_word.erase(0, closest);
    }
    if (is_long(answer))
        return { answer, LongInt };
    if (is_ident(answer))
        return { answer, Ident };
    if (is_string(answer))
        return { unquote(answer), String };
    return { answer, Reserved };
}

