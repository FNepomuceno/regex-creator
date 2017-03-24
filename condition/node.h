#ifndef CONDITION_NODE_H
#define CONDITION_NODE_H

typedef struct CondNode CondNode;
typedef struct OperationTag OperationTag;
typedef struct NegationTag NegationTag;
void cleanCondNode(CondNode *node);

#ifdef  CONDITION_PACKAGE

extern CondNode *nil_node;
typedef int CondFunc(char, char, char);

CondNode *mergeNodes(CondNode *node1, CondNode *node2,
	const OperationTag *op_tag);
CondNode *newCondNode(CondFunc func, char arg1, char arg2,
	const NegationTag *neg_tag);

void testNodes();
int isEquivalent(CondNode *node1, CondNode *node2);

#endif

#endif
