#define CONDITION_PACKAGE
#define CONDITION_BUILDER_MOD

#include <string.h>
#include "../testing.h"
#include "builder.h"
#include "list.h"
#include "maker.h"
#include "node.h"
#include "parsestring.h"
#include "tags.h"
#include "function/info.h"
#include "../utils/bool.h"
#include "../utils/test.h"

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

#ifdef TESTING_CONDITION_BUILDER
#ifdef TEST_THIS
int main() {
	USE_CASE(BuildCond);
	return 0;
}
#endif

TEST_CASE(BuildCond) {
	testBuildCondNull();
	testBuildCondSingleChar();
	testBuildCondCharCategory();
	testBuildCondSimpleCharClass();
	testBuildCondComplexCharClass();
}

static TEST_CASE(BuildCondNull) {
	COMPARE_TEST(CondNode *, Null,
		buildCond(NULL),
		USE_RES(Null,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, Nil,
		buildCond(""),
		USE_RES(Nil,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, Null, void) {
	return nil_node;
}

static TEST_RES(CondNode *, Nil, void) {
	return nil_node;
}

static TEST_CASE(BuildCondSingleChar) {
	COMPARE_TEST(CondNode *, LoneChar,
		buildCond("a"),
		USE_RES(LoneChar,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, EscapedLiteral,
		buildCond("\\g"),
		USE_RES(EscapedLiteral,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, EscapedMeta,
		buildCond("\\."),
		USE_RES(EscapedMeta,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, LoneChar, void) {
	return newCondNode(equalsChar, 'a', '\0', NO_NEGATION);
}

static TEST_RES(CondNode *, EscapedLiteral, void) {
	return newCondNode(equalsChar, 'g', '\0', NO_NEGATION);
}

static TEST_RES(CondNode *, EscapedMeta, void) {
	return newCondNode(equalsChar, '.', '\0', NO_NEGATION);
}

static TEST_CASE(BuildCondCharCategory) {
	COMPARE_TEST(CondNode *, DigitCategory,
		buildCond("\\d"),
		USE_RES(DigitCategory,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, NegatedDigitCategory,
		buildCond("\\D"),
		USE_RES(NegatedDigitCategory,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, WordCategory,
		buildCond("\\w"),
		USE_RES(WordCategory,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, DigitCategory, void) {
	return newCondNode(isDigitChar, '\0', '\0', NO_NEGATION);
}

static TEST_RES(CondNode *, NegatedDigitCategory, void) {
	return newCondNode(isDigitChar, '\0', '\0', YES_NEGATION);
}

static TEST_RES(CondNode *, WordCategory, void) {
	return newCondNode(isWordChar, '\0', '\0', NO_NEGATION);
}

static TEST_CASE(BuildCondSimpleCharClass) {
	COMPARE_TEST(CondNode *, SingleCharClass,
		buildCond("[C]"),
		USE_RES(SingleCharClass,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, NegatedSingleCharClass,
		buildCond("[^q]"),
		USE_RES(NegatedSingleCharClass,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, SingleRangeClass,
		buildCond("[R-b]"),
		USE_RES(SingleRangeClass,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, SingleCharClass, void) {
	return newCondNode(equalsChar, 'C', '\0', NO_NEGATION);
}

static TEST_RES(CondNode *, NegatedSingleCharClass, void) {
	return newCondNode(equalsChar, 'q', '\0', YES_NEGATION);
}

static TEST_RES(CondNode *, SingleRangeClass, void) {
	return newCondNode(inRangeChar, 'R', 'b', NO_NEGATION);
}

static TEST_CASE(BuildCondComplexCharClass) {
	COMPARE_TEST(CondNode *, DoubleLetterClass,
		buildCond("[eZ]"),
		USE_RES(DoubleLetterClass,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, ClassWithDash,
		buildCond("[-#-%!]"),
		USE_RES(ClassWithDash,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
	COMPARE_TEST(CondNode *, NegatedComplexClass,
		buildCond("[^Wua-h?]"),
		USE_RES(NegatedComplexClass,),
		isEquivalent, TRUE_BOOL,
		cleanCondNode);
}

static TEST_RES(CondNode *, DoubleLetterClass, void) {
	CondNode *setup1 = newCondNode(equalsChar, 'e', '\0',
		NO_NEGATION);
	CondNode *setup2 = newCondNode(equalsChar, 'Z', '\0',
		NO_NEGATION);
	CondNode *setup3 = mergeNodes(setup1, setup2,
		OR_OPERATION);
	return setup3;
}

static TEST_RES(CondNode *, ClassWithDash, void) {
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

static TEST_RES(CondNode *, NegatedComplexClass, void) {
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
#endif
