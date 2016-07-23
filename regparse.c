#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef int (*CondType)(char, char, char);
typedef struct RegexNode RegexNode;
typedef struct StateNode StateNode;
typedef struct CondNode  CondNode;
typedef struct PathNode  PathNode;

struct RegexNode {
	char **name;
	RegexNode *nextregex;
};

struct StateNode {
	RegexNode *matchedRegexes;
	PathNode *paths;
};

//TODO: put this in condition.c
struct CondNode {
	CondType type;
	int ifnegate;
	char arg1, arg2;
	CondNode *nextcond;
};

//TODO: put this in path.c
struct PathNode {
	StateNode *dst;
	CondNode *conds;
	PathNode *nextPath;
};

//TODO: put this function in condition.c
int meetsCond(char input, CondNode *cond) {
	CondType condFunc = cond->type;
	char inp1 = cond->arg1, inp2 = cond->arg2;
	return condFunc(input, inp1, inp2) != cond->ifnegate;
}

//TODO: put this function in path.c
int satisfiesPath(char input, PathNode *path) {
	CondNode *cond = path->conds;
	while(cond != NULL) {
		if(!meetsCond(input, cond)) {
			return 0; // false
		}
		cond = cond->nextcond;
	}
	return 1; // true
}

//TODO: put these 5 functions in condition.c
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
