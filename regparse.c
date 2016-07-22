#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef int (*CondType)(char, char);

typedef struct a {
} StateNode;

typedef struct b {
	CondType type;
	int ifnegate;
	char arg1, arg2;
	struct b *nextcond;
} CondNode;

typedef struct c {
	StateNode *src, *dst;
	CondNode *conds;
} PathNode;

int matches(char input, char c, char ignore) {
	return input == c;
}

int inrange(char input, char start, char end) {
	return input >= start && input <= end;
}

int iswordchar(char input, char ign1, char ign2) {
	return isalnum(input) || input == '_';
}

int isnumchar(char input, char ign1, char ign2) {
	return isdigit(input);
}

int isspacechar(char input, char ign1, char ign2) {
	return isspace(input);
}

int main() {
	printf("hello\n");
	return 0;
}
