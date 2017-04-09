#ifndef CONDITION_MAKER_H
#define CONDITION_MAKER_H

#ifdef MODULE_TESTING_H
#ifdef TESTING_PROGRAM
#define TESTING_CONDITION_PACKAGE
#endif
#ifdef TESTING_CONDITION_PACKAGE
#define TESTING_CONDITION_MAKER
#else
#define TEST_THIS
#endif
#ifdef TESTING_CONDITION_MAKER
#include "../utils/test.h"
#endif
#endif


#ifdef  CONDITION_PACKAGE

typedef struct CondMakr CondMakr;
typedef struct CondNode CondNode;
extern CondMakr *nil_makr;

CondMakr *initializeMakr();
CondNode *finalizeMakr(CondMakr *makr);

CondMakr *addOr(CondMakr *makr, CondNode *node);
CondMakr *addAnd(CondMakr *makr, CondNode *node);
CondMakr *addCond(CondMakr *makr, CondNode *node);

#ifdef TESTING_CONDITION_MAKER
void testMakr();
#endif
#endif


#ifdef CONDITION_MAKER_MOD

typedef int CondFunc(char, char, char);
typedef struct OperationTag OperationTag;
typedef struct NegationTag NegationTag;
static CondMakr *newCondMakr(CondMakr *parent,
		const OperationTag *op_tag);
static int makrHasResult(CondMakr *makr);
static void cleanCondMakr(CondMakr *makr);
static CondMakr *addNodeToMakr(CondNode *node, CondMakr *makr);
static CondMakr *addNodeSimple(CondNode *node, CondMakr *makr);
static CondMakr *simplifyMakr(CondMakr *makr);
static int isRootMakr(CondMakr *makr);
static CondMakr *passNodeToParent(CondMakr *makr);
static CondNode *makeMergedNode(CondMakr *makr);

#ifdef TESTING_CONDITION_MAKER
static TEST_CASE(FinalizeNullMakr);
static TEST_RES(CondNode *, FinalizedNullMakrResult, void);

static TEST_CASE(FinalizeNilMakr);
static TEST_RES(CondNode *, FinalizedNilMakrResult, void);

static TEST_CASE(FinalizeEmptyMakr);
static TEST_RES(CondNode *, FinalizedEmptyMakrResult, void);

static TEST_CASE(FinalizeMakrWithResult);
static TEST_RES(CondNode *, FinalizedFinMakrResult, void);
static TEST_RES(CondNode *, TestLeafCondNode, void);

static TEST_CASE(FinalizeIncompleteMakr);
static TEST_RES(CondNode *, FinalizedUnfMakrResult, void);

static TEST_CASE(AddOrWithNonNilNode);
static TEST_RES(CondNode *, AddOrWithNonNilNode, void);

static TEST_CASE(AddAndWithNonNilNode);
static TEST_RES(CondNode *, AddAndWithNonNilNode, void);

static TEST_CASE(MakrMergeOr);
static TEST_RES(CondNode *, FinalizedOrMakrResult, void);
static TEST_RES(CondNode *, TestOrCondNode, void);

static TEST_CASE(MakrMergeAnd);
static TEST_RES(CondNode *, FinalizedAndMakrResult, void);
static TEST_RES(CondNode *, TestAndCondNode, void);
#endif
#endif

#endif
