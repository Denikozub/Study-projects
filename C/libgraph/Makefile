test_libgraph:	libgraph.a

libgraph.a:	source.o
	ar -cr libgraph.a source.o 
	ranlib libgraph.a

source.o:	source.c
	gcc -c -Wall source.c

remove:
	rm -rf source.o
	rm -rf libgraph.a

