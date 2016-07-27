.PHONY: all clean

srcs := *.c
cc := gcc

all:
	@$(cc) $(srcs) -g -o test

clean:
	@rm test
