#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "condition.h"
//#define __DEBUG__

struct CondNode {
	CondType type;
	int ifnegate;
	char arg1, arg2;
	CondNode *orcond, *andcond;
};


static int meetsCond(char input, CondNode *cond) {
	CondType condFunc = cond->type;
	char inp1 = cond->arg1, inp2 = cond->arg2;
	return condFunc(input, inp1, inp2) != cond->ifnegate;
}

int satisfiesAndCond(char input, CondNode *cond) {
#ifdef __DEBUG__
	if(meetsCond(input, cond)) {
		printf("going down\n");
		return satisfiesCond(input, cond->andcond);
	}
	return 0;
#else
	return meetsCond(input, cond) &&
		satisfiesCond(input, cond->andcond);
#endif
}

int satisfiesCond(char input, CondNode *cond) {
	if(cond == NULL) return 1;
#ifdef __DEBUG__
	printf("%c %c\n", cond->arg1, cond->arg2);
	if(satisfiesAndCond(input, cond)) {
		printf("and condition satisfied\n");
		return 1;
	} else {
		printf("and condition not satisfied\n");
		if(cond->orcond == NULL) {
			printf("out of or conds\n");
			return 0;
		} else {
			printf("still got or conds\n");
			return satisfiesCond(input, cond->orcond);
		}
	}
#else
	return satisfiesAndCond(input, cond) ||
		cond->orcond != NULL &&
		satisfiesCond(input, cond->orcond);
#endif
}

CondNode *nextOrCond(CondNode *cond) {
	if(cond == NULL) return NULL;
	return cond->orcond;
}

CondNode *nextAndCond(CondNode *cond) {
	if(cond == NULL) return NULL;
	return cond->andcond;
}

CondNode *addBranchCond(CondNode *oldCond, int branchand,
	CondNode *newCond) {
	if(branchand) {
		oldCond->andcond = newCond;
	} else {
		oldCond->orcond = newCond;
	}
	return oldCond;
}

CondNode *newCond(CondType type,
	int negate,	char arg1, char arg2) {
	CondNode *result = (CondNode *) malloc(sizeof(CondNode));
	result->type = type;
	result->ifnegate = negate;
	result->arg1 = arg1;
	result->arg2 = arg2;
	result->orcond = NULL;
	result->andcond = NULL;
	return result;
}

void cleanCond(CondNode *cond) {
	if(cond == NULL) return;
	cleanCond(nextOrCond(cond));
	cleanCond(nextAndCond(cond));
	free(cond);
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
