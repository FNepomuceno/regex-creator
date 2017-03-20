.PHONY: all clean test CharstringTest

srcs := *.c
cc := gcc

all:
	@$(cc) $(srcs) -g -o test

CharstringTest:
	@gcc charstring.c utils/bool.c -g -o CharstringTest
	@valgrind ./CharstringTest 1> /dev/null;:
	@./CharstringTest | grep "FAILED";:
	@rm CharstringTest

test:
	make
	valgrind ./test
	make clean

clean:
	@rm test
