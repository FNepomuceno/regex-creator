.PHONY: all clean CharstringTest todo

all:

CharstringTest:
	@gcc charstring.c utils/bool.c -g -o CharstringTest
	@valgrind ./CharstringTest 1> /dev/null;:
	@./CharstringTest | grep "FAILED";:
	@rm CharstringTest

todo:
	@grep TODO *.c */*.c

clean:
	@rm test
