#define CONDITION_PACKAGE

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "stringtype.h"
#include "../testing.h"
#include "../utils/bool.h"
#include "../utils/test.h"

char *INVALID_STRING = "";

void cleanParseString(char *str) {
	if(str != NULL && str != INVALID_STRING) {
		free(str);
	}
}

static char *newSingleCharParseString();
static char *newLinkedDataParseString(char *str);
char *getParseString(char *str) {
	if(isEmptyCharString(str) == TRUE_BOOL) {
		return INVALID_STRING;
	}
	if(isLoneCharString(str) == TRUE_BOOL ||
			isEscapedCharString(str) == TRUE_BOOL) {
		return newSingleCharParseString();
	}
	if(isClosedBracketExpr(str) == FALSE_BOOL) {
		return INVALID_STRING;
	}
	return newLinkedDataParseString(str);
}

static char *newParseStringBase(int length);
static char *newSingleCharParseString() {
	return newParseStringBase(1);
}

static char *newParseStringBase(int length) {
	char *result = malloc((length+1) * sizeof (char));
	*(result+length) = '\0';
	memset(result, '#', length);
	return result;
}

static char *setOrLink(char *str, int size);
static char *setAndLink(char *str, int size);
static char *newLinkedDataParseString(char *str) {
	int amt_data = getAmtDataInCharClass(str);
	if(amt_data <= 0) return INVALID_STRING;

	char *result = newParseStringBase(2*amt_data-1);
	if(isCharClass(str) == TRUE_BOOL) {
		return setOrLink(result, 2*amt_data);
	} else if(isNegatedCharClass(str) == TRUE_BOOL) {
		return setAndLink(result, 2*amt_data);
	}
	return INVALID_STRING;
}

static char *setOrLink(char *str, int size) {
	int numOrs = (size-1)/2;
	for(int i = 0; i < numOrs; i++) {
		*(str+i*2) = '|';
	}
	return str;
}

static char *setAndLink(char *str, int size) {
	int numAnds = (size-1)/2;
	for(int i = 0; i < numAnds; i++) {
		*(str+i) = '&';
	}
	return str;
}

void testGetParseString();
#ifdef TEST_CONDITION_PARSESTRING
int main() {
	testGetParseString();
	return 0;
}
#endif

static void testGetParseStringNull();
static void testGetParseStringSingleChar();
static void testGetParseStringCharCategory();
static void testGetParseStringSimpleCharClass();
static void testGetParseStringComplexCharClass();
void testGetParseString() {
	testGetParseStringNull();
	testGetParseStringSingleChar();
	testGetParseStringCharCategory();
	testGetParseStringSimpleCharClass();
	testGetParseStringComplexCharClass();
}

static void testGetParseStringNull() {
	char *test1 = getParseString(NULL);
	TEST(strcmp(test1, INVALID_STRING) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("");
	TEST(strcmp(test2, INVALID_STRING) == 0);
	cleanParseString(test2);
}

static void testGetParseStringSingleChar() {
	char *test1 = getParseString("a");
	char *expected1 = "#";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("\\\\");
	char *expected2 = "#";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test2);

	char *test3 = getParseString("\\.");
	char *expected3 = "#";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test3);
}

static void testGetParseStringCharCategory() {
	char *test1 = getParseString("\\d");
	char *expected1 = "#";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("\\D");
	char *expected2 = "#";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test2);

	char *test3 = getParseString("\\w");
	char *expected3 = "#";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test3);
}

static void testGetParseStringSimpleCharClass() {
	char *test1 = getParseString("[C]");
	char *expected1 = "#";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("[^q]");
	char *expected2 = "#";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test2);

	char *test3 = getParseString("[R-b]");
	char *expected3 = "#";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test3);

	char *test4 = getParseString("[^\\d]");
	char *expected4 = "#";
	TEST(strcmp(test4, expected4) == 0);
	cleanParseString(test4);
}

static void testGetParseStringComplexCharClass() {
	char *test1 = getParseString("[eZ]");
	char *expected1 = "|##";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("[-#-%!]");
	char *expected2 = "|#|##";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test2);

	char *test3 = getParseString("[^Wua-h?]");
	char *expected3 = "&&&####";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test3);
}
