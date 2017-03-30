#ifdef CONDITION_NODE_MODULE

typedef struct CondNode CondNode;
typedef int CondFunc(char, char, char);

int evaluateCond(CondNode *node, char input);
static int combineEvals(int bool1, int bool2,
		const OperationTag *op_tag);
static int evaluateFunc(CondNode *node, char input);

static int isNegated(CondNode *node);
static int isLeafCond(CondNode *node);
static int isLinkCond(CondNode *node);
static int isBranchCond(CondNode *node);
static int isNullCond(CondNode *node);
static int isValidCond(CondNode *node);

static int dataEquivalent(CondNode *node1, CondNode *node2);
static int haveFuncsEquivalent(CondNode *node1, CondNode *node2);

static void testPropertyFuncs();
static void testIsNullCond();
static void testIsNegatedCond();
static void testIsLeafCond();
static void testIsLinkCond();
static void testIsBranchCond();
static void testIsValidCond();
static CondNode *getTestLeafCond();
static CondNode *getTestLinkCond();
static CondNode *getTestBranchCond();
static CondNode *getTestMalformedCond();

static void testMergingFuncs();
static void testLinkNodes();
static void testBranchNodes();

static void testMergeNodes();
static void testMergeNode1Null();
static void testMergeNode2Null();
static void testMergeNode1Nil();
static void testMergeNode2Nil();
static void testMergeNode1Invalid();
static void testMergeNode2Invalid();
static void testMergeNode1Leaf();
static void testMergeNode1NotLeaf();
static CondNode *getTestMergeInvalidNode();
static CondNode *getTestMergeLeafNode();
static CondNode *getTestMergeLinkNode();

static void testFuncEquivalence();
static void testFuncEqvBothBranchNodes();
static void testFuncEqvNode1NotBranch();
static void testFuncEqvNode2NotBranch();
static void testFuncEqvNeitherBranchNodes();

static void testDataEquivalence();
static void testDataEqvUnequalOpTags();
static void testDataEqvUnequalNegTags();
static void testDataEqvFuncsNotEquivalent();
static void testDataEqvNominalCase();
static CondNode *getDataEqvTargetLink();
static CondNode *getDataEqvDiffOpLink();
static CondNode *getDataEqvDiffNegLink();
static CondNode *getDataEqvDiffFuncLink();

static void testCondEquivalence();
static void testCondEqvNode1IsNull();
static void testCondEqvNode2IsNull();
static void testCondEqvNode1SameAsNode2();
static void testCondEqvNode1IsNil();
static void testCondEqvNode2IsNil();
static void testCondEqvDataEqvFalse();
static void testCondEqvChild1NotEqv();
static void testCondEqvChild2NotEqv();
static void testCondEqvNodesEqv();
static CondNode *getCondEqvTargetBranch();
static CondNode *getCondEqvDiffData();
static CondNode *getCondEqvDiffChild1();
static CondNode *getCondEqvDiffChild2();

static void testEvalSubFuncs();
static void testCombineEvalsBool1Invalid();
static void testCombineEvalsBool2Invalid();
static void testCombineEvalsOpTagInvalid();
static void testEvaluateFuncNodeIsNull();
static void testEvaluateFuncNodeIsNil();

static void testEvaluateCond();
static void testEvaluateCondLeaf();
static void testEvaluateCondLink();
static void testEvaluateCondBranch();
static void testEvaluateCondInvalid();
static CondNode *getTestEvalLeafCond();
static CondNode *getTestEvalLinkCond();
static CondNode *getTestEvalBranchCond();
static CondNode *getTestEvalInvalidCond();

#endif
