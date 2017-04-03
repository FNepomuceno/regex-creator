.PHONY: all clean todo test redo

files=regex.c testing.c

all:

todo:
	@grep TODO *.c */*.c */*/*.c;:

test:
	@gcc $(files) -Wall -g -o Test
	@valgrind ./Test | grep FAILED;:
	@rm Test

clean:

redo: clean all
