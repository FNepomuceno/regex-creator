#ifndef CONDITION_MAKER_H
#define CONDITION_MAKER_H

typedef struct CondMakr CondMakr;
typedef struct CondNode CondNode;
extern CondMakr *nil_makr;

CondMakr *initializeMakr();
CondNode *finalizeMakr(CondMakr *makr);

void testMakr();

#endif
