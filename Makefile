.PHONY: all clean test

srcs := *.c
cc := gcc

all:
	@$(cc) $(srcs) -g -o test

test:
	make; valgrind ./test; make clean

clean:
	@rm test
