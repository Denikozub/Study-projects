TARGET=test
CXX=g++
CXXFLAGS=-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm
OBJECTS=test.o

all:$(TARGET)

clean:
	rm -f *.o $(TARGET) core

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

sparse_matrix.o: sparse_matrix.hpp
sparse_vector.o: sparse_vector.hpp
list.o: list.hpp
