#ifndef CONDITION_NODE_H
#define CONDITION_NODE_H
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
void testNodes();

#endif
#endif
