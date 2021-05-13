#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "server.hpp"
#include "dbms.hpp"
#include "parser.hpp"
#include "calculate.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <stack>
using std::vector;
using std::set;
using std::stack;


Server::Server(string address_name) {
    remove(address_name.c_str());
    CHECK_SOCKET((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)));
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, address_name.c_str());
}

void Server::connect() {
    CHECK_SOCKET(bind(sock_fd, (struct sockaddr *)&sa, sizeof(sa)));
    CHECK_SOCKET(listen(sock_fd, 1));
    CHECK_SOCKET((connect_fd = accept(sock_fd, NULL, NULL)));
}

void Server::send(string word) {
    CHECK_SOCKET(write(connect_fd, word.c_str(), word.length()));
}

string Server::receive() {
    char buf[MAX_LEN] = {0};
    CHECK_SOCKET(read(connect_fd, buf, MAX_LEN));
    return (string)buf;
}

Server::~Server() {
    shutdown(connect_fd, 2);
    close(connect_fd);
}

int main ()
{   
    Server s("My_socket");
    try {
        s.connect();
    } catch (string message) {
        std::cerr << message << std::endl;
    }
    
    try {
        string request;
        set<string> text_fields, long_fields;
        stack<string> all_fields;
        
        while (1) {
            
            // get SQL request
            request = s.receive();
            if (request == "stop") {
                s.send("stopped\n");
                break;
            }
            
            // initialize parser
            Parser parser(request, text_fields, long_fields);
            auto result = parser.parse();
            
            /******************************* CREATE ***************************/
            if (result[0] == "CREATE") {
            
                // initialize fields
                vector<Field> fields;
                for (auto it = result.cbegin() + 2; it < result.cend() - 2; it += 3) {
                    all_fields.push(*it);
                    
                    // TEXT field
                    if (*(it + 1) == "TEXT") {
                        text_fields.insert(*it);
                        fields.push_back(Field(all_fields.top().c_str(),
                                Text, stol(*(it + 2))));
                    }
                    
                    // LONG field
                    else {
                        long_fields.insert(*it);
                        fields.push_back(Field(all_fields.top().c_str(), Long,
                                stol(*(it + 2))));
                    }
                }
                
                // create and print table
                Dbms::create_table({result[1], fields});
                Table table = Dbms::get_table(result[1]);
                table.open();
                s.send(table.print_table());
                table.close();
            }
            
            /******************************** DROP ****************************/
            else if (result[0] == "DROP") {
                Dbms::drop_table(result[1]);
                s.send("deleted\n");
            }
            
            /******************************* INSERT ***************************/
            else if (result[0] == "INSERT") {
                Table table = Dbms::get_table(result[1]);
                table.open();
                table.create_new();
                
                // insert values
                unsigned i = 2;
                for (auto col = table.column_begin(); !col.is_end(); ++col) {
                    if (col.type == Long)
                        col.put_new(stol(result[i++]));
                    else col.put_new(result[i++]);
                }
                table.insert();
                
                // print table
                s.send(table.print_table());
                table.close();
            }
            
            /******************************* SELECT ***************************/
            else if (result[0] == "SELECT") {
                set<string> select_fields;
                string name;
                vector<string>::const_iterator where_start;
                
                // SELECT *
                if (result[1] == "*") {
                    name = result[3];
                    where_start = result.cbegin() + 5;
                }
                
                // find WHERE expression
                else for (auto i = result.cbegin() + 1; i != result.cend(); ++i) {
                    if (*i == "FROM") {
                        name = *(i + 1);
                        where_start = i + 3;
                        break;
                    }
                    select_fields.insert(*i);
                }
                
                Table table = Dbms::get_table(name);
                table.open();
                
                // find lines which fit WHERE expression
                auto line_indexes = calculate_where(table, where_start,
                        result.cend(), text_fields, long_fields);
                
                // print selected fields and lines
                string tbl = table.print_headers(select_fields);
                for (auto line = table.line_begin(); !line.is_end(); ++line) {
                    if (line_indexes.find(line.number) != line_indexes.end())
                        tbl += line.print_line(select_fields);
                }
                s.send(tbl);
                table.close();
            }
            
            /******************************* DELETE ***************************/
            else if (result[0] == "DELETE") {
                Table table = Dbms::get_table(result[1]);
                table.open();
                
                // find lines which fit WHERE expression
                auto line_indexes = calculate_where(table, result.cbegin() + 3,
                        result.cend(), text_fields, long_fields);
                
                // delete selected lines
                for (auto line = table.line_begin(); !line.is_end(); ++line) {
                    if (line_indexes.find(line.number) != line_indexes.end())
                        line.delete_line();
                }
                
                // print table
                s.send(table.print_table());
                table.close();
            }
            
            /******************************* UPDATE ***************************/
            else if (result[0] == "UPDATE") {
                Table table = Dbms::get_table(result[1]);
                table.open();
                
                // find WHERE expression
                vector<string>::iterator start_where =
                        std::find(result.begin(), result.end(), "WHERE") + 1;
                
                // find lines which fit WHERE expression
                auto line_indexes = calculate_where(table, start_where,
                        result.cend(), text_fields, long_fields);
                
                // update selected lines
                for (auto line = table.line_begin(); !line.is_end(); ++line) {
                    if (line_indexes.find(line.number) != line_indexes.end()) {
                        
                        // TEXT expression
                        if (text_fields.find(result[2]) != text_fields.end()) {
                            if (text_fields.find(result[3]) != text_fields.end())
                                line.put<string>(result[2], line.get<string>(result[3]));
                            else line.put<string>(result[2], result[3]);
                            continue;
                        }
                        
                        // LONG expression
                        set<string> operations = { "+", "-", "*", "/", "%" };
                        stack<double> double_ops;
                        for (auto it = result.begin() + 3; it != start_where - 1; ++it) {
                            
                            // if current lexem is an arithmetic operation
                            if (operations.find(*it) != operations.end())
                                calculate_arithm(double_ops, *it);
                            
                            // if current lexem is an operand
                            else {
                                if (long_fields.find(*it) != long_fields.end()) {
                                    double_ops.push((double)line.get<long>(*it));
                                } else double_ops.push((double)stol(*it));
                            }
                        }
                        
                        line.put<long>(result[2], double_ops.top());
                    }
                }
                
                // print table
                s.send(table.print_table());
                table.close();
            }
        }
        
    } catch (string message) {
        s.send(message);
    } catch (std::exception& e) {
        s.send(e.what());
    }
    
    remove("My_socket");
    Dbms::drop_all();
    
    return 0;
}

