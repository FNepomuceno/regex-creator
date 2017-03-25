#define CONDITION_PACKAGE
//#define TEST_CONDITION_MAKER
#include <stdlib.h>

#include "node.h"
#include "tags.h"
#include "function/info.h"
#include "../utils/bool.h"
#include "../utils/test.h"

typedef struct CondNode CondNode;
typedef struct CondMakr CondMakr;
typedef int CondFunc(char, char, char);

struct CondMakr {
	CondMakr *parent;
	CondNode *result;
	CondNode *arg1, *arg2;
	const OperationTag *_op_tag;
};

static CondMakr nil_condmakr_obj;
CondMakr *nil_makr = &nil_condmakr_obj;

static CondMakr *newCondMakr(CondMakr *parent,
		const OperationTag *op_tag);
static CondMakr *simplifyMakr(CondMakr *makr);
static CondMakr *addNodeSimple(CondNode *node, CondMakr *makr);
static CondMakr *addNodeToMakr(CondNode *node, CondMakr *makr);
CondMakr *addOr(CondMakr *makr, CondNode *node);
CondMakr *addAnd(CondMakr *makr, CondNode *node);
CondMakr *addCond(CondMakr *makr, CondNode *node);

static CondMakr *newCondMakr(CondMakr *parent,
		const OperationTag *op_tag) {
	CondMakr *newMakr = malloc(sizeof (CondMakr));
	newMakr->parent = parent;
	newMakr->result = nil_node;
	newMakr->arg1 = nil_node;
	newMakr->arg2 = nil_node;
	newMakr->_op_tag = op_tag;
	return newMakr;
}

CondMakr *initializeMakr() {
	return newCondMakr(nil_makr, IGNORE_OPERATION);
}

static int isRootMakr(CondMakr *makr) {
	return makr->_op_tag == IGNORE_OPERATION;
}

static int makrHasResult(CondMakr *makr) {
	return makr->result != nil_node;
}

static CondNode *makeMergedNode(CondMakr *makr) {
	return mergeNodes(makr->arg1, makr->arg2, makr->_op_tag);
}

static CondMakr *passNodeToParent(CondMakr *makr) {
	CondMakr *parent = makr->parent;
	CondMakr *child = makr;
	parent = addNodeSimple(makeMergedNode(child), parent);
	free(child);
	return parent;
}

static CondMakr *simplifyMakr(CondMakr *makr) {
	ASSURE(makr != NULL && makr != nil_makr,
		LOG_ERROR("INTERNAL ERROR");
		return nil_makr;
	);
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
	ASSURE(node != NULL && makr != NULL &&
			node != nil_node && makr != nil_makr,
		LOG_ERROR("INTERNAL ERROR");
		cleanCondNode(node);
		return (makr == NULL)? nil_makr: makr;
	);
	ASSURE(!makrHasResult(makr),
		LOG_ERROR("MAKER ALREADY HAS RESULT. DISCARDING INPUT");
		cleanCondNode(node);
		return makr;
	);
	CondMakr *result = makr;
	result = addNodeSimple(node, result);
	result = simplifyMakr(result);
	return result;
}

CondMakr *addOr(CondMakr *makr, CondNode *node) {
	return newCondMakr(makr, OR_OPERATION);
}

CondMakr *addAnd(CondMakr *makr, CondNode *node) {
	return newCondMakr(makr, AND_OPERATION);
}

CondMakr *addCond(CondMakr *makr, CondNode *node) {
	ASSURE(makr != NULL && makr != nil_makr,
		LOG_ERROR("MAKER IS INVALID, DISCARDING INPUT");
		cleanCondNode(node);
		return nil_makr;
	);
	return addNodeToMakr(node, makr);
}

static void cleanCondMakr(CondMakr *makr) {
	CondMakr *cur_makr = makr;
	while(cur_makr != nil_makr) {
		cleanCondNode(cur_makr->result);
		cleanCondNode(cur_makr->arg1);
		cleanCondNode(cur_makr->arg2);
		CondMakr *parent = cur_makr->parent;
		free(cur_makr);
		cur_makr = parent;
	}
}

CondNode *finalizeMakr(CondMakr *makr) {
	ASSURE(makrHasResult(makr),
		LOG_ERROR("INVALID INPUT")
		return nil_node;
	);
	CondNode *result = makr->result;
	makr->result = nil_node;
	cleanCondMakr(makr);
	return result;
}

void testMakr();
#ifdef TEST_CONDITION_MAKER
int main() {
	testNodes();
	testMakr();
	return 0;
}
#endif

static void testInitializeMakr();
static void testAddOr();
static void testAddAnd();
static void testFinalizeMakr();
static void testAddNodeSimple();
static void testPassNodeToParent();
static void testSimplifyMakr();
static void testAddNodeToMakr();
static void testAddCond();

void testMakr() {
	testInitializeMakr();
	testAddOr();
	testAddAnd();
	testFinalizeMakr();
	testAddNodeSimple();
	testPassNodeToParent();
	testSimplifyMakr();
	testAddNodeToMakr();
	testAddCond();
}

static void testInitializeMakr() {
	CondMakr *test = initializeMakr();
	TEST(test->result == nil_node);
	TEST(test->arg1 == nil_node);
	TEST(test->arg1 == nil_node);
	TEST(test->_op_tag == IGNORE_OPERATION);
	cleanCondMakr(test);
}

static void testAddOr() {
	CondMakr *test = addOr(nil_makr, nil_node);
	TEST(test->result == nil_node);
	TEST(test->arg1 == nil_node);
	TEST(test->arg1 == nil_node);
	TEST(test->_op_tag == OR_OPERATION);
	cleanCondMakr(test);
}

static void testAddAnd() {
	CondMakr *test = addAnd(nil_makr, nil_node);
	TEST(test->result == nil_node);
	TEST(test->arg1 == nil_node);
	TEST(test->arg1 == nil_node);
	TEST(test->_op_tag == AND_OPERATION);
	cleanCondMakr(test);
}

static CondNode *getTestMakrCond() {
	return newCondNode(equalsChar, '0', '\0', NO_NEGATION);
}

static void testFinalizeMakr() {
	CondMakr *test = initializeMakr();
	TEST(finalizeMakr(test) == nil_node);
	//not using another makr because test should still be usable
	CondNode *input = getTestMakrCond();
	test->result = input;	
	CondNode *expected = getTestMakrCond();
	TEST(isEquivalent(finalizeMakr(test), expected));
	cleanCondNode(expected);
	cleanCondNode(input);
}

static void testAddNodeSimple() {
	CondMakr *test1 = addNodeSimple(getTestMakrCond(),
		initializeMakr());
	TEST(makrHasResult(test1));
	CondNode *expected1 = getTestMakrCond();
	TEST(isEquivalent(test1->result, expected1));
	cleanCondNode(expected1);
	cleanCondMakr(test1);

	CondMakr *test2 = addOr(initializeMakr(), nil_node);
	test2 = addNodeSimple(getTestMakrCond(), test2);
	CondNode *expected2 = getTestMakrCond();
	TEST(isEquivalent(test2->arg1, expected2));
	test2 = addNodeSimple(getTestMakrCond(), test2);
	TEST(isEquivalent(test2->arg2, expected2));
	cleanCondNode(expected2);
	cleanCondMakr(test2);
}

static void testPassNodeToParent() {
	CondMakr *test1 = addAnd(initializeMakr(), nil_node);
	test1 = addNodeSimple(getTestMakrCond(), test1);
	test1 = addNodeSimple(getTestMakrCond(), test1);
	test1 = passNodeToParent(test1);
	TEST(makrHasResult(test1));
	CondNode *expected1 = mergeNodes(getTestMakrCond(),
		getTestMakrCond(), AND_OPERATION);
	TEST(isEquivalent(test1->result, expected1));
	cleanCondNode(expected1);
	cleanCondMakr(test1);

	CondMakr *test2 = addAnd(initializeMakr(), nil_node);
	test2 = addAnd(test2, nil_node);
	test2 = addNodeSimple(getTestMakrCond(), test2);
	test2 = addNodeSimple(getTestMakrCond(), test2);
	test2 = passNodeToParent(test2);
	CondNode *expected2 = mergeNodes(getTestMakrCond(),
		getTestMakrCond(), AND_OPERATION);
	TEST(isEquivalent(test2->arg1, expected2));
	cleanCondNode(expected2);
	cleanCondMakr(test2);
}

static void testSimpMakrIsNull();
static void testSimpMakrIsNil();
static void testSimpMakrHasResult();
static void testSimpMakrStopsOnRoot();
static void testSimpMakrStopsOnNonNil();
static void testSimpMakrRecurses();
static void testSimplifyMakr() {
	testSimpMakrIsNull();
	testSimpMakrIsNil();
	testSimpMakrHasResult();
	testSimpMakrStopsOnRoot();
	testSimpMakrStopsOnNonNil();
	testSimpMakrRecurses();
}

static void testSimpMakrIsNull() {
	CondMakr *test = NULL;
	TEST(simplifyMakr(test) == nil_makr);
}

static void testSimpMakrIsNil() {
	CondMakr *test = nil_makr;
	TEST(simplifyMakr(test) == nil_makr);
}

static void testSimpMakrHasResult() {
	CondMakr *test = addNodeSimple(getTestMakrCond(),
		initializeMakr());
	CondMakr *expected = test;
	TEST(expected == simplifyMakr(test));
	cleanCondMakr(test);
}

static void testSimpMakrStopsOnRoot() {
	CondMakr *test = initializeMakr();
	CondMakr *expected = test;
	TEST(!makrHasResult(test));
	TEST(expected == simplifyMakr(test));
	cleanCondMakr(test);
}

static void testSimpMakrStopsOnNonNil() {
	CondMakr *test = addAnd(initializeMakr(), nil_node);
	test = addNodeSimple(getTestMakrCond(), test);
	CondMakr *expected = test;
	TEST(expected == simplifyMakr(test));
	cleanCondMakr(test);
}

static void testSimpMakrRecurses() {
	CondMakr *root = addAnd(initializeMakr(), nil_node);
	CondMakr *test = addAnd(root, nil_node);
	test = addNodeSimple(getTestMakrCond(), test);
	test = addNodeSimple(getTestMakrCond(), test);
	CondMakr *expected = root;
	TEST(expected = simplifyMakr(test));
	cleanCondMakr(root);
}

static void testAddNodeNodeNil();
static void testAddNodeMakrNil();
static void testAddNodeMakrHasResult();
static void testAddNodeNominal();
static void testAddNodeToMakr() {
	testAddNodeNodeNil();
	testAddNodeMakrNil();
	testAddNodeMakrHasResult();
	testAddNodeNominal();
}

static void testAddNodeNodeNil() {
	CondMakr *test_makr = initializeMakr();
	CondNode *test_node1 = NULL;
	TEST(addNodeToMakr(test_node1, test_makr) == test_makr);
	CondNode *test_node2 = nil_node;
	TEST(addNodeToMakr(test_node2, test_makr) == test_makr);
	cleanCondMakr(test_makr);
}

static void testAddNodeMakrNil() {
	CondMakr *test_makr1 = NULL;
	CondNode *test_node1 = getTestMakrCond();
	TEST(addNodeToMakr(test_node1, test_makr1) == nil_makr);

	//Need another CondNode because the first gets discarded
	CondMakr *test_makr2 = nil_makr;
	CondNode *test_node2 = getTestMakrCond();
	TEST(addNodeToMakr(test_node2, test_makr2) == nil_makr);
}

static void testAddNodeMakrHasResult() {
	CondMakr *test = initializeMakr();
	test = addNodeToMakr(getTestMakrCond(), test);
	CondMakr *expected = test;
	TEST(expected == addNodeToMakr(getTestMakrCond(), test));
	cleanCondMakr(test);
}

static void testAddNodeNominal() {
	CondMakr *test = initializeMakr();
	CondMakr *expected = test;
	test = addNodeToMakr(getTestMakrCond(), test);
	TEST(expected == test);
	TEST(makrHasResult(test));
	cleanCondMakr(test);
}

static void testAddCondMakrNil();
static void testAddCondMakrNotNil();
static void testAddCond() {
	testAddCondMakrNil();
	testAddCondMakrNotNil();
}

static void testAddCondMakrNil() {
	CondMakr *test_makr1 = NULL;
	CondMakr *test_makr2 = nil_makr;
	CondNode *test_node1 = getTestMakrCond();
	CondNode *test_node2 = getTestMakrCond();
	TEST(addCond(test_makr1, test_node1) == nil_makr);
	TEST(addCond(test_makr2, test_node2) == nil_makr);
}

static void testAddCondMakrNotNil() {
	CondMakr *test = initializeMakr();
	CondMakr *expected = test;
	TEST(expected == addCond(test, getTestMakrCond()));
	cleanCondMakr(test);
}
