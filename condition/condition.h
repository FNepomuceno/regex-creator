#ifndef CONDITION_H
#define CONDITION_H

typedef struct CondNode CondNode;
void cleanCondition(CondNode *node);
CondNode *getCondition(char *str);

#endif
