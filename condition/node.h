#ifndef CONDITION_NODE_H
#define CONDITION_NODE_H

#ifdef MODULE_TESTING_H
#ifdef TESTING_PROGRAM
#define TESTING_CONDITION_PACKAGE
#endif
#ifdef TESTING_CONDITION_PACKAGE
#define TESTING_CONDITION_NODE
#else
#define TEST_THIS
#endif
#ifdef TESTING_CONDITION_NODE
#include "../utils/test.h"
#endif
#endif


#ifdef  CONDITION_PACKAGE

typedef struct CondNode CondNode;
typedef struct OperationTag OperationTag;
typedef struct NegationTag NegationTag;

extern CondNode *nil_node;
typedef int CondFunc(char, char, char);

CondNode *newCondNode(CondFunc func, char arg1, char arg2,
	const NegationTag *neg_tag);
CondNode *mergeNodes(CondNode *node1, CondNode *node2,
	const OperationTag *op_tag);
void cleanCondNode(CondNode *node);
int isEquivalent(CondNode *node1, CondNode *node2);
int evaluateFunc(CondNode *node, char input);

#ifdef TESTING_CONDITION_NODE
TEST_CASE(Node);
#endif
#endif


#ifdef CONDITION_NODE_MOD

typedef struct CondNode CondNode;
typedef int CondFunc(char, char, char);

int evaluateCond(CondNode *node, char input);
static int combineEvals(int bool1, int bool2,
		const OperationTag *op_tag);

static int isNegated(CondNode *node);
static int isLeafCond(CondNode *node);
static int isLinkCond(CondNode *node);
static int isBranchCond(CondNode *node);
static int isNullCond(CondNode *node);
static int isValidCond(CondNode *node);

static int dataEquivalent(CondNode *node1, CondNode *node2);
static int haveFuncsEquivalent(CondNode *node1, CondNode *node2);

#ifdef TESTING_CONDITION_NODE
static TEST_CASE(NullEquivalence);
static TEST_RES(CondNode *, EqvLeafCondNode, void);
static TEST_CASE(NilNodeEquivalence);
static TEST_CASE(SameNodeEquivalence);
static TEST_CASE(DiffArgsEquivalence);
static TEST_RES(CondNode *, EqvDiffArgsLeafNode, void);
static TEST_CASE(DiffFuncEquivalence);
static TEST_RES(CondNode *, EqvLinkCondNode, void);
static TEST_RES(CondNode *, EqvDiffFuncLinkNode, void);
static TEST_CASE(DiffOperationTagEquivalence);
static TEST_RES(CondNode *, EqvDiffOpTagLinkNode, void);
static TEST_CASE(DiffNegationTagEquivalence);
static TEST_RES(CondNode *, EqvDiffNegTagLeafNode, void);
static TEST_CASE(LinkWithLeafEquivalence);
static TEST_CASE(LeafWithLinkEquivalence);
static TEST_CASE(LinkWithBranchEquivalence);
static TEST_RES(CondNode *, EqvBranchCondNode, void);
static TEST_CASE(BranchWithLinkEquivalence);

static TEST_CASE(LeafEvaluation);
static TEST_RES(CondNode *, EvlLeafCondNode, void);
static TEST_CASE(LinkEvaluation);
static TEST_RES(CondNode *, EvlLinkCondNode, void);
static TEST_CASE(BranchEvaluation);
static TEST_RES(CondNode *, EvlBranchCondNode, void);

#endif
#endif

#endif
