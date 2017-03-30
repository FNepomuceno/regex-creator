#ifndef CONDITION_MAKER_H
#define CONDITION_MAKER_H
#ifdef  CONDITION_PACKAGE

typedef struct CondMakr CondMakr;
typedef struct CondNode CondNode;
extern CondMakr *nil_makr;

CondMakr *initializeMakr();
CondNode *finalizeMakr(CondMakr *makr);

CondMakr *addOr(CondMakr *makr, CondNode *node);
CondMakr *addAnd(CondMakr *makr, CondNode *node);
CondMakr *addCond(CondMakr *makr, CondNode *node);

void testMakr();

#endif
#endif
