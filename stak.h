#ifndef STAK_H
#define STAK_H

typedef struct CondStakNode CondStakNode;
typedef struct CondNode CondNode;
typedef int (*CondType)(char, char, char);

struct CondStakNode {
	CondStakNode *prevstak;
	CondNode *cond;
	int branchand, filledand, filledor;
};

CondStakNode *startStak();
CondNode *extractCond(CondStakNode *);
CondStakNode *addAnd(CondStakNode *);
CondStakNode *addOr(CondStakNode *);
CondStakNode *addCond(CondStakNode *, CondType, int, char, char);
void cleanStak(CondStakNode *);

#endif
