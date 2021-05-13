#include "parser.hpp"

bool Parser::is_text_field() {
   return text_fields.find(curr_lex) != text_fields.end();
}

bool Parser::is_long_field() {
    return long_fields.find(curr_lex) != long_fields.end();
}

Parser::Parser(string str, const set<string>& set_text, const set<string>& set_long) :
        scanner(str), text_fields(set_text), long_fields(set_long) {}

void Parser::get_lex() {
    auto lexem = scanner.get_lex();
    curr_lex = lexem.first;
    lex_type = lexem.second;
}

vector<string> Parser::parse() {
    get_lex();
    REQUEST();
    if (lex_type != Empty)
        throw (string)"request: syntax error\n";
    return poliz.return_poliz();
}

void Parser::REQUEST() {
    if (curr_lex == "SELECT") {
        poliz.add_word(curr_lex);
        get_lex();
        SELECT();
    }
    else if (curr_lex == "INSERT") {
        poliz.add_word(curr_lex);
        get_lex();
        INSERT();
    }
    else if (curr_lex == "UPDATE") {
        poliz.add_word(curr_lex);
        get_lex();
        UPDATE();
    }
    else if (curr_lex == "DELETE") {
        poliz.add_word(curr_lex);
        get_lex();
        DELETE();
    }
    else if (curr_lex == "CREATE") {
        poliz.add_word(curr_lex);
        get_lex();
        CREATE();
    }
    else if (curr_lex == "DROP") {
        poliz.add_word(curr_lex);
        get_lex();
        DROP();
    }
    else throw (string)"request: syntax error\n";
}

void Parser::SELECT() {
    FIELDS();
    if (curr_lex != "FROM")
        throw (string)"select: syntax error - missing FROM\n";
    poliz.add_word(curr_lex);
    get_lex();
    if (lex_type != Ident)
        throw (string)"select: syntax error - wrong table name\n";
    poliz.add_var(curr_lex);
    get_lex();
    CLAUSE();
}

void Parser::INSERT() {
    if (curr_lex != "INTO")
        throw (string)"insert: syntax error - missing INTO\n";
    get_lex();
    if (lex_type != Ident)
        throw (string)"insert: syntax error - wrong table name\n";
    poliz.add_var(curr_lex);
    get_lex();
    if (curr_lex != "(")
        throw (string)"insert: syntax error - missing (\n";
    get_lex();
    VALUES();
    if (curr_lex != ")")
        throw (string)"insert: syntax error - missing )\n";
    get_lex();
}

void Parser::UPDATE() {
    if (lex_type != Ident)
        throw (string)"update: syntax error - wrong table name\n";
    poliz.add_var(curr_lex);
    get_lex();
    if (curr_lex != "SET")
        throw (string)"update: syntax error - missing SET\n";
    get_lex();
    if (lex_type != Ident)
        throw (string)"update: syntax error - wrong field name\n";
    poliz.add_var(curr_lex);
    get_lex();
    if (curr_lex != "=")
        throw (string)"update: syntax error - missing =\n";
    get_lex();
    if ((lex_type == Ident && is_text_field()) || lex_type == String) {
        poliz.add_var(curr_lex);
        get_lex();
    } else {
        LEXPR();
        poliz.pop();
    }
    CLAUSE();
}

void Parser::DELETE() {
    if (curr_lex != "FROM")
        throw (string)"delete: syntax error - missing FROM\n";
    get_lex();
    if (lex_type != Ident)
        throw (string)"delete: syntax error - wrong table name\n";
    poliz.add_var(curr_lex);
    get_lex();
    CLAUSE();
}

void Parser::CREATE() {
    if (curr_lex != "TABLE")
        throw (string)"create: syntax error - missing TABLE\n";
    get_lex();
    if (lex_type != Ident)
        throw (string)"create: syntax error - wrong table name\n";
    poliz.add_var(curr_lex);
    get_lex();
    if (curr_lex != "(")
        throw (string)"create: syntax error - missing (\n";
    get_lex();
    DESCR_LIST();
    if (curr_lex != ")")
        throw (string)"create: syntax error - missing )\n";
    get_lex();
}

void Parser::DROP() {
    if (curr_lex != "TABLE")
        throw (string)"drop: syntax error - missing TABLE\n";
    get_lex();
    if (lex_type != Ident)
        throw (string)"drop: syntax error - wrong table name\n";
    poliz.add_var(curr_lex);
    get_lex();
}

void Parser::FIELDS() {
    if (curr_lex == "*") {
        poliz.add_word(curr_lex);
        get_lex();
        return;
    }
    if (lex_type != Ident)
        throw (string)"select: syntax error - wrong field name\n";
    poliz.add_var(curr_lex);
    get_lex();
    while (curr_lex == ",") {
        get_lex();
        if (lex_type != Ident)
            throw (string)"select: syntax error - wrong field name\n";
        poliz.add_var(curr_lex);
        get_lex();
    }
}

void Parser::VALUES() {
    if (lex_type != String && lex_type != LongInt)
        throw (string)"insert: syntax error - wrong field value\n";
    poliz.add_var(curr_lex);
    get_lex();
    while (curr_lex == ",") {
        get_lex();
        if (lex_type != String && lex_type != LongInt)
            throw (string)"insert: syntax error - wrong field value\n";
        poliz.add_var(curr_lex);
        get_lex();
    }
}

void Parser::DESCR_LIST() {
    FIELD_DESCR();
    while (curr_lex == ",") {
        get_lex();
        FIELD_DESCR();
    }
}

void Parser::FIELD_DESCR() {
    if (lex_type != Ident)
        throw (string)"create: syntax error - wrong field name\n";
    poliz.add_var(curr_lex);
    get_lex();
    if (curr_lex == "TEXT") {
        poliz.add_var(curr_lex);
        get_lex();
        if (curr_lex != "(")
            throw (string)"create: syntax error - missing (\n";
        get_lex();
        if (lex_type != LongInt && stol(curr_lex) <= 0)
            throw (string)"create: syntax error - wrong TEXT length\n";
        poliz.add_var(curr_lex);
        get_lex();
        if (curr_lex != ")")
            throw (string)"create: syntax error - missing )\n";
        get_lex();
    } else if (curr_lex == "LONG") {
        poliz.add_var(curr_lex);
        poliz.add_var("4");
        get_lex();
    } else throw (string)"create: syntax error - wrong field type\n";
}

void Parser::CLAUSE() {
    if (curr_lex != "WHERE")
        throw (string)"where-clause: syntax error - missing WHERE\n";
    poliz.add_word(curr_lex);
    get_lex();
    WHEREEXPR();
}

void Parser::WHEREEXPR() {
    if (lex_type == Ident && is_text_field()) {
        op_types.push(Str);
        poliz.add_var(curr_lex);
        get_lex();
        LOGINLIKE();
    } else if (lex_type == String) {
        op_types.push(Str);
        poliz.add_var(curr_lex);
        get_lex();
        LOGIN();
    } else if (curr_lex == "ALL") {
        poliz.add_word(curr_lex);
        get_lex();
    } else {
        LOGEXPR();
        if (!op_types.empty() && op_types.top() == Boolean)
            return;
        if (curr_lex == "NOT") {
            poliz.add_word(curr_lex);
            get_lex();
            if (curr_lex != "IN")
                throw (string)"where-clause: syntax error - missing IN\n";
        }
        if (curr_lex == "IN") {
            poliz.add_word(curr_lex);
            get_lex();
            if (curr_lex != "(")
                throw (string)"where-clause: syntax error - missing (\n";
            get_lex();
            CONST();
            if (curr_lex != ")")
                throw (string)"where-clause: syntax error - missing )\n";
            get_lex();
        }
    }
}

void Parser::LOGINLIKE() {
    if (curr_lex == "NOT" || curr_lex == "IN" || curr_lex == "LIKE")
        NOTINLIKE();
    else LOGTREL();
}

void Parser::LOGIN() {
    if (curr_lex == "NOT" || curr_lex == "IN") {
        if (curr_lex == "NOT") {
            poliz.add_word(curr_lex);
            get_lex();
        }
        if (curr_lex != "IN")
            throw (string)"where-clause: syntax error - missing IN\n";
        poliz.add_word(curr_lex);
        get_lex();
        if (curr_lex != "(")
            throw (string)"where-clause: syntax error - missing (\n";
        get_lex();
        CONST();
        if (curr_lex != ")")
            throw (string)"where-clause: syntax error - missing )\n";
        get_lex();
    } else LOGTREL();
}

void Parser::LOGTREL() {
    CMP();
    TEXPR();
    OpType first_op = op_types.top();
    op_types.pop();
    OpType second_op = op_types.top();
    op_types.pop();
    if (first_op != second_op || first_op == Boolean)
        throw (string)"compare: error - incompatible types\n";
    op_types.push(Boolean);
    LOGEXPR();
}

void Parser::NOTINLIKE() {
    if (curr_lex == "NOT") {
        poliz.add_word(curr_lex);
        get_lex();
    }
    INLIKE();
}

void Parser::INLIKE() {
    if (curr_lex == "LIKE") {
        poliz.add_word(curr_lex);
        get_lex();
        if (lex_type != String)
            throw (string)"where-clause: syntax error - wrong LIKE string\n";
        poliz.add_var(curr_lex);
        get_lex();
    } else if (curr_lex == "IN") {
        poliz.add_word(curr_lex);
        get_lex();
        if (curr_lex != "(")
            throw (string)"where-clause: syntax error - missing (\n";
        get_lex();
        CONST();
        if (curr_lex != ")")
            throw (string)"where-clause: syntax error - missing )\n";
        get_lex();
    } else throw (string)"where-clause: syntax error - wrong expression\n";
}

void Parser::CONST() {
    if (lex_type == String) {
        poliz.add_var(curr_lex);
        get_lex();
        while (curr_lex == ",") {
            get_lex();
            if (lex_type != String)
                throw (string)"where-clause: syntax error - wrong const list\n";
            poliz.add_var(curr_lex);
            get_lex();
        }
    } else if (lex_type == LongInt) {
        poliz.add_var(curr_lex);
        get_lex();
        while (curr_lex == ",") {
            get_lex();
            if (lex_type != LongInt)
                throw (string)"where-clause: syntax error - wrong const list\n";
            poliz.add_var(curr_lex);
            get_lex();
        }
    } else throw (string)"where-clause: syntax error - wrong const list\n";
}

void Parser::LEXPR() {
    LS();
    while (curr_lex == "+" || curr_lex == "-") {
        poliz.add_operation(curr_lex);
        get_lex();
        LS();
        OpType first_op = op_types.top();
        op_types.pop();
        OpType second_op = op_types.top();
        op_types.pop();
        if (first_op != Int || second_op != Int)
            throw (string)"sum: error - incompatible types\n";
        op_types.push(Int);
    }
}

void Parser::LS() {
    LM();
    while (curr_lex == "*" || curr_lex == "/" || curr_lex == "%") {
        poliz.add_operation(curr_lex);
        get_lex();
        LM();
        OpType first_op = op_types.top();
        op_types.pop();
        OpType second_op = op_types.top();
        op_types.pop();
        if (first_op != Int || second_op != Int)
            throw (string)"mult: error - incompatible types\n";
        op_types.push(Int);
    }
}

void Parser::LM() {
    if (curr_lex == "(") {
        poliz.add_open_br();
        get_lex();
        LOGEXPR();
        if (curr_lex != ")")
            throw (string)"syntax error: bracket mismatch\n";
        poliz.add_close_br();
        get_lex();
    } else LVAL();
}

void Parser::LVAL() {
    if ((lex_type == Ident && is_long_field()) || lex_type == LongInt) {
        op_types.push(Int);
        poliz.add_var(curr_lex);
        get_lex();
    } else throw (string)"where-clause: syntax error - wrong long value\n";
}

void Parser::TEXPR() {
    if ((lex_type == Ident && is_text_field()) || lex_type == String) {
        op_types.push(Str);
        poliz.add_var(curr_lex);
        get_lex();
    } else throw (string)"where-clause: syntax error - wrong text value\n";
}

void Parser::LOGEXPR() {
    if (curr_lex.empty()) {
        if (op_types.empty() || op_types.top() != Boolean)
            throw (string)"where-clause: syntax error - missing clause\n";
        return;
    }
    LOGS();
    while (curr_lex == "OR") {
        poliz.add_operation(curr_lex);
        get_lex();
        LOGS();
        OpType first_op = op_types.top();
        op_types.pop();
        OpType second_op = op_types.top();
        op_types.pop();
        if (first_op != Boolean || second_op != Boolean)
            throw (string)"OR: error - incompatible types\n";
        op_types.push(Boolean);
    }
}

void Parser::LOGS() {
    LOGM();
    while (curr_lex == "AND") {
        poliz.add_operation(curr_lex);
        get_lex();
        LOGM();
        OpType first_op = op_types.top();
        op_types.pop();
        OpType second_op = op_types.top();
        op_types.pop();
        if (first_op != Boolean || second_op != Boolean)
            throw (string)"AND: error - incompatible types\n";
        op_types.push(Boolean);
    }
}

void Parser::LOGM() {
    if (curr_lex == "NOT") {
        poliz.add_operation(curr_lex);
        get_lex();
        LOGM();
        if (op_types.empty() || op_types.top() != Boolean)
            throw (string)"NOT: error - incompatible types\n";
    } else REL();
}

void Parser::REL() {
    if ((lex_type == Ident && is_text_field()) || lex_type == String) {
        TREL();
    } else LREL();
}

void Parser::TREL() {
    TEXPR();
    CMP();
    TEXPR();
    OpType first_op = op_types.top();
    op_types.pop();
    OpType second_op = op_types.top();
    op_types.pop();
    if (first_op != second_op || first_op == Boolean)
        throw (string)"compare: error - incompatible types\n";
    op_types.push(Boolean);
}

void Parser::LREL() {
    LEXPR();
    if (curr_lex == "=" or curr_lex == "<" or curr_lex == ">" or
        curr_lex == "<=" or curr_lex == ">=" or curr_lex == "!=") {
        CMP();
        LEXPR();
        OpType first_op = op_types.top();
        op_types.pop();
        OpType second_op = op_types.top();
        op_types.pop();
        if (first_op != second_op || first_op == Boolean)
            throw (string)"compare: error - incompatible types\n";
        op_types.push(Boolean);
    }
}

void Parser::CMP() {
    if (curr_lex == "=" or curr_lex == "<" or curr_lex == ">" or
        curr_lex == "<=" or curr_lex == ">=" or curr_lex == "!=") {
        poliz.add_operation(curr_lex);
        get_lex();
    } else throw (string)"where-clause: syntax error - wrong comparison\n";
}

