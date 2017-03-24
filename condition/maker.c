#define CONDITION_PACKAGE
//#define TEST
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "tags.h"
#include "function/info.h"
#include "../utils/bool.h"
#include "../utils/test.h"

//condition.h
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
CondMakr *addOr(CondMakr *makr);
CondMakr *addAnd(CondMakr *makr);
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
	ASSURE(node != nil_node && makr != nil_makr,
		LOG_ERROR("INVALID INPUT");
		return makr;
	);
	CondMakr *result = makr;
	result = addNodeSimple(node, result);
	result = simplifyMakr(result);
	return result;
}

CondMakr *addOr(CondMakr *makr) {
	return newCondMakr(makr, OR_OPERATION);
}

CondMakr *addAnd(CondMakr *makr) {
	return newCondMakr(makr, AND_OPERATION);
}

CondMakr *addCond(CondMakr *makr, CondNode *node) {
	ASSURE(makr != nil_makr,
		LOG_ERROR("MAKER IS INVALID");
		return nil_makr;
	);
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

CondNode *finalizeMakr(CondMakr *makr) {
	ASSURE(makrHasResult(makr),
		LOG_ERROR("INVALID INPUT")
		return nil_node;
	);
	CondNode *result = makr->result;
	cleanMakr(makr);
	return result;
}

//TODO: redo the tests with the new interface
