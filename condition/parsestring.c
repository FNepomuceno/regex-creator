#define CONDITION_PACKAGE
//#define TEST

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "charstring.h"
#include "node.h"
#include "tags.h"
#include "../utils/bool.h"
static const int YES_NEGATE = 0;
static const int NO_NEGATE = 1;
static const int IGNORE_NEGATE = -1;

CondNode *parseCharString(char *str) {
	if(isValidCharString(str) == FALSE_BOOL) {
		return nil_node;
	}
	return nil_node;
}

void testParseCharString();
void testParseNull();
void testParseSingleChar();
void testParseCharCategory();
void testParseSimpleCharClass();
void testParseComplexCharClass();

#ifdef TEST
int main() {
	testParseCharString();
	return 0;
}
#endif

void testParseCharString() {
	testParseNull();
}

void testParseNull() {
	CondNode *test1 = parseCharString(NULL);
	CondNode *expected1 = nil_node;
	assert(isEquivalent(test1, expected1) == TRUE_BOOL);

	CondNode *test2 = parseCharString("");
	CondNode *expected2 = nil_node;
	assert(isEquivalent(test2, expected2) == TRUE_BOOL);
}

void testParseSingleChar() {
	CondNode *test1 = parseCharString("a");
	CondNode *expected1 = nil_node;
	assert(isEquivalent(test1, expected1) == TRUE_BOOL);

	CondNode *test2 = parseCharString("\\\\");
	CondNode *expected2 = nil_node;
	assert(isEquivalent(test2, expected2) == TRUE_BOOL);

	CondNode *test3 = parseCharString("\\.");
	CondNode *expected3 = nil_node;
	assert(isEquivalent(test3, expected3) == TRUE_BOOL);
}

void testParseCharCategory() {
	CondNode *test1 = parseCharString("\\d");
	CondNode *expected1 = nil_node;
	assert(isEquivalent(test1, expected1) == TRUE_BOOL);

	CondNode *test2 = parseCharString("\\D");
	CondNode *expected2 = nil_node;
	assert(isEquivalent(test2, expected2) == TRUE_BOOL);

	CondNode *test3 = parseCharString("\\w");
	CondNode *expected3 = nil_node;
	assert(isEquivalent(test3, expected3) == TRUE_BOOL);
}

void testParseSimpleCharClass() {
	CondNode *test1 = parseCharString("[C]");
	CondNode *expected1 = nil_node;
	assert(isEquivalent(test1, expected1) == TRUE_BOOL);

	CondNode *test2 = parseCharString("[^q]");
	CondNode *expected2 = nil_node;
	assert(isEquivalent(test2, expected2) == TRUE_BOOL);

	CondNode *test3 = parseCharString("[R-b]");
	CondNode *expected3 = nil_node;
	assert(isEquivalent(test3, expected3) == TRUE_BOOL);
}

void testParseComplexCharClass() {
	CondNode *test1 = parseCharString("[eZ]");
	CondNode *expected1 = nil_node;
	assert(isEquivalent(test1, expected1) == TRUE_BOOL);

	CondNode *test2 = parseCharString("[-#-%]");
	CondNode *expected2 = nil_node;
	assert(isEquivalent(test2, expected2) == TRUE_BOOL);

	CondNode *test3 = parseCharString("[^Wua-h?]");
	CondNode *expected3 = nil_node;
	assert(isEquivalent(test3, expected3) == TRUE_BOOL);
}
