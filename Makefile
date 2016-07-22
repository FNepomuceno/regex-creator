.PHONY: all clean

srcs := *.c
cc := gcc

all:
	@$(cc) $(srcs) -o test

clean:
	@rm test
