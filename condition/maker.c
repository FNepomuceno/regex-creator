#include <stdio.h>
#include <stdlib.h>

//condition.h
typedef struct CondNode CondNode;
typedef struct CondMakr CondMakr;
typedef int CondFunc(char, char, char);//TODO: look up syntax
static const int OR_OP = 1;
static const int AND_OP = 0;
static const int IGNORE_OP = -1;
static const int YES_NEGATE = 1;
static const int NO_NEGATE = 0;
static const int IGNORE_NEGATE = -1;


static CondMakr *addNodeSimple(CondNode *, CondMakr *);

struct CondMakr {
	CondMakr *parent;
	CondNode *result;
	CondNode *arg1, *arg2;
	int op_flag;
}

CondMakr *newCondMakr (CondMakr *parent, int op_flag) {
	CondMakr *newMakr = malloc(sizeof (CondMakr));
	newMakr->op_flag = op_flag;
	newMakr->parent = parent;
	newMakr->result = NULL;
	newMakr->arg1 = NULL;
	newMakr->arg2 = NULL;
	return newMakr;
}

static  simplifyMakr(CondMakr *stak) {
	if(stak == NULL) return NULL;
	if(stak->arg2 != NULL) return stak;

	CondMakr *newMakr = stak;
	CondNode *resultNode = mergeNodes(newMakr->arg1,
		newMakr->arg2, newMakr->op_flag);
	newMakr->arg1 = NULL;
	newMakr->arg2 = NULL;
	while(newMakr->parent != NULL) {
		CondMakr *parent = newMakr->parent;
		parent = addNodeSimple(result, parent);
		free(newMakr);
		if(parent->arg2 == NULL) return parent;
		newMakr = parent;
		resultNode = mergeNodes(newMakr->arg1, newMakr->arg2,
			newMakr->op_flag);
	}
	newStak->result = resultNode;
	return newStak;
}

static CondMakr *addNodeSimple(CondNode *node, CondMakr *stak) {
	if(stak->arg1 == NULL) {
		stak->arg1 = node;
	} else {
		stak->arg2 = node;
	}
	return stak;
}

CondMakr *addNodeToMakr(CondNode *node, CondMakr *stak) {
	if(node == NULL) {
		printf("ERROR: CANNOT ADD A NULL NODE TO A MAKER\n");
		return stak;
	} if(stak == NULL) {
		printf("ERROR: CANNOT ADD A NODE TO A NULL MAKER\n");
		return NULL;
	}
	
	CondMakr *newMakr = stak;
	newMakr = addNodeSimple(node, newMakr);
	newMakr = simplifyMakr(newMakr);
	return newMakr;
}

CondMakr *addOr(CondMakr *stak) {
	return newCondMakr(stak, OR_OP);
}

CondMakr *addAnd(CondMakr *stak) {
	return newCondMakr(stak, AND_OP);
}

CondMakr *addNewCond(CondMakr *stak, CondFunc func, char arg1,
		char arg2, int neg_flag) {
	if(stak == NULL) {
		prinf("ERROR: NEED A CONDITION-MAKER\n");
		return NULL;
	}

	CondNode *newNode = newNodeLeaf(func, arg1, arg2, neg_flag);
	return addCond(newNode, stak);
}

CondMakr *addCond(CondMakr *stak, CondNode *node) {
	if(stak == NULL) {
		prinf("ERROR: NEED A CONDITION-MAKER\n");
		return NULL;
	}

	return addNodeToMakr(node, stak);
}

int main() {
	return 0;
}
