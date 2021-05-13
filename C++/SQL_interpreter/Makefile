TARGET1 = server
TARGET2 = client
CC = gcc
CXX = g++
CFLAGS = -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm
CXXFLAGS = -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm
objects = dbms.o table.o al_table.o parser.o lex_analyzer.o poliz.o calculate.o

all:$(TARGET1) $(TARGET2)

$(TARGET1): $(objects)
	$(CXX) -o $(TARGET1) $(CXXFLAGS) server.cpp $(objects)

dbms.o:
	$(CXX) -c $(CXXFLAGS) dbms.cpp

table.o:
	$(CXX) -c $(CXXFLAGS) table.cpp

al_table.o:
	$(CC) -c $(CFLAGS) al_table.c

parser.o:
	$(CXX) -c $(CXXFLAGS) parser.cpp

lex_analyzer.o:
	$(CXX) -c $(CXXFLAGS) lex_analyzer.cpp

poliz.o:
	$(CXX) -c $(CXXFLAGS) poliz.cpp

calculate.o:
	$(CXX) -c $(CXXFLAGS) calculate.cpp

$(TARGET2):
	$(CXX) -o $(TARGET2) $(CXXFLAGS) client.cpp

clean:
	rm -f *.o $(TARGET1) $(TARGET2)
