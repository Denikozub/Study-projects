#include "table.hpp"
#include <cstring>

/************************************* Field **********************************/

Field::Field(const char * name, FieldType type, unsigned len) {
        definition.name = name;
        definition.type = type;
        definition.len = len;
    }

/********************************* LineIterator *******************************/

LineIterator::LineIterator(Table* src) : table(src), number(0) {}

void LineIterator::operator++() {
    CHECK_TABLE(moveNext(table->table), "operator++");
    ++number;
}

bool LineIterator::is_end() {
    return afterLast(table->table);
}

void LineIterator::delete_line() {
    CHECK_TABLE(deleteRec(table->table), "delete_line");
}

template <> long LineIterator::get(string name) {
    long lvalue;
    CHECK_TABLE(getLong(table->table, name.c_str(), &lvalue), "get");
    return lvalue;
}

template <> string LineIterator::get(string name) {
    char *svalue;
    CHECK_TABLE(getText(table->table, name.c_str(), &svalue), "get");
    return (string)svalue;
}

template <typename T> T LineIterator::get(string) {
    throw (string)"error: get - wrong type\n";
}

template <> void LineIterator::put(string name, long value) {
    CHECK_TABLE(startEdit(table->table), "startEdit");
    CHECK_TABLE(putLong(table->table, name.c_str(), value), "put");
    CHECK_TABLE(finishEdit(table->table), "finishEdit");
}

template <> void LineIterator::put(string name, string value) {
    CHECK_TABLE(startEdit(table->table), "startEdit");
    CHECK_TABLE(putText(table->table, name.c_str(), value.c_str()), "put");
    CHECK_TABLE(finishEdit(table->table), "finishEdit");
}

template <typename T> void LineIterator::put(string name, T value) {
    throw (string)"error: put - wrong type\n";
}

string LineIterator::print_line(const set<string>& columns) {
    string line = "";
    for (auto col = table->column_begin(); !col.is_end(); ++col)
        if (columns.empty() || columns.find(col.name) != columns.end())
            line += col.print_field() + "\t";
    line += "\n";
    return line;
}

/******************************** ColumnIterator ******************************/

ColumnIterator::ColumnIterator(Table* src) : table(src), curr_field(0) {
    update();
}

void ColumnIterator::update() {
    char *field_name;
    CHECK_TABLE(getFieldName(table->table, curr_field, &field_name), "ColumnIterator");
    name = field_name;
    FieldType field_type;
    CHECK_TABLE(getFieldType(table->table, name, &field_type), "ColumnIterator");
    type = field_type;
}

void ColumnIterator::operator++() {
    ++curr_field;
    if (is_end()) return;
    update();
}

bool ColumnIterator::is_end() {
    unsigned num;
    CHECK_TABLE(getFieldsNum(table->table, &num), "is_end");
    return curr_field >= num;
}

template <> long ColumnIterator::get() {
    long lvalue;
    CHECK_TABLE(getLong(table->table, name, &lvalue), "get");
    return lvalue;
}

template <> string ColumnIterator::get() {
    char *svalue;
    CHECK_TABLE(getText(table->table, name, &svalue), "get");
    return (string)svalue;
}

template <typename T> T ColumnIterator::get() {
    throw (string)"error: get - wrong type\n";
}

template <> void ColumnIterator::put(long value) {
    CHECK_TABLE(startEdit(table->table), "startEdit");
    CHECK_TABLE(putLong(table->table, name, value), "put");
    CHECK_TABLE(finishEdit(table->table), "finishEdit");
}

template <> void ColumnIterator::put(string value) {
    CHECK_TABLE(startEdit(table->table), "startEdit");
    CHECK_TABLE(putText(table->table, name, value.c_str()), "put");
    CHECK_TABLE(finishEdit(table->table), "finishEdit");
}

template <typename T> void ColumnIterator::put(T value) {
    throw (string)"error: put - wrong type\n";
}

template <> void ColumnIterator::put_new(long value) {
    CHECK_TABLE(putLongNew(table->table, name, value), "put_new");
}

template <> void ColumnIterator::put_new(string value) {
    CHECK_TABLE(putTextNew(table->table, name, value.c_str()), "put_new");
}

template <typename T> void ColumnIterator::put_new(T value) {
    throw (string)"error: put_new - wrong type\n";
}

string ColumnIterator::print_field() {
    if (type == Text)
        return get<string>();
    return std::to_string(get<long>());
}

/************************************* Table **********************************/

Table::Table(string set_name, vector<Field> set_fields) : name(set_name) {
    unsigned fields_number = set_fields.size();
    FieldDef *fields = (FieldDef*)malloc(fields_number * sizeof(*fields));
    for (unsigned i = 0; i < fields_number; ++i)
        fields[i] = set_fields.at(i);
    TableStruct table_struct;
    table_struct.numOfFields = fields_number;
    table_struct.fieldsDef = fields;
    CHECK_TABLE(createTable(name.c_str(), &table_struct), "createTable");
    free(table_struct.fieldsDef);
}

Table::~Table() {
    if (opened)
        close();
}

void Table::delete_table() {
    CHECK_TABLE(deleteTable(name.c_str()), "delete_table");
}

void Table::open() {
    CHECK_TABLE(openTable(name.c_str(), &table), "open_table");
    opened = true;
}

void Table::close() {
    CHECK_TABLE(closeTable(table), "close_table");
    opened = false;
}

LineIterator Table::line_begin() {
    CHECK_TABLE(moveFirst(table), "begin");
    return LineIterator(this);
}

ColumnIterator Table::column_begin() {
    return ColumnIterator(this);
}

bool Table::is_empty() const {
    return beforeFirst(table);
}

void Table::create_new() {
    CHECK_TABLE(createNew(table), "create_new");
}

void Table::insert() {
    CHECK_TABLE(insertzNew(table), "insert");
}

string Table::print_headers(const set<string>& columns) {
    string headers = "";
    for (auto col = column_begin(); !col.is_end(); ++col)
        if (columns.empty() || columns.find((string)col.name) != columns.end())
            headers += (string)col.name + "\t";
    headers += "\n";
    return headers;
}

string Table::print_table() {
    string tbl = print_headers();
    for (auto line = line_begin(); !line.is_end(); ++line)
        tbl += line.print_line();
    return tbl;
}

