#define CONDITION_PACKAGE
#define CONDITION_NODE_MODULE

#include <stdlib.h>
#include "node.h"
#include "tags.h"
#include "function/data.h"
#include "function/info.h"
#include "../testing.h"
#include "../utils/bool.h"
#include "../utils/test.h"

struct CondNode {
	CondNode *chld1, *chld2;
	const OperationTag *_op_tag;
	const NegationTag *_neg_tag;
	FuncData *func_data;
};

static CondNode nil_obj;
CondNode *nil_node = &nil_obj;

CondNode *newCondNode(CondFunc func, char arg1, char arg2,
		const NegationTag *neg_tag) {
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->chld1 = nil_node;
	newNode->chld2 = nil_node;
	newNode->_op_tag = IGNORE_OPERATION;
	newNode->_neg_tag = neg_tag;
	newNode->func_data = newFuncData(func, arg1, arg2);
	return newNode;
}

static CondNode *newNodeBranch(CondNode *node1, CondNode *node2,
		const OperationTag *op_tag) {
	ASSURE(node1 != NULL && node2 != NULL,
		LOG_ERROR("FOUND NULL POINTER");
		cleanCondNode(node1);
		cleanCondNode(node2);
		return nil_node;
	);
	CondNode *newNode = malloc(sizeof (CondNode));
	newNode->chld1 = node1;
	newNode->chld2 = node2;
	newNode->_op_tag = op_tag;
	newNode->_neg_tag = IGNORE_NEGATION;
	newNode->func_data = nil_data;
	return newNode;
}

void cleanCondNode(CondNode *node) {
	if(node == NULL || node == nil_node) {
		return;
	}
	cleanFuncData(node->func_data);
	cleanCondNode(node->chld1);
	cleanCondNode(node->chld2);
	free(node);
}


static CondNode *linkNodes(CondNode *node1, CondNode *node2,
		const OperationTag *op_tag);
static CondNode *branchNodes(CondNode *node1, CondNode *node2,
		const OperationTag *op_tag);

CondNode *mergeNodes(CondNode *node1, CondNode *node2,
		const OperationTag *op_tag) {
	ASSURE(node1 != NULL && node2 != NULL,
		LOG_ERROR("FOUND NULL POINTER");
		cleanCondNode(node1);
		cleanCondNode(node2);
		return nil_node;
	);
	ASSURE(node1 != nil_node && node2 != nil_node,
		LOG_ERROR("CANNOT USE NIL NODES");
		cleanCondNode(node1);
		cleanCondNode(node2);
		return nil_node;
	);
	if(!isValidCond(node1) || !isValidCond(node2)) {
		cleanCondNode(node1);
		cleanCondNode(node2);
		return nil_node;
	}
	if(isLeafCond(node1)) {
		return linkNodes(node1, node2, op_tag);
	} else {
		return branchNodes(node1, node2, op_tag);
	}
}

static CondNode *linkNodes(CondNode *node1, CondNode *node2,
		const OperationTag *op_tag) {
	ASSURE(node1 != NULL && node2 != NULL,
		LOG_ERROR("FOUND NULL POINTER");
		return nil_node;
	);
	CondNode *result = node1;
	result->_op_tag = op_tag;
	result->chld2 = node2;
	return result;
}

static CondNode *branchNodes(CondNode *node1, CondNode *node2,
		const OperationTag *op_tag) {
	ASSURE(node1 != NULL && node2 != NULL,
		LOG_ERROR("FOUND NULL POINTER");
		return nil_node;
	);
	CondNode *result = newNodeBranch(node1, node2, op_tag);
	return result;
}

int evaluateCond(CondNode *node, char input) {
	ASSURE(!isNullCond(node),
		LOG_ERROR("INVALID NODE INPUT");
		return FALSE_BOOL;
	);
	if(isBranchCond(node)) {
		return combineEvals(evaluateCond(node->chld1, input),
			evaluateCond(node->chld2, input), node->_op_tag);
	} else if(isLinkCond(node)) {
		return combineEvals(evaluateFunc(node, input),
			evaluateCond(node->chld2, input), node->_op_tag);
	} else if(isLeafCond(node)) {
		return evaluateFunc(node, input);
	}
	LOG_ERROR("INVALID NODE INPUT");
	return FALSE_BOOL;
}

static int combineEvals(int bool1, int bool2,
		const OperationTag *op_tag) {
	ASSURE((bool1 == TRUE_BOOL || bool1 == FALSE_BOOL) &&
			(bool2 == TRUE_BOOL || bool2 == FALSE_BOOL) &&
			isUsableOperationTag(op_tag),
		LOG_ERROR("INVALID INPUT")
		return FALSE_BOOL;
	);
	if(op_tag == OR_OPERATION) {
		return boolOr(bool1, bool2);
	} else {
		return boolAnd(bool1, bool2);
	}
}

static int evaluateFunc(CondNode *node, char input) {
	ASSURE(node != NULL && node != nil_node &&
			node->func_data != nil_data,
		LOG_ERROR("INVALID INPUT");
		return FALSE_BOOL;
	);
	return boolXor(applyFunc(node->func_data, input),
		isNegated(node));
}

static int isNegated(CondNode *node) {
	ASSURE(!isNullCond(node) &&
			isUsableNegationTag(node->_neg_tag),
		LOG_ERROR("INVALID NODE INPUT");
		return FALSE_BOOL;
	);
	if(node->_neg_tag == YES_NEGATION) {
		return TRUE_BOOL;
	} else {
		return FALSE_BOOL;
	}
}

static int isBranchCond(CondNode *node) {
	return (node->chld1 != nil_node) && (node->chld2 != nil_node);
}

static int isLinkCond(CondNode *node) {
	return (node->chld1 == nil_node) && (node->chld2 != nil_node);
}

static int isLeafCond(CondNode *node) {
	return (node->chld1 == nil_node) && (node->chld2 == nil_node);
}

static int isNullCond(CondNode *node) {
	return node == NULL || node == nil_node;
}

static int isValidCond(CondNode *node) {
	return !isNullCond(node) &&
		(isBranchCond(node) || isLinkCond(node) ||
		isLeafCond(node));
}

int isEquivalent(CondNode *node1, CondNode *node2) {
	ASSURE(node1 != NULL && node2 != NULL,
		LOG_ERROR("NULL POINTER FOUND");
		return FALSE_BOOL;
	)
	if(node1 == node2) {
		return TRUE_BOOL;
	} else if(node1 == nil_node || node2 == nil_node) {
		return FALSE_BOOL;
	} else if(!dataEquivalent(node1, node2)) {
		return FALSE_BOOL;
	}
	return boolAnd(isEquivalent(node1->chld1, node2->chld1),
		isEquivalent(node1->chld2, node2->chld2));
}

static int sharesOpTag(CondNode *node1, CondNode *node2) {
	return (isValidOperationTag(node1->_op_tag) &&
		isValidOperationTag(node2->_op_tag) &&
		node1->_op_tag == node2->_op_tag);
}

static int sharesNegTag(CondNode *node1, CondNode *node2) {
	return (isValidNegationTag(node1->_neg_tag) &&
	isValidNegationTag(node2->_neg_tag) &&
	node1->_neg_tag == node2->_neg_tag);
}

static int dataEquivalent(CondNode *node1, CondNode *node2) {
	ASSURE((node1 != NULL && node2 != NULL &&
			node1 != nil_node && node2 != nil_node),
		LOG_ERROR("SHOULD NOT GET HERE\n");
		abort();
	);
	return sharesOpTag(node1, node2) &&
		sharesNegTag(node1, node2) &&
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

#ifdef TEST_CONDITION_NODE
int main() {
	testNodes();
	return 0;
}
#endif

void testNodes() {
	testPropertyFuncs();
	testMergingFuncs();
	testMergeNodes();
	testFuncEquivalence();
	testDataEquivalence();
	testCondEquivalence();
	testEvalSubFuncs();
	testEvaluateCond();
}

static void testPropertyFuncs() {
	testIsNullCond();
	testIsNegatedCond();
	testIsLeafCond();
	testIsLinkCond();
	testIsBranchCond();
	testIsValidCond();
}

static void testIsNullCond() {
	TEST(isNullCond(NULL));
	TEST(isNullCond(nil_node));

	CondNode *test = newCondNode(isWhitespace, '\0', '\0',
		YES_NEGATION);
	TEST(!isNullCond(test));
	cleanCondNode(test);
}

static void testIsNegatedCond() {
	CondNode *test1 = newCondNode(isWhitespace, '\0', '\0',
		YES_NEGATION);
	TEST(isNegated(test1) == TRUE_BOOL);
	cleanCondNode(test1);

	CondNode *test2 = newCondNode(isWhitespace, '\0', '\0',
		NO_NEGATION);
	TEST(isNegated(test2) == FALSE_BOOL);
	cleanCondNode(test2);

	CondNode *test3 = newCondNode(isWhitespace, '\0', '\0',
		IGNORE_NEGATION);
	TEST(isNegated(test3) == FALSE_BOOL);
	cleanCondNode(test3);

	CondNode *test4 = newCondNode(isWhitespace, '\0', '\0',
		INVALID_NEGATION);
	TEST(isNegated(test4) == FALSE_BOOL);
	cleanCondNode(test4);
}

static void testIsLeafCond() {
	CondNode *test1 = getTestLeafCond();
	TEST(isLeafCond(test1));
	cleanCondNode(test1);

	CondNode *test2 = getTestLinkCond();
	TEST(!isLeafCond(test2));
	cleanCondNode(test2);

	CondNode *test3 = getTestBranchCond();
	TEST(!isLeafCond(test3));
	cleanCondNode(test3);

	CondNode *test4 = getTestMalformedCond();
	TEST(!isLeafCond(test4));
	cleanCondNode(test4);
}

static void testIsLinkCond() {
	CondNode *test1 = getTestLeafCond();
	TEST(!isLinkCond(test1));
	cleanCondNode(test1);

	CondNode *test2 = getTestLinkCond();
	TEST(isLinkCond(test2));
	cleanCondNode(test2);

	CondNode *test3 = getTestBranchCond();
	TEST(!isLinkCond(test3));
	cleanCondNode(test3);

	CondNode *test4 = getTestMalformedCond();
	TEST(!isLinkCond(test4));
	cleanCondNode(test4);
}

static void testIsBranchCond() {
	CondNode *test1 = getTestLeafCond();
	TEST(!isBranchCond(test1));
	cleanCondNode(test1);

	CondNode *test2 = getTestLinkCond();
	TEST(!isBranchCond(test2));
	cleanCondNode(test2);

	CondNode *test3 = getTestBranchCond();
	TEST(isBranchCond(test3));
	cleanCondNode(test3);

	CondNode *test4 = getTestMalformedCond();
	TEST(!isBranchCond(test4));
	cleanCondNode(test4);
}

static void testIsValidCond() {
	CondNode *test1 = getTestLeafCond();
	TEST(isValidCond(test1));
	cleanCondNode(test1);

	CondNode *test2 = getTestLinkCond();
	TEST(isValidCond(test2));
	cleanCondNode(test2);

	CondNode *test3 = getTestBranchCond();
	TEST(isValidCond(test3));
	cleanCondNode(test3);

	CondNode *test4 = getTestMalformedCond();
	TEST(!isValidCond(test4));
	cleanCondNode(test4);

	CondNode *test5 = NULL;
	TEST(!isValidCond(test5));

	CondNode *test6 = nil_node;
	TEST(!isValidCond(test6));
}

static CondNode *getTestLeafCond() {
	return newCondNode(isWordChar, '\0', '\0', NO_NEGATION);
}

static CondNode *getTestLinkCond() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *result = setup1;
	setup1->chld2 = setup2;
	return result;
}

static CondNode *getTestBranchCond() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *result = newCondNode(inRangeChar, 'A', 'z',
		YES_NEGATION);
	result->chld1 = setup1;
	result->chld2 = setup2;
	return result;
}

static CondNode *getTestMalformedCond() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *result = setup1;
	result->chld1 = setup2;
	return result;
}

static void testMergingFuncs() {
	testLinkNodes();
	testBranchNodes();
}

static void testLinkNodes() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *test = linkNodes(setup1, setup2, OR_OPERATION);
	TEST(isLinkCond(test));
	cleanCondNode(test);
}

static void testBranchNodes() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *test = branchNodes(setup1, setup2,
		OR_OPERATION);
	TEST(isBranchCond(test));
	cleanCondNode(test);
}

static void testMergeNodes() {
	testMergeNode1Null();
	testMergeNode2Null();
	testMergeNode1Nil();
	testMergeNode2Nil();
	testMergeNode1Invalid();
	testMergeNode2Invalid();
	testMergeNode1Leaf();
	testMergeNode1NotLeaf();
}

static void testMergeNode1Null() {
	CondNode *test1 = NULL;
	CondNode *test2 = getTestMergeLeafNode();
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(test3 == nil_node);
	cleanCondNode(test3);
}

static void testMergeNode2Null() {
	CondNode *test1 = getTestMergeLeafNode();
	CondNode *test2 = NULL;
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(test3 == nil_node);
	cleanCondNode(test3);
}

static void testMergeNode1Nil() {
	CondNode *test1 = nil_node;
	CondNode *test2 = getTestMergeLeafNode();
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(test3 == nil_node);
	cleanCondNode(test3);
}

static void testMergeNode2Nil() {
	CondNode *test1 = getTestMergeLeafNode();
	CondNode *test2 = nil_node;
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(test3 == nil_node);
	cleanCondNode(test3);
}

static void testMergeNode1Invalid() {
	CondNode *test1 = getTestMergeInvalidNode();
	CondNode *test2 = getTestMergeLeafNode();
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(test3 == nil_node);
	cleanCondNode(test3);
}

static void testMergeNode2Invalid() {
	CondNode *test1 = getTestMergeLeafNode();
	CondNode *test2 = getTestMergeInvalidNode();
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(test3 == nil_node);
	cleanCondNode(test3);
}

static void testMergeNode1Leaf() {
	CondNode *test1 = getTestMergeLeafNode();
	CondNode *test2 = getTestMergeLinkNode();
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(isLinkCond(test3));
	cleanCondNode(test3);
}

static void testMergeNode1NotLeaf() {
	CondNode *test1 = getTestMergeLinkNode();
	CondNode *test2 = getTestMergeLeafNode();
	CondNode *test3 = mergeNodes(test1, test2,
		AND_OPERATION);
	TEST(isBranchCond(test3));
	cleanCondNode(test3);
}

static CondNode *getTestMergeInvalidNode() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *result = setup1;
	result->chld1 = setup2;
	return result;
}

static CondNode *getTestMergeLeafNode() {
	return newCondNode(inRangeChar, 'A', 'z', NO_NEGATION);
}

static CondNode *getTestMergeLinkNode() {
	CondNode *setup1 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	return linkNodes(setup1, setup2, OR_OPERATION);
}

static void testFuncEquivalence() {
	testFuncEqvBothBranchNodes();
	testFuncEqvNode1NotBranch();
	testFuncEqvNode2NotBranch();
	testFuncEqvNeitherBranchNodes();
}

static CondNode* getFuncEqvBranch() {
	CondNode *setup1 = newCondNode(equalsChar, 'a', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'b', '\0',
		NO_NEGATION);
	return branchNodes(setup1, setup2, OR_OPERATION);
}

static CondNode* getFuncEqvNonBranch() {
	CondNode *setup1 = newCondNode(equalsChar, 'a', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'b', '\0',
		NO_NEGATION);
	return linkNodes(setup1, setup2, OR_OPERATION);
}

static void testFuncEqvBothBranchNodes() {
	CondNode *test1 = getFuncEqvBranch();
	CondNode *test2 = getFuncEqvBranch();
	TEST(haveFuncsEquivalent(test1, test2) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testFuncEqvNode1NotBranch() {
	CondNode *test1 = getFuncEqvNonBranch();
	CondNode *test2 = getFuncEqvBranch();
	TEST(haveFuncsEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testFuncEqvNode2NotBranch() {
	CondNode *test1 = getFuncEqvBranch();
	CondNode *test2 = getFuncEqvNonBranch();
	TEST(haveFuncsEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testFuncEqvNeitherBranchNodes() {
	CondNode *test1 = getFuncEqvNonBranch();
	CondNode *test2 = getFuncEqvNonBranch();
	TEST(haveFuncsEquivalent(test1, test2) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testDataEquivalence() {
	testDataEqvUnequalOpTags();
	testDataEqvUnequalNegTags();
	testDataEqvFuncsNotEquivalent();
	testDataEqvNominalCase();
}

static void testDataEqvUnequalOpTags() {
	CondNode *test1 = getDataEqvTargetLink();
	CondNode *test2 = getDataEqvDiffOpLink();
	TEST(!dataEquivalent(test1, test2));
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testDataEqvUnequalNegTags() {
	CondNode *test1 = getDataEqvTargetLink();
	CondNode *test2 = getDataEqvDiffNegLink();
	TEST(!dataEquivalent(test1, test2));
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testDataEqvFuncsNotEquivalent() {
	CondNode *test1 = getDataEqvTargetLink();
	CondNode *test2 = getDataEqvDiffFuncLink();
	TEST(!dataEquivalent(test1, test2));
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testDataEqvNominalCase() {
	CondNode *test1 = getDataEqvTargetLink();
	CondNode *test2 = getDataEqvTargetLink();
	TEST(dataEquivalent(test1, test2));
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static CondNode *getDataEqvTargetLink() {
	CondNode *setup1 = newCondNode(equalsChar, '?', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		NO_NEGATION);
	return linkNodes(setup1, setup2, OR_OPERATION);
}

static CondNode *getDataEqvDiffOpLink() {
	CondNode *setup1 = newCondNode(equalsChar, '?', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		NO_NEGATION);
	return linkNodes(setup1, setup2, AND_OPERATION);
}

static CondNode *getDataEqvDiffNegLink() {
	CondNode *setup1 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		NO_NEGATION);
	return linkNodes(setup1, setup2, OR_OPERATION);
}

static CondNode *getDataEqvDiffFuncLink() {
	CondNode *setup1 = newCondNode(equalsChar, '!', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		NO_NEGATION);
	return linkNodes(setup1, setup2, OR_OPERATION);
}

static void testCondEquivalence() {
	testCondEqvNode1IsNull();
	testCondEqvNode2IsNull();
	testCondEqvNode1SameAsNode2();
	testCondEqvNode1IsNil();
	testCondEqvNode2IsNil();
	testCondEqvDataEqvFalse();
	testCondEqvChild1NotEqv();
	testCondEqvChild2NotEqv();
	testCondEqvNodesEqv();
}

static void testCondEqvNode1IsNull() {
	CondNode *test1 = NULL;
	CondNode *test2 = getCondEqvTargetBranch();
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvNode2IsNull() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = NULL;
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvNode1SameAsNode2() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = test1;
	TEST(isEquivalent(test1, test2) == TRUE_BOOL);
	cleanCondNode(test1);
}

static void testCondEqvNode1IsNil() {
	CondNode *test1 = nil_node;
	CondNode *test2 = getCondEqvTargetBranch();
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvNode2IsNil() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = nil_node;
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvDataEqvFalse() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = getCondEqvDiffData();
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvChild1NotEqv() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = getCondEqvDiffChild1();
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvChild2NotEqv() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = getCondEqvDiffChild2();
	TEST(isEquivalent(test1, test2) == FALSE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static void testCondEqvNodesEqv() {
	CondNode *test1 = getCondEqvTargetBranch();
	CondNode *test2 = getCondEqvTargetBranch();
	TEST(isEquivalent(test1, test2) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(test2);
}

static CondNode *getCondEqvTargetBranch() {
	CondNode *setup1 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	return branchNodes(setup1, setup2, AND_OPERATION);
}

static CondNode *getCondEqvDiffData() {
	CondNode *setup1 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	return branchNodes(setup1, setup2, OR_OPERATION);
}

static CondNode *getCondEqvDiffChild1() {
	CondNode *setup1 = newCondNode(equalsChar, '!', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	return branchNodes(setup1, setup2, AND_OPERATION);
}

static CondNode *getCondEqvDiffChild2() {
	CondNode *setup1 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, '!', '\0',
		YES_NEGATION);
	return branchNodes(setup1, setup2, AND_OPERATION);
}

static void testEvalSubFuncs() {
	testCombineEvalsBool1Invalid();
	testCombineEvalsBool2Invalid();
	testCombineEvalsOpTagInvalid();
	testEvaluateFuncNodeIsNull();
	testEvaluateFuncNodeIsNil();
}

static void testCombineEvalsBool1Invalid() {
	TEST(combineEvals(-1, TRUE_BOOL, OR_OPERATION) ==
		FALSE_BOOL);
}

static void testCombineEvalsBool2Invalid() {
	TEST(combineEvals(TRUE_BOOL, -1, OR_OPERATION) ==
		FALSE_BOOL);
}

static void testCombineEvalsOpTagInvalid() {
	TEST(combineEvals(TRUE_BOOL, TRUE_BOOL,
		IGNORE_OPERATION) == FALSE_BOOL);
	TEST(combineEvals(TRUE_BOOL, TRUE_BOOL,
		INVALID_OPERATION) == FALSE_BOOL);
}

static void testEvaluateFuncNodeIsNull() {
	TEST(evaluateFunc(NULL, '!') == FALSE_BOOL);
}

static void testEvaluateFuncNodeIsNil() {
	TEST(evaluateFunc(nil_node, '!') == FALSE_BOOL);
}

static void testEvaluateCond() {
	testEvaluateCondBranch();
	testEvaluateCondLink();
	testEvaluateCondLeaf();
	testEvaluateCondInvalid();
}

static void testEvaluateCondLeaf() {
	CondNode *test = getTestEvalLeafCond();
	TEST(evaluateCond(test, ' ') == TRUE_BOOL);
	TEST(evaluateCond(test, '5') == FALSE_BOOL);
	cleanCondNode(test);
}

static void testEvaluateCondLink() {
	CondNode *test = getTestEvalLinkCond();
	TEST(evaluateCond(test, 'B') == TRUE_BOOL);
	TEST(evaluateCond(test, '8') == FALSE_BOOL);
	cleanCondNode(test);
}

static void testEvaluateCondBranch() {
	CondNode *test = getTestEvalBranchCond();
	TEST(evaluateCond(test, '\t') == TRUE_BOOL);
	TEST(evaluateCond(test, '3') == FALSE_BOOL);
	cleanCondNode(test);
}

static void testEvaluateCondInvalid() {
	CondNode *test = getTestEvalInvalidCond();
	TEST(evaluateCond(test, 'a') == FALSE_BOOL);
	TEST(evaluateCond(test, 'Z') == FALSE_BOOL);
	cleanCondNode(test);
}

static CondNode *getTestEvalLeafCond() {
	return newCondNode(isWordChar, '\0', '\0', YES_NEGATION);
}

static CondNode *getTestEvalLinkCond() {
	CondNode *setup1 = newCondNode(isWhitespace, '\0', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		YES_NEGATION);
	return mergeNodes(setup1, setup2, AND_OPERATION);
}

static CondNode *getTestEvalBranchCond() {
	CondNode *setup1 = newCondNode(isWhitespace, '\0', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup3 = mergeNodes(setup1, setup2,
		OR_OPERATION);
	CondNode *setup4 = newCondNode(equalsChar, '3', '\0',
		YES_NEGATION);
	return mergeNodes(setup3, setup4, AND_OPERATION);
}

static CondNode *getTestEvalInvalidCond() {
	return newCondNode(inRangeChar, 'a', 'Z', NO_NEGATION);
}
