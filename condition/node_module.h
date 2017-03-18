#ifdef CONDITION_NODE_MODULE
static const int OR_OP = 2;
static const int AND_OP = 3;
static const int IGNORE_OP = 4;
static const int YES_NEGATE = 5;
static const int NO_NEGATE = 6;
static const int IGNORE_NEGATE = 7;

static CondNode *newNodeLeaf(CondFunc, char, char);
static CondNode *newNodeBranch(CondNode *, CondNode *, int);
//public access?
void cleanCondNode(CondNode *data);

//package access
CondNode *mergeNodes(CondNode *, CondNode *, int);
static CondNode *linkNodes(CondNode *, CondNode *, int);
static CondNode *branchNodes(CondNode *, CondNode *, int);
//package access
CondNode *negateNode(CondNode *);

int evaluateCond(CondNode *, char);
static int combineEvals(int, int, int);
static int evaluateFunc(CondNode *, char);
static int applyCondition(CondNode *, char);

static int isNegated(int);
static int isLeafCond(CondNode *);
static int isLinkCond(CondNode *);
static int isBranchCond(CondNode *);

int isEquivalent(CondNode *, CondNode *);
static int dataEquivalent(CondNode *, CondNode *);

struct CondNode {
	CondFunc *func;
	CondNode *chld1, *chld2; // children of node
	int op_flag; // and or flag
	int neg_flag; // negate flag
	char arg1, arg2; // arguments for func
};

static CondNode nil_obj;
static CondNode *nil = &nil_obj;
#endif
