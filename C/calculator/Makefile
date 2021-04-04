calculate:	polis.o stack.o recursive_descent.o var_table.o
	gcc -Wall calculate.c polis.o stack.o recursive_descent.o var_table.o

polis.o:
	gcc -c -Wall polis.c

stack.o:
	gcc -c -Wall stack.c

recursive_descent.o:
	gcc -c -Wall recursive_descent.c

var_table.o:
	gcc -c -Wall var_table.c

remove:
	rm -rf *.o

