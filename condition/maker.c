#define CONDITION_PACKAGE
#define TEST
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "function/info.h"
#include "../utils/bool.h"

//condition.h
typedef struct CondNode CondNode;
typedef struct CondMakr CondMakr;
typedef int CondFunc(char, char, char);
static const int OR_OP = 1;
static const int AND_OP = 0;
static const int IGNORE_OP = -1;
static const int YES_NEGATE = 1;
static const int NO_NEGATE = 0;
static const int IGNORE_NEGATE = -1;

struct CondMakr {
	CondMakr *parent;
	CondNode *result;
	CondNode *arg1, *arg2;
	int op_flag;
};

static CondMakr nil_condmakr_obj;
static CondMakr *nil_makr = &nil_condmakr_obj;

static CondMakr *newCondMakr (CondMakr *parent, int op_flag);
static CondMakr *initializeMakr();
static CondMakr *simplifyMakr(CondMakr *makr);
static CondMakr *addNodeSimple(CondNode *node, CondMakr *makr);
static CondMakr *addNodeToMakr(CondNode *node, CondMakr *makr);
static CondMakr *addOr(CondMakr *makr);
static CondMakr *addAnd(CondMakr *makr);
static CondMakr *addNewCond(CondMakr *makr,
		CondFunc func, char arg1, char arg2, int neg_flag);
static CondMakr *addCond(CondMakr *makr, CondNode *node);

static CondMakr *newCondMakr (CondMakr *parent, int op_flag) {
	CondMakr *newMakr = malloc(sizeof (CondMakr));
	newMakr->op_flag = op_flag;
	newMakr->parent = parent;
	newMakr->result = nil_node;
	newMakr->arg1 = nil_node;
	newMakr->arg2 = nil_node;
	return newMakr;
}

static CondMakr *initializeMakr() {
	return newCondMakr(nil_makr, IGNORE_OP);
}

static int isRootMakr(CondMakr *makr) {
	return makr->op_flag == IGNORE_OP;
}

static int makrHasResult(CondMakr *makr) {
	return makr->result != nil_node;
}

static CondNode *makeMergedNode(CondMakr *makr) {
	return mergeNodes(makr->arg1, makr->arg2, makr->op_flag);
}

static CondMakr *passNodeToParent(CondMakr *makr) {
	CondMakr *parent = makr->parent;
	CondMakr *child = makr;
	parent = addNodeSimple(makeMergedNode(child), parent);
	free(child);
	return parent;
}

static CondMakr *simplifyMakr(CondMakr *makr) {
	if(makr == nil_makr) return nil_makr;
	if(makrHasResult(makr)) return makr;

	CondMakr *result = makr;
	while(!isRootMakr(result) && result->arg2 != nil_node) {
		result = passNodeToParent(result);
	}
	return result;
}

static CondMakr *addNodeSimple(CondNode *node, CondMakr *makr) {
	if(isRootMakr(makr)) {
		makr->result = node;
		return makr;
	}

	if(makr->arg1 == nil_node) makr->arg1 = node;
	else makr->arg2 = node;

	return makr;
}

static CondMakr *addNodeToMakr(CondNode *node, CondMakr *makr) {
	if(node == nil_node) {
		fprintf(stderr, "ERROR: CANNOT ADD A NULL NODE "
			"TO A MAKER\n");
		return makr;
	} if(makr == nil_makr) {
		fprintf(stderr, "ERROR: CANNOT ADD A NODE TO "
			"A NULL MAKER\n");
		return nil_makr;
	}
	
	CondMakr *result = makr;
	result = addNodeSimple(node, result);
	result = simplifyMakr(result);
	return result;
}

static CondMakr *addOr(CondMakr *makr) {
	return newCondMakr(makr, OR_OP);
}

static CondMakr *addAnd(CondMakr *makr) {
	return newCondMakr(makr, AND_OP);
}

static CondMakr *addNewCond(CondMakr *makr,
		CondFunc func, char arg1, char arg2, int neg_flag) {
	if(makr == nil_makr) {
		printf("ERROR: NEED A CONDITION-MAKER\n");
		return nil_makr;
	}
	if(makrHasResult(makr)) {
		fprintf(stderr, "ERROR: CANNOT ADD NEW COND TO MAKER\n");
		return makr;
	}

	CondNode *newNode = newNodeLeaf(func, arg1, arg2);
	if(neg_flag == YES_NEGATE) {
		newNode = negateNode(newNode);
	}
	return addCond(makr, newNode);
}

static CondMakr *addCond(CondMakr *makr, CondNode *node) {
	if(makr == nil_makr) {
		printf("ERROR: NEED A CONDITION-MAKER\n");
		return nil_makr;
	}

	return addNodeToMakr(node, makr);
}

static void cleanMakr(CondMakr *makr) {
	CondMakr *cur_makr = makr;
	while(cur_makr != nil_makr) {
		cleanCondNode(cur_makr->arg1);
		cleanCondNode(cur_makr->arg2);
		CondMakr *parent = cur_makr->parent;
		free(cur_makr);
		cur_makr = parent;
	}
}

static CondNode *finalizeMakr(CondMakr *makr) {
	if(!makrHasResult(makr)) {
		fprintf(stderr, "ERROR: MAKER IS INVALID FOR FINALIZING\n");
		return nil_node;
	}
	CondNode *result = makr->result;
	cleanMakr(makr);
	return result;
}

static void testMakrLeaf();
static void testMakrLink();
static void testMakrBranch();

static CondNode *getTestMakrOrLink();
static CondNode *getTestMakrOrBranch();
static CondNode *getTestMakrAndLink();
static CondNode *getTestMakrAndBranch();
#define test(x) printf("%s:%d TEST `%s\' %s\n", __FILE__,\
	__LINE__, #x, (x)? "PASSED": "FAILED");

#ifdef TEST
int main() {
	testNodes();
	testMakrLeaf();
	testMakrLink();
	testMakrBranch();

	CondMakr *test1 = initializeMakr();
	CondMakr *test2 = addOr(test1);
	CondMakr *test3 = addAnd(test2);
	CondMakr *test4 = addNewCond(test3, isWhitespace,
		'\0', '\0', NO_NEGATE);
	cleanMakr(test4);
	return 0;
}
#endif

static void testMakrLeaf() {
	CondMakr *test_makr = initializeMakr();
	test_makr = addNewCond(test_makr, isWordChar, '\0', '\0',
		NO_NEGATE);
	CondNode *test_node = finalizeMakr(test_makr);
	CondNode *expected_node = newNodeLeaf(isWordChar, '\0', '\0');
	test(isEquivalent(test_node, expected_node)
		== TRUE_BOOL);
	cleanCondNode(test_node);
	cleanCondNode(expected_node);
}

static void testMakrLink() {
	CondNode *test_node_link = getTestMakrOrLink();
	CondNode *test_node_branch = getTestMakrOrBranch();
	CondNode *setup1 = newNodeLeaf(inRangeChar, 'A', 'Z');
	CondNode *setup2 = newNodeLeaf(inRangeChar, 'a', 'z');
	CondNode *setup3 = mergeNodes(setup2, setup1, OR_OP);
	CondNode *setup4 = newNodeLeaf(equalsChar, ' ', '\0');
	CondNode *expected_node = mergeNodes(setup4, setup3, OR_OP);
	test(isEquivalent(test_node_link, expected_node) == TRUE_BOOL);
	test(isEquivalent(test_node_branch, expected_node) ==
		FALSE_BOOL);
	cleanCondNode(test_node_link);
	cleanCondNode(test_node_branch);
	cleanCondNode(expected_node);
}

static CondNode *getTestMakrOrLink() {
	CondMakr *test_makr = initializeMakr();
	test_makr = addOr(test_makr);
	test_makr = addNewCond(test_makr, equalsChar, ' ', '\0',
		NO_NEGATE);
	test_makr = addOr(test_makr);
	test_makr = addNewCond(test_makr, inRangeChar, 'a', 'z',
		NO_NEGATE);
	test_makr = addNewCond(test_makr, inRangeChar, 'A', 'Z',
		NO_NEGATE);
	return finalizeMakr(test_makr);
}

static CondNode *getTestMakrOrBranch() {
	CondMakr *test_makr = initializeMakr();
	test_makr = addOr(test_makr);
	test_makr = addOr(test_makr);
	test_makr = addNewCond(test_makr, equalsChar, ' ', '\0',
		NO_NEGATE);
	test_makr = addNewCond(test_makr, inRangeChar, 'a', 'z',
		NO_NEGATE);
	test_makr = addNewCond(test_makr, inRangeChar, 'A', 'Z',
		NO_NEGATE);
	return finalizeMakr(test_makr);
}

static void testMakrBranch() {
	CondNode *test_node_link = getTestMakrAndLink();
	CondNode *test_node_branch = getTestMakrAndBranch();
	CondNode *setup1 = newNodeLeaf(isWordChar, '\0', '\0');
	CondNode *setup2 = newNodeLeaf(inRangeChar, 'P', 's');
	CondNode *setup3 = newNodeLeaf(inRangeChar, 'G', 'a');
	CondNode *setup4 = mergeNodes(setup3, setup2, AND_OP);
	CondNode *expected_node = mergeNodes(setup4, setup1, AND_OP);
	test(isEquivalent(test_node_link, expected_node) ==
		FALSE_BOOL);
	test(isEquivalent(test_node_branch, expected_node) ==
		TRUE_BOOL);
	cleanCondNode(test_node_link);
	cleanCondNode(test_node_branch);
	cleanCondNode(expected_node);
}

static CondNode *getTestMakrAndLink() {
	CondMakr *test_makr = initializeMakr();
	test_makr = addAnd(test_makr);
	test_makr = addNewCond(test_makr, inRangeChar, 'G', 'a',
		NO_NEGATE);
	test_makr = addAnd(test_makr);
	test_makr = addNewCond(test_makr, inRangeChar, 'P', 's',
		NO_NEGATE);
	test_makr = addNewCond(test_makr, isWordChar, '\0', '\0',
		NO_NEGATE);
	return finalizeMakr(test_makr);
}

static CondNode *getTestMakrAndBranch() {
	CondMakr *test_makr = initializeMakr();
	test_makr = addAnd(test_makr);
	test_makr = addAnd(test_makr);
	test_makr = addNewCond(test_makr, inRangeChar, 'G', 'a',
		NO_NEGATE);
	test_makr = addNewCond(test_makr, inRangeChar, 'P', 's',
		NO_NEGATE);
	test_makr = addNewCond(test_makr, isWordChar, '\0', '\0',
		NO_NEGATE);
	return finalizeMakr(test_makr);
}
