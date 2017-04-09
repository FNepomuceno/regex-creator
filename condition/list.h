#ifndef CONDITION_LIST_H
#define CONDITION_LIST_H

#ifdef MODULE_TESTING_H
#ifdef TESTING_PROGRAM
#define TESTING_CONDITION_PACKAGE
#endif
#ifdef TESTING_CONDITION_PACKAGE
#define TESTING_CONDITION_CONDLIST
#else
#define TEST_THIS
#endif
#ifdef TESTING_CONDITION_CONDLIST
#include "../utils/test.h"
#endif
#endif


#ifdef  CONDITION_PACKAGE

typedef struct CondList CondList;
typedef struct CondNode CondNode;
extern CondList *nil_list;

CondList *getCondList(char *str);
void cleanCondList(CondList *list);
CondList *advanceList(CondList *list);
CondNode *extractNode(CondList *list);

#ifdef TESTING_CONDITION_CONDLIST
TEST_CASE(GetCondList);
#endif
#endif


#ifdef CONDITION_CONDLIST_MOD
typedef struct NegationTag NegationTag;
static CondList *newLoneCharList(char *str,
		const NegationTag *tag);
static CondList *newCharCategoryList(char *str);
static CondList *newNegatedCharCategoryList(char *str);
static CondList *newEscapedLiteralCharList(char *str);
static CondList *newLinkedDataList(char *str, int length);
static CondList *initializeList();
static CondList *addNodeToList(CondList *list, CondNode *node);
static CondList *addEscapedCharNodeToList(CondList *list,
		char ch, const NegationTag *tag);
static CondList *finishList(CondList *list);
static CondList *addNewListItem(CondList *list, CondNode *node);
static void insertListItem(CondList *dst, CondList *src);

#ifdef TESTING_CONDITION_CONDLIST
static int listsAreEquivalent(CondList *list1, CondList *list2);

static TEST_CASE(GetCondListNull);

static TEST_CASE(GetCondListSingleChar);
static TEST_RES(CondList *, LoneChar, void);
static TEST_RES(CondList *, EscapedLiteral, void);
static TEST_RES(CondList *, EscapedMeta, void);
static TEST_RES(CondList *, WildcardChar, void);

static TEST_CASE(GetCondListCharCategory);
static TEST_RES(CondList *, DigitCategory, void);
static TEST_RES(CondList *, NegatedDigitCategory, void);
static TEST_RES(CondList *, WordCategory, void);

static TEST_CASE(GetCondListSimpleCharClass);
static TEST_RES(CondList *, SingleCharClass, void);
static TEST_RES(CondList *, NegatedSingleCharClass, void);
static TEST_RES(CondList *, SingleRangeClass, void);
static TEST_RES(CondList *, EscapedCharClass, void);

static TEST_CASE(GetCondListComplexCharClass);
static TEST_RES(CondList *, DoubleLetterClass, void);
static TEST_RES(CondList *, ClassWithDash, void);
static TEST_RES(CondList *, NegatedComplexClass, void);
#endif
#endif

#endif
