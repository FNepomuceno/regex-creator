#include <stdio.h>
#include <stdlib.h>
#include "stak.h"
#include "condition.h"

CondStakNode *startStak() {
	return (CondStakNode *) calloc(sizeof(CondStakNode), 1);
}

CondNode *extractCond(CondStakNode *stak) {
	return stak->cond;
}

static int tryFillAnd(CondStakNode *stak) {
	if(stak == NULL) return 1;
	if(stak->cond != NULL) return 1;
	if(stak->filledand) return 0;
	if(!tryFillAnd(stak->prevstak)) return 0;
	stak->filledand = 1;
	return 1;
}

static int tryFillOr(CondStakNode *stak) {
	if(stak == NULL) return 1;
	if(stak->cond != NULL) return 1;
	if(stak->filledor) return 0;
	if(!tryFillOr(stak->prevstak)) return 0;
	stak->filledor = 1;
	return 1;
}

static CondStakNode *newStakNode(CondStakNode *prev,
	CondNode *cond, int branchand, int filledand, int filledor) {
	CondStakNode *result = malloc(sizeof(CondStakNode));
	result->prevstak = prev;
	result->cond = cond;
	result->branchand = branchand;
	result->filledand = filledand;
	result->filledor = filledor;
	return result;
}

CondStakNode *addAnd(CondStakNode *stak) {
	if(!tryFillAnd(stak)) return stak;
	return newStakNode(stak, NULL, 1, 1, 0);
}

CondStakNode *addOr(CondStakNode *stak) {
	if(!tryFillOr(stak)) return stak;
	return newStakNode(stak, NULL, 0, 0, 1);
}

CondStakNode *addCond(CondStakNode *stak, CondType type,
	int negate, char arg1, char arg2) {
	CondNode *cond1 = extractCond(stak);
	CondNode *cond2 = newCond(type, negate, arg1, arg2);
	CondStakNode *curstak = stak, *prevstak;
	while(cond1 != NULL) {
		prevstak = curstak;
		curstak = prevstak->prevstak;
		cond2 = addBranchCond(cond1, prevstak->branchand, cond2);
		cond1 = curstak->cond;
		free(prevstak);
	}
	curstak->cond = cond2;
	return curstak;
}

void cleanStak(CondStakNode *stak) {
	CondStakNode *cur = stak, *prev;
	while(cur != NULL) {
		prev = cur;
		cur = cur->prevstak;
		free(prev);
	}
}
