#ifndef NODE_TEST
#define NODE_TEST
#ifdef CONDITION_NODE_MODULE

static void testNodes();
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
#endif
