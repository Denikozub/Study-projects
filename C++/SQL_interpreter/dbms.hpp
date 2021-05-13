#ifndef DBMS_HPP
#define DBMS_HPP

#include <map>
#include <string>
#include "table.hpp"
using std::map;
using std::string;

class Dbms {
    static map<string, Table> tables;
public:
    static void create_table(const Table&);
    static Table& get_table(string);
    static void drop_table(string);
    static void drop_all();
};

/*
Dbms::create_table({"Name", { {"field1", Long, 4}, {"field2", Text, 10} }});
Table table = Dbms::get_table("Name");
...
Dbms::drop_table("Name");
*/

#endif
