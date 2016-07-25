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

CondNode *appendCond(CondNode *oldCond, CondNode *newCond) {
	if(newCond == NULL) return oldCond;
	newCond->nextcond = oldCond;
	return newCond;
}

CondNode *newCond(CondType type, int negate, char arg1, char arg2) {
	CondNode *result = (CondNode *) malloc(sizeof(CondNode));
	result->type = type;
	result->ifnegate = negate;
	result->arg1 = arg1;
	result->arg2 = arg2;
	result->nextcond = NULL;
	return result;
}

void cleanCond(CondNode *cond) {
	CondNode *cur = cond;
	while(cur != NULL) {
		CondNode *prev = cur;
		cur = nextCond(cur);
		free(prev);
	}
}

int matches(char input, char c, char ignore) {
	return input == c;
}

int inrange(char input, char start, char end) {
	return input >= start && input <= end;
}

int isalphanum(char input, char ign1, char ign2) {
	return isalnum(input);
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
