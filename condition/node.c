#define CONDITION_PACKAGE
#define CONDITION_NODE_MODULE
#define TEST

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "bool.h"
#include "function.h"
#include "node_module.h"

static CondNode *newNodeLeaf(CondFunc func, char arg1,
		char arg2) {
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->func = func;
	newNode->chld1 = nil;
	newNode->chld2 = nil;
	newNode->op_flag = IGNORE_OP;
	newNode->neg_flag = NO_NEGATE;
	newNode->arg1 = arg1;
	newNode->arg2 = arg2;
	return newNode;
}

static CondNode *newNodeBranch(CondNode *node1, CondNode *node2,
		int op_flag) {
	assert(node1 != NULL && node2 != NULL);
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

void cleanCondNode(CondNode *data) {
	assert(data != NULL);
	if(data == nil) {
		return;
	}
	cleanCondNode(data->chld1);
	cleanCondNode(data->chld2);
	free(data);
}

CondNode *mergeNodes(CondNode *node1, CondNode *node2,
		int op_flag) {	
	assert(node1 != NULL && node2 != NULL);
	if(node1 == nil || node2 == nil) {
		printf("ERROR: CANNOT COMBINE NIL NODES\n");
		return nil;
	}
	if(isLeafCond(node1)) {
		return linkNodes(node1, node2, op_flag);
	} else {
		return branchNodes(node1, node2, op_flag);
	}
}

static CondNode *linkNodes(CondNode *node1, CondNode *node2,
		int op_flag) {
	assert(node1 != NULL && node2 != NULL);
	CondNode *result = node1;
	result->op_flag = op_flag;
	result->chld2 = node2;
	return result;
}

static CondNode *branchNodes(CondNode *node1, CondNode *node2,
		int op_flag) {
	assert(node1 != NULL && node2 != NULL);
	CondNode *result = newNodeBranch(node1, node2, op_flag);
	return result;
}

CondNode *negateNode(CondNode *data) {
	assert(data != NULL);
	assert(data->neg_flag == YES_NEGATE ||
		data->neg_flag == NO_NEGATE);
	if(data->neg_flag == NO_NEGATE) {
		data->neg_flag = YES_NEGATE;
	} else {
		data->neg_flag = NO_NEGATE;
	}
	return data;
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

static int combineEvals(int bool1, int bool2, int op_flag) {
	assert(bool1 == TRUE_BOOL || bool1 == FALSE_BOOL);
	assert(bool2 == TRUE_BOOL || bool2 == FALSE_BOOL);
	assert(op_flag == OR_OP || op_flag == AND_OP);
	if(op_flag == OR_OP) {
		return boolOr(bool1, bool2);
	} else {
		return boolAnd(bool1, bool2);
	}
}

static int evaluateFunc(CondNode *cond, char input) {
	assert(cond != NULL && cond != nil);
	return boolXor(applyCondition(cond, input),
		isNegated(cond->neg_flag));
}

static int applyCondition(CondNode *cond, char input) {
	return cond->func(input, cond->arg1, cond->arg2)?
		TRUE_BOOL: FALSE_BOOL;
}

static int isNegated(int negate_flag) {
	assert(negate_flag == YES_NEGATE || negate_flag == NO_NEGATE);
	if(negate_flag == YES_NEGATE) {
		return TRUE_BOOL;
	} else {
		return FALSE_BOOL;
	}
}

static int isBranchCond(CondNode *cond) {
	assert(cond != NULL);
	return (cond->chld1 != nil) && (cond->chld2 != nil);
}

static int isLinkCond(CondNode *cond) {
	assert(cond != NULL);
	return (cond->chld1 == nil) && (cond->chld2 != nil);
}

static int isLeafCond(CondNode *cond) {
	assert(cond != NULL);
	return (cond->chld1 == nil) && (cond->chld2 == nil);
}

int isEquivalent(CondNode *node1, CondNode *node2) {
	assert(node1 != NULL && node2 != NULL);
	if(node1 == node2) {
		return TRUE_BOOL;
	} else if(node1 == nil || node2 == nil) {
		return FALSE_BOOL;
	} else if(!dataEquivalent(node1, node2)) {
		return FALSE_BOOL;
	}
	return boolAnd(isEquivalent(node1->chld1, node2->chld1),
		isEquivalent(node1->chld2, node2->chld2));
}

static int dataEquivalent(CondNode *node1, CondNode *node2) {
	assert(node1 != NULL && node2 != NULL);
	assert(node1 != nil && node2 != nil);
	return node1->func == node2->func &&
		node1->op_flag == node2->op_flag &&
		node1->neg_flag == node2->neg_flag &&
		node1->arg1 == node2->arg1 &&
		node1->arg2 == node2->arg2;
}

#ifdef TEST
static void testNodes();
static void testCondEval();
static void testCondEquivalence();
static void testLeafEval();
static void testOrLinkEval();
static void testAndLinkEval();
static void testMultiLinkEval();
static void testBranchEval();
static void testNegateEval();
static void testLeafEquivalence();
static void testLinkEquivalence();
static void testLinkSizeEquivalence();
static void testBranchEquivalence();
static void testNegateEquivalence();

static CondNode *getTestLeaf();
static CondNode *getTestOrLink();
static CondNode *getTestAndLink();
static CondNode *getTestMultiLink();
static CondNode *getTestBranch();
static CondNode *getTestNegate();
static CondNode *getTestDiffLeaf();
static CondNode *getTestDiffLink();
static CondNode *getTestDiffSizeLink();
static CondNode *getTestDiffBranch();
static CondNode *getTestNegateLeaf();
static CondNode *getTestDoubleNegateLeaf();

#define test(x) printf("%s:%d TEST `%s\' %s\n", __FILE__,\
	__LINE__, #x, (x)? "PASSED": "FAILED");

int main() {
	testNodes();
	return 0;
}

static void testNodes() {
	printf("Starting Node Tests\n");
	testCondEval();
	testCondEquivalence();
}

static void testCondEval() {
	testLeafEval();
	testOrLinkEval();
	testAndLinkEval();
	testMultiLinkEval();
	testBranchEval();
	testNegateEval();
	printf("\n");
}

static void testCondEquivalence() {
	testLeafEquivalence();
	testLinkEquivalence();
	testLinkSizeEquivalence();
	testBranchEquivalence();
	testNegateEquivalence();
	printf("\n");
}

static void testLeafEval() {
	printf("Testing leaf node evaluation\n");
	CondNode *test = getTestLeaf();
	test(evaluateCond(test, 'b') == FALSE_BOOL);
	test(evaluateCond(test, '4') == TRUE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestLeaf() {
	return newNodeLeaf(isDigitChar, 0, 0);
}

static void testOrLinkEval() {
	printf("Testing link node evaluation using or\n");
	CondNode *test = getTestOrLink();
	test(evaluateCond(test, ' ') == TRUE_BOOL);
	test(evaluateCond(test, 'H') == TRUE_BOOL);
	test(evaluateCond(test, 'i') == TRUE_BOOL);
	test(evaluateCond(test, '_') == TRUE_BOOL);
	test(evaluateCond(test, '5') == FALSE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestOrLink() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = newNodeLeaf(isWhitespace, 0, 0);
	return linkNodes(test1, test2, OR_OP);	
}

static void testAndLinkEval() {
	printf("Testing link node evaluation using and\n");
	CondNode *test = getTestAndLink();
	test(evaluateCond(test, 'b') == FALSE_BOOL);
	test(evaluateCond(test, 'o') == TRUE_BOOL);
	test(evaluateCond(test, 'y') == FALSE_BOOL);
	test(evaluateCond(test, '!') == FALSE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestAndLink() {
	CondNode *test1 = newNodeLeaf(inRangeChar, 'a', 'p');
	CondNode *test2 = newNodeLeaf(inRangeChar, 'h', 'z');
	return linkNodes(test1, test2, AND_OP);	
}

static void testMultiLinkEval() {
	printf("Testing link node evaluation using multiple nodes\n");
	CondNode *test = getTestMultiLink();
	test(evaluateCond(test, '{') == FALSE_BOOL);
	test(evaluateCond(test, 'C') == FALSE_BOOL);
	test(evaluateCond(test, 'a') == TRUE_BOOL);
	test(evaluateCond(test, 'W') == TRUE_BOOL);
	test(evaluateCond(test, 'z') == FALSE_BOOL);
	test(evaluateCond(test, '?') == TRUE_BOOL);
	test(evaluateCond(test, ']') == FALSE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestMultiLink() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = newNodeLeaf(inRangeChar, 'I', 'q');
	CondNode *test3 = linkNodes(test1, test2, AND_OP);
	CondNode *test4 = newNodeLeaf(equalsChar, '?', 0);
	return linkNodes(test4, test3, OR_OP);
}

static void testBranchEval() {
	printf("Testing branch node evaluation\n");
	CondNode *test = getTestBranch();
	test(evaluateCond(test, '<') == FALSE_BOOL);
	test(evaluateCond(test, '1') == FALSE_BOOL);
	test(evaluateCond(test, '6') == TRUE_BOOL);
	test(evaluateCond(test, '%') == FALSE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestBranch() {
	CondNode *test1 = newNodeLeaf(isDigitChar, 0, 0);
	CondNode *test2 = newNodeLeaf(inRangeChar, '5', '>');
	return branchNodes(test1, test2, AND_OP);
}

static void testNegateEval() {
	printf("Testing evaluation with node negation\n");
	CondNode *test = getTestNegate();
	test(evaluateCond(test, '\\') == TRUE_BOOL);
	test(evaluateCond(test, 'e') == FALSE_BOOL);
	test(evaluateCond(test, 'Z') == FALSE_BOOL);
	test(evaluateCond(test, '#') == FALSE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestNegate() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = negateNode(test1);
	CondNode *test3 = newNodeLeaf(inRangeChar, 'Y', 'e');
	CondNode *test4 = negateNode(test3);
	CondNode *test5 = negateNode(test4);
	return linkNodes(test2, test5, AND_OP);
}

static void testLeafEquivalence() {
	printf("Testing leaf node equivalence\n");
	CondNode *test1 = getTestLeaf();
	CondNode *test2 = test1;
	CondNode *test3 = getTestLeaf();
	CondNode *test4 = getTestDiffLeaf();
	test(isEquivalent(test1, test2) == TRUE_BOOL);
	test(isEquivalent(test1, test3) == TRUE_BOOL);
	test(isEquivalent(test3, test1) == TRUE_BOOL);
	test(isEquivalent(test1, test4) == FALSE_BOOL);
	test(isEquivalent(test3, test4) == FALSE_BOOL);
	test(isEquivalent(test4, test3) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test3);
	cleanCondNode(test4);
}

static CondNode *getTestDiffLeaf() {
	return newNodeLeaf(isWordChar, 0, 0);
}

static void testLinkEquivalence() {
	printf("Testing link node equivalence\n");
	CondNode *test1 = getTestOrLink();
	CondNode *test2 = test1;
	CondNode *test3 = getTestOrLink();
	CondNode *test4 = getTestDiffLink();
	test(isEquivalent(test1, test2) == TRUE_BOOL);
	test(isEquivalent(test1, test3) == TRUE_BOOL);
	test(isEquivalent(test3, test1) == TRUE_BOOL);
	test(isEquivalent(test1, test4) == FALSE_BOOL);
	test(isEquivalent(test3, test4) == FALSE_BOOL);
	test(isEquivalent(test4, test3) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test3);
	cleanCondNode(test4);
}

static CondNode *getTestDiffLink() {
	CondNode *test1 = newNodeLeaf(isWordChar, 0, 0);
	CondNode *test2 = newNodeLeaf(isWhitespace, 0, 0);
	return linkNodes(test1, test2, AND_OP);	
}

static void testLinkSizeEquivalence() {
	printf("Testing link node equivalence of different lengths\n");
	CondNode *test1 = getTestDiffLeaf();
	CondNode *test2 = getTestDiffLink();
	CondNode *test3 = getTestDiffSizeLink();
	test(isEquivalent(test1, test2) == FALSE_BOOL);
	test(isEquivalent(test2, test3) == FALSE_BOOL);
	test(isEquivalent(test3, test1) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
	cleanCondNode(test3);
}

static CondNode *getTestDiffSizeLink() {
	CondNode *test1 = newNodeLeaf(equalsChar, '*', 0);
	CondNode *test2 = newNodeLeaf(isWhitespace, 0, 0);
	CondNode *test3 = linkNodes(test2, test1, OR_OP);
	CondNode *test4 = newNodeLeaf(isWordChar, 0, 0);
	return linkNodes(test4, test3, AND_OP);	
}

static void testBranchEquivalence() {
	printf("Testing branch node equivalence\n");
	CondNode *test1 = getTestBranch();
	CondNode *test2 = getTestBranch();
	CondNode *test3 = getTestDiffBranch();
	test(isEquivalent(test1, test3) == FALSE_BOOL);
	test(isEquivalent(test2, test1) == TRUE_BOOL);
	test(isEquivalent(test3, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
	cleanCondNode(test3);
}

static CondNode *getTestDiffBranch() {
	CondNode *test1 = newNodeLeaf(isDigitChar, 0, 0);
	CondNode *test2 = newNodeLeaf(inRangeChar, '5', '=');
	return branchNodes(test1, test2, AND_OP);
}

static void testNegateEquivalence() {
	printf("Testing negate node equivalence\n");
	CondNode *test1 = getTestLeaf();
	CondNode *test2 = getTestNegateLeaf();
	CondNode *test3 = getTestDoubleNegateLeaf();
	test(isEquivalent(test1, test2) == FALSE_BOOL);
	test(isEquivalent(test1, test3) == TRUE_BOOL);
	test(isEquivalent(test2, test3) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
	cleanCondNode(test3);
}

static CondNode *getTestNegateLeaf() {
	return negateNode(getTestLeaf());
}

static CondNode *getTestDoubleNegateLeaf() {
	return negateNode(getTestNegateLeaf());
}

#endif
