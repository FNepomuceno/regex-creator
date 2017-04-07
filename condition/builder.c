#define CONDITION_PACKAGE

#include <string.h>
#include "list.h"
#include "maker.h"
#include "node.h"
#include "parsestring.h"
#include "tags.h"
#include "function/info.h"
#include "../testing.h"
#include "../utils/bool.h"
#include "../utils/test.h"

typedef struct CondMakr *MakrAction(CondMakr *, CondNode *);

static MakrAction *getAction(char ch);
CondNode *buildCond(char *str) {
	char *parse_string = getParseString(str);
	CondList *cond_list = getCondList(str);
	if(parse_string == INVALID_STRING ||
			cond_list == nil_list) {
		cleanParseString(parse_string);
		cleanCondList(cond_list);
		return nil_node;
	}
	CondMakr *makr = initializeMakr();
	int length = strlen(parse_string);
	for(int i = 0; i < length; i++) {
		MakrAction *act = getAction(*(parse_string+i));
		if(act == addCond) {
			makr = act(makr, extractNode(cond_list));
			cond_list = advanceList(cond_list);
		} else {
			makr = act(makr, nil_node);
		}
	}

	cleanParseString(parse_string);
	cleanCondList(cond_list);
	return finalizeMakr(makr);
}

static MakrAction *getAction(char ch) {
	switch(ch) {
		case '#':
			return addCond;
		case '|':
			return addOr;
		case '&':
			return addAnd;
		default:
			return NULL;
	}
}

void testBuildCond();
#ifdef TEST_CONDITION_BUILDER
int main() {
	testBuildCond();
	return 0;
}
#endif

static void testBuildCondNull();
static void testBuildCondSingleChar();
static void testBuildCondCharCategory();
static void testBuildCondSimpleCharClass();
static void testBuildCondComplexCharClass();
void testBuildCond() {
	testBuildCondNull();
	testBuildCondSingleChar();
	testBuildCondCharCategory();
	testBuildCondSimpleCharClass();
	testBuildCondComplexCharClass();
}

static CondNode *getTestBuildCondNullNode1();
static CondNode *getTestBuildCondNullNode2();
static void testBuildCondNull() {
	CondNode *test1 = buildCond(NULL);
	CondNode *expected1 = getTestBuildCondNullNode1();
	TEST(isEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(expected1);

	CondNode *test2 = buildCond("");
	CondNode *expected2 = getTestBuildCondNullNode2();
	TEST(isEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondNode(test2);
	cleanCondNode(expected2);
}

static CondNode *getTestBuildCondNullNode1() {
	return nil_node;
}

static CondNode *getTestBuildCondNullNode2() {
	return nil_node;
}

static CondNode *getTestBuildCondSingleCharNode1();
static CondNode *getTestBuildCondSingleCharNode2();
static CondNode *getTestBuildCondSingleCharNode3();
static void testBuildCondSingleChar() {
	CondNode *test1 = buildCond("a");
	CondNode *expected1 = getTestBuildCondSingleCharNode1();
	TEST(isEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(expected1);

	CondNode *test2 = buildCond("\\\\");
	CondNode *expected2 = getTestBuildCondSingleCharNode2();
	TEST(isEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondNode(test2);
	cleanCondNode(expected2);
	
	CondNode *test3 = buildCond("\\.");
	CondNode *expected3 = getTestBuildCondSingleCharNode3();
	TEST(isEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondNode(test3);
	cleanCondNode(expected3);
}

static CondNode *getTestBuildCondSingleCharNode1() {
	return newCondNode(equalsChar, 'a', '\0', NO_NEGATION);
}

static CondNode *getTestBuildCondSingleCharNode2() {
	return newCondNode(equalsChar, '\\', '\0', NO_NEGATION);
}

static CondNode *getTestBuildCondSingleCharNode3() {
	return newCondNode(equalsChar, '.', '\0', NO_NEGATION);
}

static CondNode *getTestBuildCondCharCategoryNode1();
static CondNode *getTestBuildCondCharCategoryNode2();
static CondNode *getTestBuildCondCharCategoryNode3();
static void testBuildCondCharCategory() {
	CondNode *test1 = buildCond("\\d");
	CondNode *expected1 = getTestBuildCondCharCategoryNode1();
	TEST(isEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(expected1);

	CondNode *test2 = buildCond("\\D");
	CondNode *expected2 = getTestBuildCondCharCategoryNode2();
	TEST(isEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondNode(test2);
	cleanCondNode(expected2);
	
	CondNode *test3 = buildCond("\\w");
	CondNode *expected3 = getTestBuildCondCharCategoryNode3();
	TEST(isEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondNode(test3);
	cleanCondNode(expected3);
}

static CondNode *getTestBuildCondCharCategoryNode1() {
	return newCondNode(isDigitChar, '\0', '\0', NO_NEGATION);
}

static CondNode *getTestBuildCondCharCategoryNode2() {
	return newCondNode(isDigitChar, '\0', '\0', YES_NEGATION);
}

static CondNode *getTestBuildCondCharCategoryNode3() {
	return newCondNode(isWordChar, '\0', '\0', NO_NEGATION);
}

static CondNode *getTestBuildCondSimpleCharClassNode1();
static CondNode *getTestBuildCondSimpleCharClassNode2();
static CondNode *getTestBuildCondSimpleCharClassNode3();
static void testBuildCondSimpleCharClass() {
	CondNode *test1 = buildCond("[C]");
	CondNode *expected1 = getTestBuildCondSimpleCharClassNode1();
	TEST(isEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(expected1);

	CondNode *test2 = buildCond("[^q]");
	CondNode *expected2 = getTestBuildCondSimpleCharClassNode2();
	TEST(isEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondNode(test2);
	cleanCondNode(expected2);
	
	CondNode *test3 = buildCond("[R-b]");
	CondNode *expected3 = getTestBuildCondSimpleCharClassNode3();
	TEST(isEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondNode(test3);
	cleanCondNode(expected3);
}

static CondNode *getTestBuildCondSimpleCharClassNode1() {
	return newCondNode(equalsChar, 'C', '\0', NO_NEGATION);
}

static CondNode *getTestBuildCondSimpleCharClassNode2() {
	return newCondNode(equalsChar, 'q', '\0', YES_NEGATION);
}

static CondNode *getTestBuildCondSimpleCharClassNode3() {
	return newCondNode(inRangeChar, 'R', 'b', NO_NEGATION);
}

static CondNode *getTestBuildCondComplexCharClassNode1();
static CondNode *getTestBuildCondComplexCharClassNode2();
static CondNode *getTestBuildCondComplexCharClassNode3();
static void testBuildCondComplexCharClass() {
	CondNode *test1 = buildCond("[eZ]");
	CondNode *expected1 = getTestBuildCondComplexCharClassNode1();
	TEST(isEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondNode(test1);
	cleanCondNode(expected1);

	CondNode *test2 = buildCond("[-#-%!]");
	CondNode *expected2 = getTestBuildCondComplexCharClassNode2();
	TEST(isEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondNode(test2);
	cleanCondNode(expected2);
	
	CondNode *test3 = buildCond("[^Wua-h?]");
	CondNode *expected3 = getTestBuildCondComplexCharClassNode3();
	TEST(isEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondNode(test3);
	cleanCondNode(expected3);
}

static CondNode *getTestBuildCondComplexCharClassNode1() {
	CondNode *setup1 = newCondNode(equalsChar, 'e', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'Z', '\0',
		NO_NEGATION);
	CondNode *setup3 = mergeNodes(setup1, setup2,
		OR_OPERATION);
	return setup3;
}

static CondNode *getTestBuildCondComplexCharClassNode2() {
	CondNode *setup1 = newCondNode(equalsChar, '-', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(inRangeChar, '#', '%',
		NO_NEGATION);
	CondNode *setup3 = newCondNode(equalsChar, '!', '\0',
		NO_NEGATION);
	CondNode *setup4 = mergeNodes(setup2, setup3,
		OR_OPERATION);
	CondNode *setup5 = mergeNodes(setup1, setup4,
		OR_OPERATION);
	return setup5;
}

static CondNode *getTestBuildCondComplexCharClassNode3() {
	CondNode *setup1 = newCondNode(equalsChar, 'W', '\0',
		YES_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'u', '\0',
		YES_NEGATION);
	CondNode *setup3 = newCondNode(inRangeChar, 'a', 'h',
		YES_NEGATION);
	CondNode *setup4 = newCondNode(equalsChar, '?', '\0',
		YES_NEGATION);
	CondNode *setup5 = mergeNodes(setup1, setup2,
		AND_OPERATION);
	CondNode *setup6 = mergeNodes(setup5, setup3,
		AND_OPERATION);
	CondNode *setup7 = mergeNodes(setup6, setup4,
		AND_OPERATION);
	return setup7;
}
