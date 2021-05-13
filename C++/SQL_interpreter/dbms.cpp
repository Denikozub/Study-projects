#include "dbms.hpp"
#include <unistd.h>

map<string, Table> Dbms::tables = {};

void Dbms::create_table(const Table& tbl) {
    tables.insert({tbl.name, tbl});
}

Table& Dbms::get_table(string name) {
    return tables.at(name);
}

void Dbms::drop_table(string name) {
    tables.erase(name);
    unlink(name.c_str());
}

void Dbms::drop_all() {
    for (auto i : tables)
        unlink(i.first.c_str());
}

