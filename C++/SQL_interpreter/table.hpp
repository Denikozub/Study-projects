#ifndef TABLE_HPP
#define TABLE_HPP

extern "C" {
    #include "al_table.h"
}
#include <string>
#include <vector>
#include <set>
#include "iterator.hpp"
using std::string;
using std::vector;
using std::set;

#define CHECK_TABLE(res, func) if(res != 0) \
        throw (string)"error: " + (string)func + " - " + getErrorString(res) + "\n";

class Field;

class LineIterator;

class ColumnIterator;

class Table {
    bool opened = false;
    THandle table;
public:
    string name;
    Table(string, vector<Field>);
    ~Table();
    void delete_table();
    void open();
    void close();
    bool is_empty() const;    
    void create_new();
    void insert();
    string print_headers(const set<string>& = {});
    string print_table();
    friend class LineIterator;
    LineIterator line_begin();
    friend class ColumnIterator;
    ColumnIterator column_begin();
};


class Field {
    FieldDef definition;
public:
    Field(const char * name, FieldType type, unsigned len);
    operator FieldDef() { return definition; };
};


class LineIterator : Iterator {
    Table* table;
public:
    unsigned number;
    LineIterator(Table*);
    void operator++() override;
    bool is_end() override;
    void delete_line();
    template <typename T> T get(string);
    template <typename T> void put(string, T);
    string print_line(const set<string>& = {});
};
// for (auto line = table.line_begin(); !line.is_end(); ++line) {}


class ColumnIterator : Iterator {
    Table* table;
    unsigned curr_field;
    void update();
public:
    FieldType type;
    const char * name;
    ColumnIterator(Table*);
    void operator++() override;
    bool is_end() override;
    template <typename T> T get();
    template <typename T> void put(T value);
    template <typename T> void put_new(T value);
    string print_field();
};
// for (auto col = table.column_begin(); !col.is_end(); ++col) {}

#endif
