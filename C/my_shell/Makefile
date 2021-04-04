#by Denis Kozub

my_shell:	stack.o recursive_descent.o calc_elem.o rpn.o
	@gcc -Wall my_shell.c stack.o recursive_descent.o calc_elem.o rpn.o
	@rm -rf *.o

stack.o:
	@gcc -c -Wall stack.c

recursive_descent.o:
	@gcc -c -Wall recursive_descent.c

calc_elem.o:
	@gcc -c -Wall calc_elem.c

rpn.o:
	@gcc -c -Wall rpn.c

test01:
	@valgrind ./a.out < tests/test01 || true

test02:
	@valgrind ./a.out < tests/test02 || true

test03:
	@valgrind ./a.out < tests/test03 || true

test04:
	@valgrind ./a.out < tests/test04 || true

test05:
	@valgrind ./a.out < tests/test05 || true

test06:
	@valgrind ./a.out < tests/test06 || true

test07:
	@valgrind ./a.out < tests/test07 || true

test08:
	@valgrind ./a.out < tests/test08 || true

test09:
	@valgrind ./a.out < tests/test09 || true

test10:
	@valgrind ./a.out < tests/test10 || true

test11:
	@valgrind ./a.out < tests/test11 || true

