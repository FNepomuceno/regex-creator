#define CONDITION_PACKAGE
#define CONDITION_NODE_MOD

#include <stdlib.h>
#include "../testing.h"
#include "node.h"
#include "tags.h"
#include "function/data.h"
#include "function/info.h"
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

int evaluateFunc(CondNode *node, char input) {
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
		LOG_ERROR("INVALID INPUT");
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
		LOG_ERROR("CRITICAL ERROR");
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

#ifdef TESTING_CONDITION_NODE
#ifdef TEST_THIS
int main() {
	USE_CASE(Node);
	return 0;
}
#endif

TEST_CASE(Node) {
	USE_CASE(NullEquivalence);
	USE_CASE(NilNodeEquivalence);
	USE_CASE(SameNodeEquivalence);
	USE_CASE(DiffArgsEquivalence);
	USE_CASE(DiffFuncEquivalence);
	USE_CASE(DiffOperationTagEquivalence);
	USE_CASE(DiffNegationTagEquivalence);
	USE_CASE(LinkWithLeafEquivalence);
	USE_CASE(LeafWithLinkEquivalence);
	USE_CASE(LinkWithBranchEquivalence);
	USE_CASE(BranchWithLinkEquivalence);

	USE_CASE(LeafEvaluation);
	USE_CASE(LinkEvaluation);
	USE_CASE(BranchEvaluation);
}

static TEST_CASE(NullEquivalence) {
	COMPARE_TEST(CondNode *, NullToNullEquivalence,
		NULL,
		NULL,
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, NullToValidEquivalence,
		NULL,
		USE_RES(EqvLeafCondNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, ValidToNullEquivalence,
		USE_RES(EqvLeafCondNode,),
		NULL,
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, EqvLeafCondNode, void) {
	return newCondNode(equalsChar, '%', '\0', NO_NEGATION);
}

static TEST_CASE(NilNodeEquivalence) {
	COMPARE_TEST(CondNode *, NilToNilEquivalence,
		nil_node,
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, NilToValidEquivalence,
		nil_node,
		USE_RES(EqvLeafCondNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, ValidToNilEquivalence,
		USE_RES(EqvLeafCondNode,),
		nil_node,
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_CASE(SameNodeEquivalence) {
	COMPARE_TEST(CondNode *, SameNodeEquivalence,
		USE_RES(EqvLeafCondNode,),
		USE_RES(EqvLeafCondNode,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_CASE(DiffArgsEquivalence) {
	COMPARE_TEST(CondNode *, DiffArgsEquivalence,
		USE_RES(EqvLeafCondNode,),
		USE_RES(EqvDiffArgsLeafNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, EqvDiffArgsLeafNode, void) {
	return newCondNode(equalsChar, '#', '\0', NO_NEGATION);
}

static TEST_CASE(DiffFuncEquivalence) {
	COMPARE_TEST(CondNode *, DiffFuncEquivalence,
		USE_RES(EqvLinkCondNode,),
		USE_RES(EqvDiffFuncLinkNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, EqvLinkCondNode, void) {
	CondNode *setup1 = newCondNode(equalsChar, '%', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	return mergeNodes(setup1, setup2, OR_OPERATION);
}

static TEST_RES(CondNode *, EqvDiffFuncLinkNode, void) {
	CondNode *setup1 = newCondNode(equalsChar, '%', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	return mergeNodes(setup1, setup2, OR_OPERATION);
}

static TEST_CASE(DiffOperationTagEquivalence) {
	COMPARE_TEST(CondNode *, DiffFuncEquivalence,
		USE_RES(EqvLinkCondNode,),
		USE_RES(EqvDiffOpTagLinkNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, EqvDiffOpTagLinkNode, void) {
	CondNode *setup1 = newCondNode(equalsChar, '%', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	return mergeNodes(setup1, setup2, AND_OPERATION);
}

static TEST_CASE(DiffNegationTagEquivalence) {
	COMPARE_TEST(CondNode *, DiffNegTagEquivalence,
		USE_RES(EqvLeafCondNode,),
		USE_RES(EqvDiffNegTagLeafNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, EqvDiffNegTagLeafNode, void) {
	return newCondNode(equalsChar, '%', '\0', YES_NEGATION);
}

static TEST_CASE(LinkWithLeafEquivalence) {
	COMPARE_TEST(CondNode *, LinkWithLeafEquivalence,
		USE_RES(EqvLinkCondNode,),
		USE_RES(EqvLeafCondNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_CASE(LeafWithLinkEquivalence) {
	COMPARE_TEST(CondNode *, LeafWithLinkEquivalence,
		USE_RES(EqvLeafCondNode,),
		USE_RES(EqvLinkCondNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_CASE(LinkWithBranchEquivalence) {
	COMPARE_TEST(CondNode *, LinkWithBranchEquivalence,
		USE_RES(EqvLinkCondNode,),
		USE_RES(EqvBranchCondNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, EqvBranchCondNode, void) {
	CondNode *setup1 = newCondNode(equalsChar, '%', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(isDigitChar, '\0', '\0',
		NO_NEGATION);
	CondNode *setup3 = mergeNodes(setup1, setup2, OR_OPERATION);
	CondNode *setup4 = newCondNode(isWordChar, '\0', '\0',
		NO_NEGATION);
	return mergeNodes(setup3, setup4, AND_OPERATION);
}

static TEST_CASE(BranchWithLinkEquivalence) {
	COMPARE_TEST(CondNode *, BranchWithLinkEquivalence,
		USE_RES(EqvBranchCondNode,),
		USE_RES(EqvLinkCondNode,),
		isEquivalent, FALSE_BOOL,
		cleanCondNode);
}

static TEST_CASE(LeafEvaluation) {
	CondNode *testLeafEvaluation = USE_RES(EvlLeafCondNode,);
	TEST(evaluateCond(testLeafEvaluation, 'G') == TRUE_BOOL);
	TEST(evaluateCond(testLeafEvaluation, '}') == TRUE_BOOL);
	TEST(evaluateCond(testLeafEvaluation, 'p') == FALSE_BOOL);
	cleanCondNode(testLeafEvaluation);
}

static TEST_RES(CondNode *, EvlLeafCondNode, void) {
	return newCondNode(inRangeChar, 'a', 'z', YES_NEGATION);
}

static TEST_CASE(LinkEvaluation) {
	CondNode *testLinkEvaluation = USE_RES(EvlLinkCondNode,);
	TEST(evaluateCond(testLinkEvaluation, 'a') == TRUE_BOOL);
	TEST(evaluateCond(testLinkEvaluation, 'b') == TRUE_BOOL);
	TEST(evaluateCond(testLinkEvaluation, 'c') == TRUE_BOOL);
	TEST(evaluateCond(testLinkEvaluation, 'd') == FALSE_BOOL);
	cleanCondNode(testLinkEvaluation);
}

static TEST_RES(CondNode *, EvlLinkCondNode, void) {
	CondNode *setup1 = newCondNode(equalsChar, 'a', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'b', '\0',
		NO_NEGATION);
	CondNode *setup3 = newCondNode(equalsChar, 'c', '\0',
		NO_NEGATION);
	CondNode *setup4 = mergeNodes(setup1, setup2, OR_OPERATION);
	return mergeNodes(setup3, setup4, OR_OPERATION);
}

static TEST_CASE(BranchEvaluation) {
	CondNode *testBranchEvaluation = USE_RES(EvlBranchCondNode,);
	TEST(evaluateCond(testBranchEvaluation, 'a') == TRUE_BOOL);
	TEST(evaluateCond(testBranchEvaluation, 'b') == TRUE_BOOL);
	TEST(evaluateCond(testBranchEvaluation, 'c') == TRUE_BOOL);
	TEST(evaluateCond(testBranchEvaluation, 'd') == FALSE_BOOL);
	cleanCondNode(testBranchEvaluation);
}

static TEST_RES(CondNode *, EvlBranchCondNode, void) {
	CondNode *setup1 = newCondNode(equalsChar, 'a', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'b', '\0',
		NO_NEGATION);
	CondNode *setup3 = mergeNodes(setup1, setup2, OR_OPERATION);
	CondNode *setup4 = newCondNode(equalsChar, 'c', '\0',
		NO_NEGATION);
	return mergeNodes(setup3, setup4, OR_OPERATION);
}
#endif
