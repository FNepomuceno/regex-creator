#define CONDITION_PACKAGE
#define CONDITION_MAKER_MOD

#include <stdlib.h>
#include "../testing.h"
#include "maker.h"
#include "node.h"
#include "tags.h"
#include "function/info.h"
#include "../utils/bool.h"
#include "../utils/test.h"

struct CondMakr {
	CondMakr *parent;
	CondNode *result;
	CondNode *arg1, *arg2;
	const OperationTag *_op_tag;
};
static CondMakr nil_condmakr_obj;
CondMakr *nil_makr = &nil_condmakr_obj;

CondMakr *initializeMakr() {
	return newCondMakr(nil_makr, IGNORE_OPERATION);
}

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

CondNode *finalizeMakr(CondMakr *makr) {
	ASSURE(makr != NULL &&
			makr != nil_makr &&
			makrHasResult(makr),
		LOG_ERROR("INVALID INPUT")
		return nil_node;
	);
	CondNode *result = makr->result;
	makr->result = nil_node;
	cleanCondMakr(makr);
	return result;
}

static int makrHasResult(CondMakr *makr) {
	return makr->result != nil_node;
}

static void cleanCondMakr(CondMakr *makr) {
	if(makr == NULL) return;
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

CondMakr *addOr(CondMakr *makr, CondNode *node) {
	ASSURE(node == nil_node,
		LOG_ERROR("INPUTTED NODE IS INVALID, DISCARDING INPUT");
		cleanCondNode(node);
	);
	return newCondMakr(makr, OR_OPERATION);
}

CondMakr *addAnd(CondMakr *makr, CondNode *node) {
	ASSURE(node == nil_node,
		LOG_ERROR("INPUTTED NODE IS INVALID, DISCARDING INPUT");
		cleanCondNode(node);
	);
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

static CondMakr *addNodeSimple(CondNode *node, CondMakr *makr) {
	if(isRootMakr(makr)) {
		makr->result = node;
		return makr;
	}

	if(makr->arg1 == nil_node) makr->arg1 = node;
	else makr->arg2 = node;

	return makr;
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

static int isRootMakr(CondMakr *makr) {
	return makr->_op_tag == IGNORE_OPERATION;
}

static CondMakr *passNodeToParent(CondMakr *makr) {
	CondMakr *parent = makr->parent;
	CondMakr *child = makr;
	parent = addNodeSimple(makeMergedNode(child), parent);
	free(child);
	return parent;
}

static CondNode *makeMergedNode(CondMakr *makr) {
	return mergeNodes(makr->arg1, makr->arg2, makr->_op_tag);
}

#ifdef TESTING_CONDITION_MAKER
#ifdef TEST_THIS
int main() {
	testMakr();
	return 0;
}
#endif

void testMakr() {
	USE_CASE(FinalizeNullMakr);
	USE_CASE(FinalizeNilMakr);
	USE_CASE(FinalizeEmptyMakr);
	USE_CASE(FinalizeMakrWithResult);
	USE_CASE(FinalizeIncompleteMakr);

	USE_CASE(AddOrWithNonNilNode);
	USE_CASE(AddAndWithNonNilNode);

	USE_CASE(MakrMergeOr);
	USE_CASE(MakrMergeAnd);
}

static TEST_CASE(FinalizeNullMakr) {
	COMPARE_TEST(CondNode *, Null,
		USE_RES(FinalizedNullMakrResult,),
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedNullMakrResult, void) {
	CondMakr *makr = NULL;
	CondNode *result = finalizeMakr(makr);
	cleanCondMakr(makr);
	return result;
}

static TEST_CASE(FinalizeNilMakr) {
	COMPARE_TEST(CondNode *, Nil,
		USE_RES(FinalizedNilMakrResult,),
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedNilMakrResult, void) {
	CondMakr *makr = nil_makr;
	CondNode *result = finalizeMakr(makr);
	cleanCondMakr(makr);
	return result;
}

static TEST_CASE(FinalizeEmptyMakr) {
	COMPARE_TEST(CondNode *, Empty,
		USE_RES(FinalizedEmptyMakrResult,),
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedEmptyMakrResult, void) {
	CondMakr *makr = initializeMakr();
	CondNode *result = finalizeMakr(makr);
	cleanCondMakr(makr);
	return result;
}

static TEST_CASE(FinalizeMakrWithResult) {
	COMPARE_TEST(CondNode *, Complete,
		USE_RES(FinalizedFinMakrResult,),
		USE_RES(TestLeafCondNode,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedFinMakrResult, void) {
	CondMakr *makr = initializeMakr();
	makr = addCond(makr, newCondNode(equalsChar, '0', '\0',
		NO_NEGATION));
	CondNode *result = finalizeMakr(makr);
	return result;
}

static TEST_RES(CondNode *, TestLeafCondNode, void) {
	return newCondNode(equalsChar, '0', '\0', NO_NEGATION);
}

static TEST_CASE(FinalizeIncompleteMakr) {
	COMPARE_TEST(CondNode *, Incomplete,
		USE_RES(FinalizedUnfMakrResult,),
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedUnfMakrResult, void) {
	CondMakr *makr = initializeMakr();
	makr = addOr(makr, newCondNode(equalsChar, '0', '\0',
		NO_NEGATION));
	CondNode *result = finalizeMakr(makr);
	cleanCondMakr(makr);
	return result;
}

static TEST_CASE(AddOrWithNonNilNode) {
	COMPARE_TEST(CondNode *, OrNonNil,
		USE_RES(AddOrWithNonNilNode,),
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, AddOrWithNonNilNode, void) {
	CondMakr *makr = initializeMakr();
	makr = addOr(makr, newCondNode(equalsChar, '0', '\0',
		NO_NEGATION));
	CondNode *result = finalizeMakr(makr);
	cleanCondMakr(makr);
	return result;
}

static TEST_CASE(AddAndWithNonNilNode) {
	COMPARE_TEST(CondNode *, AndNonNil,
		USE_RES(AddAndWithNonNilNode,),
		nil_node,
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, AddAndWithNonNilNode, void) {
	CondMakr *makr = initializeMakr();
	makr = addAnd(makr, nil_node);
	CondNode *result = finalizeMakr(makr);
	cleanCondMakr(makr);
	return result;
}

static TEST_CASE(MakrMergeOr) {
	COMPARE_TEST(CondNode *, OrMerge,
		USE_RES(FinalizedOrMakrResult,),
		USE_RES(TestOrCondNode,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedOrMakrResult, void) {
	CondMakr *makr = initializeMakr();
	makr = addOr(makr, nil_node);
	makr = addCond(makr, newCondNode(equalsChar, '1', '\0',
		NO_NEGATION));
	makr = addCond(makr, newCondNode(equalsChar, '2', '\0',
		NO_NEGATION));
	CondNode *result = finalizeMakr(makr);
	return result;
}

static TEST_RES(CondNode *, TestOrCondNode, void) {
	CondNode *setup1 =  newCondNode(equalsChar, '1', '\0',
		NO_NEGATION);
	CondNode *setup2 =  newCondNode(equalsChar, '2', '\0',
		NO_NEGATION);
	return mergeNodes(setup1, setup2, OR_OPERATION);
}

static TEST_CASE(MakrMergeAnd) {
	COMPARE_TEST(CondNode *, AndMerge,
		USE_RES(FinalizedAndMakrResult,),
		USE_RES(TestAndCondNode,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, FinalizedAndMakrResult, void) {
	CondMakr *makr = initializeMakr();
	makr = addAnd(makr, nil_node);
	makr = addCond(makr, newCondNode(equalsChar, '1', '\0',
		NO_NEGATION));
	makr = addCond(makr, newCondNode(equalsChar, '2', '\0',
		NO_NEGATION));
	CondNode *result = finalizeMakr(makr);
	return result;
}

static TEST_RES(CondNode *, TestAndCondNode, void) {
	CondNode *setup1 =  newCondNode(equalsChar, '1', '\0',
		NO_NEGATION);
	CondNode *setup2 =  newCondNode(equalsChar, '2', '\0',
		NO_NEGATION);
	return mergeNodes(setup1, setup2, AND_OPERATION);
}
#endif
