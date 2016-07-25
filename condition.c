#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "condition.h"

int meetsCond(char input, CondNode *cond) {
	CondType condFunc = cond->type;
	char inp1 = cond->arg1, inp2 = cond->arg2;
	return condFunc(input, inp1, inp2) != cond->ifnegate;
}

CondNode *nextCond(CondNode *cond) {
	if(cond == NULL) return NULL;
	return cond->nextcond;
}

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
