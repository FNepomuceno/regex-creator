//#define TEST

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/bool.h"

int isValidCharString(char *str);
int isEmptyCharString(char *str);
int isSingleCharString(char *str);
int isLastCharInStr(char *str);
int isValidLoneChar(char c);
int isEscapedCharString(char *str);
int isClosedBracketExpr(char *str);
int isValidCharClass(char *str);
int getNumCharsInBrackets(char *str);
int isValidRange(char *str);

int isValidCharString(char *str) {
	if(isEmptyCharString(str) == TRUE_BOOL) return FALSE_BOOL;
	if(isSingleCharString(str) == TRUE_BOOL) return TRUE_BOOL;
	if(isEscapedCharString(str) == TRUE_BOOL) return TRUE_BOOL;
	if(isClosedBracketExpr(str) == FALSE_BOOL) return FALSE_BOOL;
	return isValidCharClass(str);
}

int isEmptyCharString(char *str) {
	return toBool(str == NULL || *str == '\0');
}

int isSingleCharString(char *str) {
	if(isLastCharInStr(str) == FALSE_BOOL) {
		return FALSE_BOOL;
	}
	return isValidLoneChar(*str);
}

int isLastCharInStr(char *str) {
	return toBool(*(str+1) == '\0');
}

int isValidLoneChar(char c) {
	return toBool(isprint(c) && c != '\\' && c != '[');
}

int isEscapedCharString(char *str) {
	return boolAnd(toBool(*str == '\\'),
		isLastCharInStr(str+1));
}

int isClosedBracketExpr(char *str) {
	if(*str != '[') return FALSE_BOOL;
	char *cur_str = str+1;
	while(*cur_str != '\0') {
		if(*cur_str == ']') {
			return isLastCharInStr(cur_str);
		}
		cur_str = cur_str+1;
	}
	return FALSE_BOOL;
}

int isValidCharClass(char *str) {
	if(isClosedBracketExpr(str) == FALSE_BOOL) abort();
	int num_chars = getNumCharsInBrackets(str);
	char *cur_str = str+1;
	if(*cur_str == '^') {
		cur_str = cur_str+1;
		num_chars -= 1;
	}
	if(num_chars == 0) {
		return FALSE_BOOL;
	}
	while(num_chars >= 3) {
		if(*(cur_str+1) == '-') {
			if(isValidRange(cur_str) == FALSE_BOOL) {
				return FALSE_BOOL;
			}
			cur_str = cur_str+3;
			num_chars-=3;
		} else {
			cur_str = cur_str+1;
			num_chars-=1;
		}
	}
	return TRUE_BOOL;
}

int getNumCharsInBrackets(char *str) {
	if(isClosedBracketExpr(str) == FALSE_BOOL) abort();
	int result = 0;
	char *cur_str = str+1;
	while(*cur_str != ']') {
		result++;
		cur_str = cur_str+1;
	}
	return result;
}

int isValidRange(char *str) {
	if(*(str+1) != '-') abort();
	char start = *str;
	char end = *(str+2);
	return toBool(start <= end);
}

void testIsValidCharString();
void testBareCharValid();
void testEscapedCharValid();
void testLoneBracketValid();
void testMisenclosedBracketValid();
void testLoneLetterCharClassValid();
void testLoneRangeCharClassValid();
void testTwoLetterCharClassValid();
void testDashInCharClassValid();
void testNegatedCharClassValid();
void testMultipleInCharClassValid();

#ifdef TEST
int main() {
	printf("TEST START\n");
	testIsValidCharString();
	printf("TEST OVER\n");
	return 0;
}
#endif

void testIsValidCharString() {
	testBareCharValid();
	testEscapedCharValid();
	testLoneBracketValid();
	testMisenclosedBracketValid();
	testLoneLetterCharClassValid();
	testLoneRangeCharClassValid();
	testTwoLetterCharClassValid();
	testDashInCharClassValid();
	testNegatedCharClassValid();
	testMultipleInCharClassValid();
}

void testBareCharValid(){
	char *test1 = NULL;
	assert(isValidCharString(test1) == FALSE_BOOL);

	char *test2 = "";
	assert(isValidCharString(test2) == FALSE_BOOL);

	char *test3 = "a";
	assert(isValidCharString(test3) == TRUE_BOOL);

	char *test4 = "\\";
	assert(isValidCharString(test4) == FALSE_BOOL);
}

void testEscapedCharValid(){
	char *test1 = "\\.";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "\\a";
	assert(isValidCharString(test2) == TRUE_BOOL);

	char *test3 = "\\^";
	assert(isValidCharString(test3) == TRUE_BOOL);

	char *test4 = "\\W";
	assert(isValidCharString(test4) == TRUE_BOOL);

	char *test5 = "\\\\";
	assert(isValidCharString(test5) == TRUE_BOOL);
}

void testLoneBracketValid(){
	char *test1 = "[";
	assert(isValidCharString(test1) == FALSE_BOOL);

	char *test2 = "]";
	assert(isValidCharString(test2) == TRUE_BOOL);

	char *test3 = "(";
	assert(isValidCharString(test3) == TRUE_BOOL);

	char *test4 = ")";
	assert(isValidCharString(test4) == TRUE_BOOL);
}

void testMisenclosedBracketValid(){
	char *test1 = "[]";
	assert(isValidCharString(test1) == FALSE_BOOL);

	char *test2 = "[^]";
	assert(isValidCharString(test2) == FALSE_BOOL);
	
	char *test3 = "[e";
	assert(isValidCharString(test3) == FALSE_BOOL);

	char *test4 = "[^";
	assert(isValidCharString(test4) == FALSE_BOOL);
}

void testLoneLetterCharClassValid(){
	char *test1 = "[e]";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "[f]x";
	assert(isValidCharString(test2) == FALSE_BOOL);
}

void testLoneRangeCharClassValid(){
	char *test1 = "[\\-a]";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "[a-\\]";
	assert(isValidCharString(test2) == FALSE_BOOL);

	char *test3 = "[u-u]";
	assert(isValidCharString(test3) == TRUE_BOOL);
}

void testTwoLetterCharClassValid(){
	char *test1 = "[eZ]";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "[\\c]";
	assert(isValidCharString(test2) == TRUE_BOOL);

	char *test3 = "[c\\]";
	assert(isValidCharString(test3) == TRUE_BOOL);
}

void testDashInCharClassValid(){
	char *test1 = "[-#-g]";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "[#-g-]";
	assert(isValidCharString(test2) == TRUE_BOOL);

	char *test3 = "[--x]";
	assert(isValidCharString(test3) == TRUE_BOOL);

	char *test4 = "[x--]";
	assert(isValidCharString(test4) == FALSE_BOOL);

	char *test5 = "[i-]";
	assert(isValidCharString(test5) == TRUE_BOOL);

	char *test6 = "[-i]";
	assert(isValidCharString(test6) == TRUE_BOOL);
}

void testNegatedCharClassValid(){
	char *test1 = "[^eat]";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "[^^._]";
	assert(isValidCharString(test2) == TRUE_BOOL);
}

void testMultipleInCharClassValid(){
	char *test1 = "[^bi-n]";
	assert(isValidCharString(test1) == TRUE_BOOL);

	char *test2 = "[^H-O-l-y]";
	assert(isValidCharString(test2) == TRUE_BOOL);

	char *test3 = "[3Y-e5-8\\]";
	assert(isValidCharString(test3) == TRUE_BOOL);
}
