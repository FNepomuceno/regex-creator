#ifndef CONDITION_H
#define CONDITION_H

typedef struct CondNode  CondNode;
typedef int (*CondType)(char, char, char);

int satisfiesCond(char, CondNode *);
CondNode *nextOrCond(CondNode *);
CondNode *nextAndCond(CondNode *);
CondNode *addBranchCond(CondNode *, int , CondNode *);
CondNode *newCond(CondType, int, char, char);
void cleanCond(CondNode *);
int matches(char, char, char);
int inrange(char, char, char);
int isalphanum(char, char, char);
int iswordchar(char, char, char);
int isnumchar(char, char, char);
int isspacechar(char, char, char);

#endif
