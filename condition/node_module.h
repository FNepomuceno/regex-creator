#ifdef CONDITION_NODE_MODULE

static const int OR_OP = 2;
static const int AND_OP = 3;
static const int IGNORE_OP = 4;
static const int YES_NEGATE = 5;
static const int NO_NEGATE = 6;
static const int IGNORE_NEGATE = 7;

static CondNode *newNodeLeaf(CondFunc func, char arg1, char arg2);
static CondNode *newNodeBranch(CondNode *node1, CondNode *node2,
	int op_flag);
void cleanCondNode(CondNode *node);

static CondNode *mergeNodes(CondNode *node1, CondNode *node2,
	int op_flag);
static CondNode *linkNodes(CondNode *node1, CondNode *node2,
	int op_flag);
static CondNode *branchNodes(CondNode *node1, CondNode *node2,
	int op_flag);
static CondNode *negateNode(CondNode *node);

static int evaluateCond(CondNode *node, char input);
static int combineEvals(int bool1, int bool2, int op_flag);
static int evaluateFunc(CondNode *node, char input);

static int isNegated(CondNode *node);
static int isLeafCond(CondNode *node);
static int isLinkCond(CondNode *node);
static int isBranchCond(CondNode *node);

int isEquivalent(CondNode *node1, CondNode *node2);
static int dataEquivalent(CondNode *node1, CondNode *node2);
static int haveFuncsEquivalent(CondNode *node1, CondNode *node2);

#endif
