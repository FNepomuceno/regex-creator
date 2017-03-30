#ifndef CONDITION_LIST_H
#define CONDITION_LIST_H
#ifdef  CONDITION_PACKAGE

typedef struct CondList CondList;
typedef struct CondNode CondNode;
CondList *getCondList(char *str);
void cleanCondList(CondList *list);

extern CondList *nil_list;

CondList *advanceList(CondList *list);
CondNode *extractNode(CondList *list);

void testGetCondList();

#endif
#endif
