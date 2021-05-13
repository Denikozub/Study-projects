# Model SQL Interpreter
by Kozub Denis
___
 ### Particular qualities
 * brackets '(' and ')' must be separated by space symbols  
 * modified grammar described in grammar.pdf  
 * incorrect Model SQL request would end up with termination  
 * each request should end with '\n'  
 * program is stopped by entering "stop"  
 * server and client working in AF_UNIX  
 * server can only work with one client at once  
 * max length of sent and received string can be changed and is currently set to 1024  

 ### Assembly and run
 1) make - this would compile both server and client programs  
 2) ./server and ./client in different terminals would run the corresponding programs  

 ### Modules and classes
 1) al_table.h - table module, allows to create, edit, access and delete tables  
 2) table.hpp - class above al_table.h, provides iterable interface for working with tables  
 3) iterator.hpp - table iterator interface  
 4) dmbs.hpp - database manager class, allows to store and access multiple tables  
 5) socket.hpp - socket server and client interface  
 6) client.hpp - class connecting to server, sending requests and receiving results  
 7) server.hpp - class connecting to client, receiving requests and sending results  
 8) poliz.hpp - class filling and returning RPN  
 9) parser.hpp - class implementing recursive descent and returning RPN  
 10) calculate.hpp - module with functions calculating RPN WHERE expressions  
