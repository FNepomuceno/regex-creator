#define CONDITION_PACKAGE
#define TEST_CONDITION_PARSESTRING

#include <string.h>
#include <stdlib.h>
#include "charstring.h"
#include "../utils/bool.h"
#include "../utils/test.h"

char *INVALID_STRING = "";
char *getParseString(char *str) {
	if(isValidCharString(str) == FALSE_BOOL) {
		return INVALID_STRING;
	}
	return INVALID_STRING;
}

void cleanParseString(char *str) {
	if(str != NULL && str != INVALID_STRING) {
		free(str);
	}
}

//TODO: Make the tests for getParseString() pass
//TODO: Make the CondList and extractCondList()
//TODO: Make tests for extractCondList() and make them pass
//TODO: Put the parseString and condList together
//	so the Assembler(not yet made) can put them together
//TODO: Make the Assembler (assembler.c)
void testGetParseString();
#ifdef TEST_CONDITION_PARSESTRING
int main() {
	testIsValidCharString();
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
	cleanParseString(test1);

	char *test3 = getParseString("\\.");
	char *expected3 = "#";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test1);
}

void testGetParseStringCharCategory() {
	char *test1 = getParseString("\\d");
	char *expected1 = "#";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("\\D");
	char *expected2 = "#";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test1);

	char *test3 = getParseString("\\w");
	char *expected3 = "#";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test1);
}

void testGetParseStringSimpleCharClass() {
	char *test1 = getParseString("[C]");
	char *expected1 = "#";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("[^q]");
	char *expected2 = "#";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test1);

	char *test3 = getParseString("[R-b]");
	char *expected3 = "#";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test1);
}

void testGetParseStringComplexCharClass() {
	char *test1 = getParseString("[eZ]");
	char *expected1 = "|##";
	TEST(strcmp(test1, expected1) == 0);
	cleanParseString(test1);

	char *test2 = getParseString("[-#-%!]");
	char *expected2 = "|#|##";
	TEST(strcmp(test2, expected2) == 0);
	cleanParseString(test1);

	char *test3 = getParseString("[^Wua-h?]");
	char *expected3 = "&&###";
	TEST(strcmp(test3, expected3) == 0);
	cleanParseString(test1);
}
