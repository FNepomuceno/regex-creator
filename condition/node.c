#define CONDITION_PACKAGE
#define CONDITION_NODE_MODULE
#define TEST

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "node_module.h"
#include "function/data.h"
#include "function/info.h"
#include "../utils/bool.h"

struct CondNode {
	CondNode *chld1, *chld2;
	int op_flag;
	int neg_flag;
	FuncData *func_data;
};

static CondNode nil_obj;
static CondNode *nil = &nil_obj;

static CondNode *newNodeLeaf(CondFunc func, char arg1,
		char arg2) {
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->func_data = newFuncData(func, arg1, arg2);
	newNode->chld1 = nil;
	newNode->chld2 = nil;
	newNode->op_flag = IGNORE_OP;
	newNode->neg_flag = NO_NEGATE;
	return newNode;
}

static CondNode *newNodeBranch(CondNode *node1, CondNode *node2,
		int op_flag) {
	assert(node1 != NULL && node2 != NULL);
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->func_data = nil_data;
	newNode->op_flag = op_flag;
	newNode->neg_flag = IGNORE_NEGATE;
	newNode->chld1 = node1;
	newNode->chld2 = node2;
	return newNode;
}

void cleanCondNode(CondNode *node) {
	assert(node != NULL);
	if(node == nil) {
		return;
	}
	cleanFuncData(node->func_data);
	cleanCondNode(node->chld1);
	cleanCondNode(node->chld2);
	free(node);
}

CondNode *mergeNodes(CondNode *node1, CondNode *node2,
		int op_flag) {	
	assert(node1 != NULL && node2 != NULL);
	if(node1 == nil || node2 == nil) {
		fprintf(stderr, "ERROR: CANNOT COMBINE NIL NODES\n");
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

CondNode *negateNode(CondNode *node) {
	assert(node != NULL);
	assert(node->neg_flag == YES_NEGATE ||
		node->neg_flag == NO_NEGATE);
	if(node->neg_flag == NO_NEGATE) {
		node->neg_flag = YES_NEGATE;
	} else {
		node->neg_flag = NO_NEGATE;
	}
	return node;
}

int evaluateCond(CondNode *node, char input) {
	assert(node != NULL);
	if(isBranchCond(node)) {
		return combineEvals(evaluateCond(node->chld1, input),
			evaluateCond(node->chld2, input), node->op_flag);
	} else if(isLinkCond(node)) {
		return combineEvals(evaluateFunc(node, input),
			evaluateCond(node->chld2, input), node->op_flag);
	} else {
		return evaluateFunc(node, input);
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

static int evaluateFunc(CondNode *node, char input) {
	assert(node != NULL && node != nil);
	return boolXor(applyFunc(node->func_data, input),
		isNegated(node));
}

static int isNegated(CondNode *node) {
	assert(node != NULL && node != nil);
	int negate_flag = node->neg_flag;
	assert(negate_flag == YES_NEGATE || negate_flag == NO_NEGATE);
	if(negate_flag == YES_NEGATE) {
		return TRUE_BOOL;
	} else {
		return FALSE_BOOL;
	}
}

static int isBranchCond(CondNode *node) {
	assert(node != NULL && node != nil);
	return (node->chld1 != nil) && (node->chld2 != nil);
}

static int isLinkCond(CondNode *node) {
	assert(node != NULL && node != nil);
	return (node->chld1 == nil) && (node->chld2 != nil);
}

static int isLeafCond(CondNode *node) {
	assert(node != NULL && node != nil);
	return (node->chld1 == nil) && (node->chld2 == nil);
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
	return node1->op_flag == node2->op_flag &&
		node1->neg_flag == node2->neg_flag &&
		haveFuncsEquivalent(node1, node2) == TRUE_BOOL;
}

static int haveFuncsEquivalent(CondNode *node1, CondNode *node2) {
	if(isBranchCond(node1) && isBranchCond(node2)) {
		return TRUE_BOOL;
	}
	if(!isBranchCond(node1) && !isBranchCond(node2)) {
		return toBool(areFuncsEquivalent(node1->func_data,
			node2->func_data));
	}
	return FALSE_BOOL;
}

#ifdef TEST
#include "node_test.h"

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
	testCondMerge();
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
	testMismatchEquivalence();
	testNegateEquivalence();
	printf("\n");
}

static void testCondMerge() {
	testLeafMerge();
	testLinkMerge();
	testBranchMerge();
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
	test(evaluateCond(test, '5') == TRUE_BOOL);
	test(evaluateCond(test, '#') == FALSE_BOOL);
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
	test(evaluateCond(test, '$') == FALSE_BOOL);
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

static void testMismatchEquivalence() {
	CondNode *test1 = getTestLeaf();
	CondNode *test2 = getTestOrLink();
	CondNode *test3 = getTestBranch();
	test(isEquivalent(test1, test2) == FALSE_BOOL);
	test(isEquivalent(test1, test3) == FALSE_BOOL);
	test(isEquivalent(test2, test3) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
	cleanCondNode(test3);
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

static void testLeafMerge() {
	CondNode *test1 = getTestMergerLeaf();
	CondNode *test2 = getTestMergeeLeaf();
	CondNode *test3 = mergeNodes(test1, test2, OR_OP);
	CondNode *test4 = getTestLeafToLeafMerge();
	test(isEquivalent(test3, test4) == TRUE_BOOL);
	cleanCondNode(test3);
	cleanCondNode(test4);
}

static CondNode *getTestMergerLeaf() {
	return newNodeLeaf(equalsChar, ' ', 0);
}

static CondNode *getTestMergeeLeaf() {
	return newNodeLeaf(equalsChar, '\t', 0);
}

static CondNode *getTestLeafToLeafMerge() {
	CondNode *test1 = getTestMergerLeaf();
	CondNode *test2 = getTestMergeeLeaf();
	return linkNodes(test1, test2, OR_OP);
}

static void testLinkMerge() {
	CondNode *test1 = getTestMergerLeaf();
	CondNode *test2 = getTestMergeeLink();
	CondNode *test3 = mergeNodes(test1, test2, OR_OP);
	CondNode *test4 = getTestLeafToLinkMerge();
	cleanCondNode(test3);
	cleanCondNode(test4);

	CondNode *test5 = getTestMergerLink();
	CondNode *test6 = getTestMergeeLeaf();
	CondNode *test7 = mergeNodes(test5, test6, OR_OP);
	CondNode *test8 = getTestLinkToLeafMerge();
	cleanCondNode(test7);
	cleanCondNode(test8);

	CondNode *test9 = getTestMergerLink();
	CondNode *test10 = getTestMergeeLink();
	CondNode *test11 = mergeNodes(test9, test10, OR_OP);
	CondNode *test12 = getTestLinkToLinkMerge();
	cleanCondNode(test11);
	cleanCondNode(test12);
}

static CondNode *getTestMergerLink() {
	CondNode *test1 = newNodeLeaf(equalsChar, '8', 0);
	CondNode *test2 = newNodeLeaf(equalsChar, '0', 0);
	return linkNodes(test1, test2, OR_OP);
}

static CondNode *getTestMergeeLink() {
	CondNode *test1 = newNodeLeaf(equalsChar, 'G', 0);
	CondNode *test2 = newNodeLeaf(equalsChar, 'O', 0);
	return linkNodes(test1, test2, OR_OP);
}

static CondNode *getTestLeafToLinkMerge() {
	CondNode *test1 = getTestMergerLeaf();
	CondNode *test2 = getTestMergeeLink();
	return linkNodes(test1, test2, OR_OP);
}

static CondNode *getTestLinkToLeafMerge() {
	CondNode *test1 = getTestMergerLink();
	CondNode *test2 = getTestMergeeLeaf();
	return branchNodes(test1, test2, OR_OP);
}

static CondNode *getTestLinkToLinkMerge() {
	CondNode *test1 = getTestMergerLink();
	CondNode *test2 = getTestMergeeLink();
	return branchNodes(test1, test2, OR_OP);
}

static void testBranchMerge() {
	CondNode *test1 = getTestMergerLeaf();
	CondNode *test2 = getTestMergeeBranch();
	CondNode *test3 = mergeNodes(test1, test2, OR_OP);
	CondNode *test4 = getTestLeafToBranchMerge();
	cleanCondNode(test3);
	cleanCondNode(test4);

	CondNode *test5 = getTestMergerLink();
	CondNode *test6 = getTestMergeeBranch();
	CondNode *test7 = mergeNodes(test5, test6, OR_OP);
	CondNode *test8 = getTestLinkToBranchMerge();
	cleanCondNode(test7);
	cleanCondNode(test8);

	CondNode *test9 = getTestMergerBranch();
	CondNode *test10 = getTestMergeeLeaf();
	CondNode *test11 = mergeNodes(test9, test10, OR_OP);
	CondNode *test12 = getTestBranchToLeafMerge();
	cleanCondNode(test11);
	cleanCondNode(test12);

	CondNode *test13 = getTestMergerBranch();
	CondNode *test14 = getTestMergeeLink();
	CondNode *test15 = mergeNodes(test13, test14, OR_OP);
	CondNode *test16 = getTestBranchToLinkMerge();
	cleanCondNode(test15);
	cleanCondNode(test16);

	CondNode *test17 = getTestMergerBranch();
	CondNode *test18 = getTestMergeeBranch();
	CondNode *test19 = mergeNodes(test17, test18, OR_OP);
	CondNode *test20 = getTestBranchToBranchMerge();
	cleanCondNode(test19);
	cleanCondNode(test20);
}

static CondNode *getTestMergerBranch() {
	CondNode *test1 = newNodeLeaf(equalsChar, '!', 0);
	CondNode *test2 = newNodeLeaf(equalsChar, '?', 0);
	return branchNodes(test1, test2, OR_OP);
}

static CondNode *getTestMergeeBranch() {
	CondNode *test1 = newNodeLeaf(equalsChar, '[', 0);
	CondNode *test2 = newNodeLeaf(equalsChar, '}', 0);
	return branchNodes(test1, test2, OR_OP);
}

static CondNode *getTestLeafToBranchMerge() {
	CondNode *test1 = getTestMergerLeaf();
	CondNode *test2 = getTestMergeeBranch();
	return linkNodes(test1, test2, OR_OP);
}

static CondNode *getTestLinkToBranchMerge() {
	CondNode *test1 = getTestMergerLink();
	CondNode *test2 = getTestMergeeBranch();
	return branchNodes(test1, test2, OR_OP);
}

static CondNode *getTestBranchToLeafMerge() {
	CondNode *test1 = getTestMergerBranch();
	CondNode *test2 = getTestMergeeLeaf();
	return branchNodes(test1, test2, OR_OP);
}

static CondNode *getTestBranchToLinkMerge() {
	CondNode *test1 = getTestMergerBranch();
	CondNode *test2 = getTestMergeeLink();
	return branchNodes(test1, test2, OR_OP);
}

static CondNode *getTestBranchToBranchMerge() {
	CondNode *test1 = getTestMergerBranch();
	CondNode *test2 = getTestMergeeBranch();
	return branchNodes(test1, test2, OR_OP);
}

#endif
