#define CONDITION_PACKAGE
//#define TEST_CONDITION_PARSESTRING

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "stringtype.h"
#include "../utils/bool.h"
#include "../utils/test.h"

char *INVALID_STRING = "";
char *newSingleCharParseString();
char *newLinkedDataParseString(char *str);
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

char *setOrLink(char *str, int size) {
	int numOrs = (size-1)/2;
	for(int i = 0; i < numOrs; i++) {
		*(str+i*2) = '|';
	}
	return str;
}

char *setAndLink(char *str, int size) {
	int numAnds = (size-1)/2;
	for(int i = 0; i < numAnds; i++) {
		*(str+i) = '&';
	}
	return str;
}

static char *newParseStringBase(int length) {
	char *result = malloc((length+1) * sizeof (char));
	*(result+length) = '\0';
	memset(result, '#', length);
	return result;
}

char *newLinkedDataParseString(char *str) {
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

char *newSingleCharParseString() {
	char *result = malloc(2 * sizeof(char));
	*result = '#';
	*(result+1) = '\0';
	return result;
}

void cleanParseString(char *str) {
	if(str != NULL && str != INVALID_STRING) {
		free(str);
	}
}

//TODO: Put the parseString and condList together
//	so the Assembler(not yet made) can put them together
//TODO: Make the Assembler (assembler.c)
void testGetParseString();
#ifdef TEST_CONDITION_PARSESTRING
int main() {
	testGetParseString();
	return 0;
}
#endif

void testGetParseStringNull();
void testGetParseStringSingleChar();
void testGetParseStringCharCategory();
void testGetParseStringSimpleCharClass();
void testGetParseStringComplexCharClass();
void testGetParseString() {
	testGetParseStringNull();
	testGetParseStringSingleChar();
	testGetParseStringCharCategory();
	testGetParseStringSimpleCharClass();
	testGetParseStringComplexCharClass();
}

void testGetParseStringNull() {
	char *test1 = getParseString(NULL);
	TEST(strcmp(test1, INVALID_STRING) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("");
	TEST(strcmp(test2, INVALID_STRING) == 0);
	cleanParseString(test2);
}

void testGetParseStringSingleChar() {
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

void testGetParseStringCharCategory() {
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

void testGetParseStringSimpleCharClass() {
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
}

void testGetParseStringComplexCharClass() {
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
