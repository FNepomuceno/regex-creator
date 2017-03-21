#ifndef NODE_PKG
#define NODE_PKG

typedef struct CondNode CondNode;
void cleanCondNode(CondNode *node);

#ifdef  CONDITION_PACKAGE

extern CondNode *nil_node;
typedef int CondFunc(char, char, char);

CondNode *mergeNodes(CondNode *node1, CondNode *node2,
	int op_flag);
CondNode *newNodeLeaf(CondFunc func, char arg1, char arg2);
CondNode *negateNode(CondNode *node);

void testNodes();
int isEquivalent(CondNode *node1, CondNode *node2);

#endif

#endif
