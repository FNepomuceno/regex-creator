#ifdef CONDITION_NODE_MODULE

static const int OR_OP = 2;
static const int AND_OP = 3;
static const int IGNORE_OP = 4;
static const int YES_NEGATE = 5;
static const int NO_NEGATE = 6;
static const int IGNORE_NEGATE = 7;

typedef struct CondNode CondNode;
typedef int CondFunc(char, char, char);
static CondNode *newNodeBranch(CondNode *node1, CondNode *node2,
	int op_flag);

static CondNode *linkNodes(CondNode *node1, CondNode *node2,
	int op_flag);
static CondNode *branchNodes(CondNode *node1, CondNode *node2,
	int op_flag);

static int evaluateCond(CondNode *node, char input);
static int combineEvals(int bool1, int bool2, int op_flag);
static int evaluateFunc(CondNode *node, char input);

static int isNegated(CondNode *node);
static int isLeafCond(CondNode *node);
static int isLinkCond(CondNode *node);
static int isBranchCond(CondNode *node);

static int dataEquivalent(CondNode *node1, CondNode *node2);
static int haveFuncsEquivalent(CondNode *node1, CondNode *node2);

static void testCondEval();
static void testCondEquivalence();
static void testCondMerge();
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
static void testMismatchEquivalence();
static void testNegateEquivalence();
static void testLeafMerge();
static void testLinkMerge();
static void testBranchMerge();

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
static CondNode *getTestMergerLeaf();
static CondNode *getTestMergeeLeaf();
static CondNode *getTestLeafToLeafMerge();
static CondNode *getTestMergerLink();
static CondNode *getTestMergeeLink();
static CondNode *getTestLeafToLinkMerge();
static CondNode *getTestLinkToLeafMerge();
static CondNode *getTestLinkToLinkMerge();
static CondNode *getTestMergerBranch();
static CondNode *getTestMergeeBranch();
static CondNode *getTestLeafToBranchMerge();
static CondNode *getTestLinkToBranchMerge();
static CondNode *getTestBranchToLeafMerge();
static CondNode *getTestBranchToLinkMerge();
static CondNode *getTestBranchToBranchMerge();

#endif
