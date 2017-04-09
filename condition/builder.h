#ifndef CONDITION_BUILDER_H
#define CONDITION_BUILDER_H

#ifdef MODULE_TESTING_H
#ifdef TESTING_PROGRAM
#define TESTING_CONDITION_PACKAGE
#endif
#ifdef TESTING_CONDITION_PACKAGE
#define TESTING_CONDITION_BUILDER
#else
#define TEST_THIS
#endif
#ifdef TESTING_CONDITION_BUILDER
#include "../utils/test.h"
#endif
#endif


#ifdef  CONDITION_PACKAGE

typedef struct CondNode CondNode;
CondNode *buildCond(char *str);

#ifdef TESTING_CONDITION_BUILDER
TEST_CASE(BuildCond);
#endif
#endif


#ifdef CONDITION_BUILDER_MOD

typedef struct CondMakr *MakrAction(struct CondMakr *,
	struct CondNode *);
static MakrAction *getAction(char ch);

#ifdef TESTING_CONDITION_BUILDER
static TEST_CASE(BuildCondNull);
static TEST_RES(CondNode *, Null, void);
static TEST_RES(CondNode *, Nil, void);

static TEST_CASE(BuildCondSingleChar);
static TEST_RES(CondNode *, LoneChar, void);
static TEST_RES(CondNode *, EscapedLiteral, void);
static TEST_RES(CondNode *, EscapedMeta, void);

static TEST_CASE(BuildCondCharCategory);
static TEST_RES(CondNode *, DigitCategory, void);
static TEST_RES(CondNode *, NegatedDigitCategory, void);
static TEST_RES(CondNode *, WordCategory, void);

static TEST_CASE(BuildCondSimpleCharClass);
static TEST_RES(CondNode *, SingleCharClass, void);
static TEST_RES(CondNode *, NegatedSingleCharClass, void);
static TEST_RES(CondNode *, SingleRangeClass, void);

static TEST_CASE(BuildCondComplexCharClass);
static TEST_RES(CondNode *, DoubleLetterClass, void);
static TEST_RES(CondNode *, ClassWithDash, void);
static TEST_RES(CondNode *, NegatedComplexClass, void);
#endif
#endif

#endif
