#include <stdio.h>
#include <stdlib.h>

typedef int (*CondType)(char, char);

typedef struct a {
	CondType type;
	char arg1, arg2;
} Cond;

int matches(char c, char ignore) {
	
}

int main() {
	printf(" \n hello\n");
	return 0;
}
