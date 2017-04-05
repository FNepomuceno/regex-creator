.PHONY: all clean todo test redo

files=graphregex.c regex.c testing.c utils/bool.c

all:

todo:
	@grep TODO *.c */*.c */*/*.c;:

test:
	@gcc $(files) -Wall -g -o Test
	@valgrind ./Test | grep FAILED;:
	@rm Test

clean:

redo: clean all
