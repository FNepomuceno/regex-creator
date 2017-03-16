#define CONDITION_PACKAGE
#define TEST

#define TEST_MSG(x) printf("\"%s\"\n", #x)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "function.h"

static const int OR_OP = 2;
static const int AND_OP = 3;
static const int IGNORE_OP = 4;
static const int YES_NEGATE = 5;
static const int NO_NEGATE = 6;
static const int IGNORE_NEGATE = 7;

static const int TRUE_BOOL = 1;
static const int FALSE_BOOL = 0;

static CondNode *newNodeLeaf(CondFunc, char, char);
static CondNode *newNodeBranch(CondNode *, CondNode *, int);
//package access
CondNode *negateNode(CondNode *);
//public access?
void cleanCondNode(CondNode *data);

//package access
CondNode *mergeNodes(CondNode *, CondNode *, int);
static CondNode *linkNodes(CondNode *, CondNode *, int);
static CondNode *branchNodes(CondNode *, CondNode *, int);

int evaluateCond(CondNode *cond, char input);
static int evaluateFunc(CondNode *cond, char input);
static int combineEvals(int expr1, int expr2, int op_flag);
static int isNotNegated(int negate_flag);
static int toBool(int eval);

static int isLeafCond(CondNode *);
static int isLinkCond(CondNode *);
static int isBranchCond(CondNode *);


struct CondNode {
	CondFunc *func;
	CondNode *chld1, *chld2; // children of node
	int op_flag; // and or flag
	int neg_flag; // negate flag
	char arg1, arg2; // arguments for func
};

static CondNode *newNodeLeaf(CondFunc func, char arg1,
		char arg2) {
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->func = func;
	newNode->chld1 = NULL;
	newNode->chld2 = NULL;
	newNode->op_flag = IGNORE_OP;
	newNode->neg_flag = NO_NEGATE;
	newNode->arg1 = arg1;
	newNode->arg2 = arg2;
	return newNode;
}

static CondNode *newNodeBranch(CondNode *node1, CondNode *node2,
		int op_flag) {
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->func = 0;
	newNode->chld1 = node1;
	newNode->chld2 = node2;
	newNode->op_flag = op_flag;
	newNode->neg_flag = IGNORE_NEGATE;
	newNode->arg1 = '\0';
	newNode->arg2 = '\0';
	return newNode;
}

CondNode *negateNode(CondNode *data) {
	assert(data->neg_flag == YES_NEGATE ||
		data->neg_flag == NO_NEGATE);
	if(data->neg_flag == NO_NEGATE) {
		data->neg_flag = YES_NEGATE;
	} else {
		data->neg_flag = NO_NEGATE;
	}
	return data;
}

void cleanCondNode(CondNode *data) {
	if(data == NULL) {
		return;
	}
	cleanCondNode(data->chld1);
	cleanCondNode(data->chld2);
	free(data);
}

CondNode *mergeNodes(CondNode *node1, CondNode *node2,
		int op_flag) {	
	if(node1 == NULL || node2 == NULL) {
		printf("ERROR: CANNOT COMBINE NULL NODES\n");
		return NULL;
	}
	if(isLeafCond(node1)) {
		return linkNodes(node1, node2, op_flag);
	} else {
		return branchNodes(node1, node2, op_flag);
	}
}

static CondNode *linkNodes(CondNode *node1, CondNode *node2,
		int op_flag) {
	CondNode *result = node1;
	result->op_flag = op_flag;
	result->chld2 = node2;
	return result;
}

static CondNode *branchNodes(CondNode *node1, CondNode *node2,
		int op_flag) {
	CondNode *result = newNodeBranch(node1, node2, op_flag);
	return result;
}

int evaluateCond(CondNode *cond, char input) {
	assert(cond != NULL);
	if(isBranchCond(cond)) {
		return combineEvals(evaluateCond(cond->chld1, input),
			evaluateCond(cond->chld2, input), cond->op_flag);
	} else if(isLinkCond(cond)) {
		return combineEvals(evaluateFunc(cond, input),
			evaluateCond(cond->chld2, input), cond->op_flag);
	} else {
		return evaluateFunc(cond, input);
	}
}

static int evaluateFunc(CondNode *cond, char input) {
	return toBool(cond->func(input, cond->arg1, cond->arg2)) ==
		isNotNegated(cond->neg_flag);
}

static int combineEvals(int expr1, int expr2, int op_flag) {
	assert(op_flag == OR_OP || op_flag == AND_OP);
	if(op_flag == OR_OP) {
		return expr1 || expr2;
	} else {
		return expr1 && expr2;
	}
}

static int isNotNegated(int negate_flag) {
	assert(negate_flag == YES_NEGATE || negate_flag == NO_NEGATE);
	if(negate_flag == YES_NEGATE) {
		return FALSE_BOOL;
	} else {
		return TRUE_BOOL;
	}
}

static int toBool(int eval) {
	if(eval) {
		return TRUE_BOOL;
	} else {
		return FALSE_BOOL;
	}
}

static int isBranchCond(CondNode *cond) {
	return (cond->chld1 != NULL) && (cond->chld2 != NULL);
}

static int isLinkCond(CondNode *cond) {
	return (cond->chld1 == NULL) && (cond->chld2 != NULL);
}

static int isLeafCond(CondNode *cond) {
	return (cond->chld1 == NULL) && (cond->chld2 == NULL);
}

#ifdef TEST
void testNodes();
void testLeafEval();
CondNode *getTestLeaf();
void testOrLinkEval();
CondNode *getTestOrLink();
void testAndLinkEval();
CondNode *getTestAndLink();
void testMultiLinkEval();
CondNode *getTestMultiLink();
void testBranchEval();
CondNode *getTestBranch();
void testNegateEval();
CondNode *getTestNegate();

int main() {
	testNodes();
	return 0;
}

void testNodes() {
	printf("Starting Tests\n");
	testLeafEval();
	testOrLinkEval();
	testAndLinkEval();
	testMultiLinkEval();
	testBranchEval();
	testNegateEval();
	printf("Ending Tests\n");
}

void testLeafEval() {
	printf("Testing leaf node evaluation\n");
	CondNode *test = getTestLeaf();
	assert(evaluateCond(test, 'b') == FALSE_BOOL);
	assert(evaluateCond(test, '4') == TRUE_BOOL);
	cleanCondNode(test);
}

CondNode *getTestLeaf() {
	return newNodeLeaf(isDigitChar, 0, 0);
}

void testOrLinkEval() {
	printf("Testing link node evaluation using or\n");
	CondNode *test = getTestOrLink();
	assert(evaluateCond(test, ' ') == TRUE_BOOL);
	assert(evaluateCond(test, 'H') == TRUE_BOOL);
	assert(evaluateCond(test, 'i') == TRUE_BOOL);
	assert(evaluateCond(test, '_') == TRUE_BOOL);
	assert(evaluateCond(test, '5') == FALSE_BOOL);
	cleanCondNode(test);
}

CondNode *getTestOrLink() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = newNodeLeaf(isWhitespace, 0, 0);
	return linkNodes(test1, test2, OR_OP);	
}

void testAndLinkEval() {
	printf("Testing link node evaluation using and\n");
	CondNode *test = getTestAndLink();
	assert(evaluateCond(test, 'b') == FALSE_BOOL);
	assert(evaluateCond(test, 'o') == TRUE_BOOL);
	assert(evaluateCond(test, 'y') == FALSE_BOOL);
	assert(evaluateCond(test, '!') == FALSE_BOOL);
	cleanCondNode(test);
}

CondNode *getTestAndLink() {
	CondNode *test1 = newNodeLeaf(inRangeChar, 'a', 'p');
	CondNode *test2 = newNodeLeaf(inRangeChar, 'h', 'z');
	return linkNodes(test1, test2, AND_OP);	
}

void testMultiLinkEval() {
	printf("Testing link node evaluation using multiple nodes\n");
	CondNode *test = getTestMultiLink();
	assert(evaluateCond(test, '{') == FALSE_BOOL);
	assert(evaluateCond(test, 'C') == FALSE_BOOL);
	assert(evaluateCond(test, 'a') == TRUE_BOOL);
	assert(evaluateCond(test, 'W') == TRUE_BOOL);
	assert(evaluateCond(test, 'z') == FALSE_BOOL);
	assert(evaluateCond(test, '?') == TRUE_BOOL);
	assert(evaluateCond(test, ']') == FALSE_BOOL);
	cleanCondNode(test);
}

CondNode *getTestMultiLink() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = newNodeLeaf(inRangeChar, 'I', 'q');
	CondNode *test3 = linkNodes(test1, test2, AND_OP);
	CondNode *test4 = newNodeLeaf(equalsChar, '?', 0);
	return linkNodes(test4, test3, OR_OP);
}

void testBranchEval() {
	printf("Testing branch node evaluation\n");
	CondNode *test = getTestBranch();
	assert(evaluateCond(test, '<') == FALSE_BOOL);
	assert(evaluateCond(test, '1') == FALSE_BOOL);
	assert(evaluateCond(test, '6') == TRUE_BOOL);
	assert(evaluateCond(test, '%') == FALSE_BOOL);
	cleanCondNode(test);
}

CondNode *getTestBranch() {
	CondNode *test1 = newNodeLeaf(isDigitChar, 0, 0);
	CondNode *test2 = newNodeLeaf(inRangeChar, '5', '>');
	return branchNodes(test1, test2, AND_OP);
}

void testNegateEval() {
	printf("Testing evaluation with node negation\n");
	CondNode *test = getTestNegate();
	assert(evaluateCond(test, '\\') == TRUE_BOOL);
	assert(evaluateCond(test, 'e') == FALSE_BOOL);
	assert(evaluateCond(test, 'Z') == FALSE_BOOL);
	assert(evaluateCond(test, '#') == FALSE_BOOL);
	cleanCondNode(test);
}

CondNode *getTestNegate() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = negateNode(test1);
	CondNode *test3 = newNodeLeaf(inRangeChar, 'Y', 'e');
	CondNode *test4 = negateNode(test3);
	CondNode *test5 = negateNode(test4);
	return linkNodes(test2, test5, AND_OP);
}
#endif
