#ifndef CONDITION_H
#define CONDITION_H

typedef struct CondNode  CondNode;
typedef int (*CondType)(char, char, char);

struct CondNode {
	CondType type;
	int ifnegate;
	char arg1, arg2;
	CondNode *nextcond;
};

int meetsCond(char, CondNode *);
CondNode *nextCond(CondNode *);
int matches(char, char, char);
int inrange(char, char, char);
int iswordchar(char, char, char);
int isnumchar(char, char, char);
int isspacechar(char, char, char);

#endif
