#by Denis Kozub
#total 6 sections: 2 for each command (cp, cmp, tail)
#cp_tests
#cp_tests_valgrind
#cmp_tests
#cmp_tests_valgrind
#tail_tests
#tail_tests_valgrind
cp_tests:
	@gcc -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm cp.c

	@echo no operands test:
	./a.out || true
	@echo __________

	@echo no destination test:
	./a.out cp.c || true
	@echo __________

	@echo too many arguments test:
	./a.out cp.c temp1 temp2|| true
	@echo __________

	@echo file not exist test:
	./a.out temp1 temp2 || true
	@echo __________

	@echo copy empty file to temp, echo temp, rm temp test:
	./a.out tests/test_empty temp || true
	@echo temp:
	@echo < temp
	@rm temp
	@echo __________

	@echo copy cp.c to temp, less temp, rm temp test:
	./a.out cp.c temp || true
	@less temp
	@rm temp

cp_tests_valgrind:
	@gcc -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm cp.c

	@echo no operands test:
	valgrind ./a.out || true
	@echo __________

	@echo no destination test:
	valgrind ./a.out cp.c || true
	@echo __________

	@echo too many arguments test:
	valgrind ./a.out cp.c temp1 temp2|| true
	@echo __________

	@echo file not exist test:
	valgrind ./a.out temp1 temp2 || true
	@echo __________

	@echo copy empty file to temp, echo temp, rm temp test:
	valgrind ./a.out tests/test_empty temp || true
	@echo temp:
	@echo < temp
	@rm temp
	@echo __________

	@echo copy cp.c to temp, less temp, rm temp test:
	valgrind ./a.out cp.c temp || true
	@less temp
	@rm temp

cmp_tests:
	@gcc -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm cmp.c

	@echo no operands test:
	./a.out || true
	@echo __________

	@echo one operand test:
	./a.out cmp.c || true
	@echo __________

	@echo too many arguments test:
	./a.out cmp.c temp1 temp2|| true
	@echo __________

	@echo file not exist test:
	./a.out temp1 temp2 || true
	@echo __________

	@echo compare empty file to empty test:
	./a.out tests/test_empty tests/test_empty || true
	@echo __________

	@echo compare empty file to non-empty test:
	./a.out tests/test_empty cmp.c || true
	@echo __________

	@echo compare non-empty file to empty test:
	./a.out cmp.c tests/test_empty || true
	@echo __________

	@echo compare file to itself test:
	./a.out cmp.c cmp.c || true
	@echo __________

	@echo compare cp.c to cmp.c test:
	./a.out cp.c cmp.c || true

cmp_tests_valgrind:
	@gcc -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm cmp.c

	@echo no operands test:
	valgrind ./a.out || true
	@echo __________

	@echo one operand test:
	valgrind ./a.out cmp.c || true
	@echo __________

	@echo too many arguments test:
	valgrind ./a.out cmp.c temp1 temp2|| true
	@echo __________

	@echo file not exist test:
	valgrind ./a.out temp1 temp2 || true
	@echo __________

	@echo compare empty file to empty test:
	valgrind ./a.out tests/test_empty tests/test_empty || true
	@echo __________

	@echo compare empty file to non-empty test:
	valgrind ./a.out tests/test_empty cmp.c || true
	@echo __________

	@echo compare non-empty file to empty test:
	valgrind ./a.out cmp.c tests/test_empty || true
	@echo __________

	@echo compare file to itself test:
	valgrind ./a.out cmp.c cmp.c || true
	@echo __________

	@echo compare cp.c to cmp.c test:
	valgrind ./a.out cp.c cmp.c || true
tail_tests:
	@gcc -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm tail.c

	@echo no operands test:
	./a.out || true
	@echo __________

	@echo two operands test:
	./a.out -n tests/test_tail || true
	@echo __________

	@echo too many operands test:
	./a.out -n +15 24 tests/test_tail || true
	@echo __________

	@echo wrong flag test:
	./a.out -m -54 tests/test_tail || true
	@echo __________

	@echo file not exist 1 test:
	./a.out -n 12 temp || true
	@echo __________

	@echo file not exist 2 test:
	./a.out temp || true
	@echo __________

	@echo diapason test:
	./a.out -n 1000000000000000000000000000000000 tests/test_tail || true
	@echo __________

	@echo wrong number test:
	./a.out -n +123g tests/test_tail || true
	@echo __________

	@echo empty file test:
	./a.out tests/test_empty || true
	@echo __________

	@echo funclional 1 test:
	./a.out tests/test_tail || true
	@echo __________

	@echo funclional 2 test:
	./a.out -n 5 tests/test_tail || true
	@echo __________

	@echo funclional 3 test:
	./a.out -n -6 tests/test_tail || true
	@echo __________

	@echo funclional 4 test:
	./a.out -n +9 tests/test_tail || true
	@echo __________

	@echo funclional 5 test:
	./a.out -n 0 tests/test_tail || true
	@echo __________

	@echo funclional 6 test:
	./a.out -n +0 tests/test_tail || true

tail_tests_valgrind:
	@gcc -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm tail.c

	@echo no operands test:
	valgrind ./a.out || true
	@echo __________

	@echo two operands test:
	valgrind ./a.out -n tests/test_tail || true
	@echo __________

	@echo too many operands test:
	valgrind ./a.out -n +15 24 tests/test_tail || true
	@echo __________

	@echo wrong flag test:
	valgrind ./a.out -m -54 tests/test_tail || true
	@echo __________

	@echo file not exist 1 test:
	valgrind ./a.out -n 12 temp || true
	@echo __________

	@echo file not exist 2 test:
	valgrind ./a.out temp || true
	@echo __________

	@echo diapason test:
	valgrind ./a.out -n 1000000000000000000000000000000000 tests/test_tail || true
	@echo __________

	@echo wrong number test:
	valgrind ./a.out -n +123g tests/test_tail || true
	@echo __________

	@echo empty file test:
	valgrind ./a.out tests/test_empty || true
	@echo __________

	@echo funclional 1 test:
	valgrind ./a.out tests/test_tail || true
	@echo __________

	@echo funclional 2 test:
	valgrind ./a.out -n 5 tests/test_tail || true
	@echo __________

	@echo funclional 3 test:
	valgrind ./a.out -n -6 tests/test_tail || true
	@echo __________

	@echo funclional 4 test:
	valgrind ./a.out -n +9 tests/test_tail || true
	@echo __________

	@echo funclional 5 test:
	valgrind ./a.out -n 0 tests/test_tail || true
	@echo __________

	@echo funclional 6 test:
	valgrind ./a.out -n +0 tests/test_tail || true

