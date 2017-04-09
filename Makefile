.PHONY: all clean todo test redo

files_regex := regex/graphregex.c regex/regex.c \
	utils/chartype.c utils/bool.c utils/test.c
files_condition := condition/builder.c condition/parsestring.c \
	condition/maker.c condition/node.c \
	condition/list.c condition/tags.c utils/bool.c \
	condition/function/data.c condition/function/info.c \
	condition/condition.c utils/chartype.c utils/test.c
flags_lenient := -Wall -g
flags_strict :=-Wall -Werror -g
group := condition
files := $(condition_files)
flagtype := strict

ifeq ($(group), regex)
	files := $(files_regex)
else
ifeq ($(group), condition)
	files := $(files_condition)
else
endif
endif

ifeq ($(flagtype), lenient)
	flags := $(flags_lenient)
else
ifeq ($(flagtype), strict)
	flags := $(flags_strict)
else
endif
endif

all:

todo:
	@grep TODO *.c */*.c */*/*.c;:

test:
	@time gcc $(files) $(flags) -o Test
	@time valgrind --leak-check=full ./Test | grep FAILED;:
	@rm Test

clean:

redo: clean all
