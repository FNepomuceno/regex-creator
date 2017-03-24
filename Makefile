.PHONY: all clean todo test redo

all:

test:

todo:
	@grep TODO *.c */*.c */*/*.c;:

clean:

redo: clean all
