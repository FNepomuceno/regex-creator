#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/bool.h"

int isSingleCharString(char *str);
int isValidCharString(char *str) {
	if(isSingleCharString(str) == TRUE_BOOL) {
		return TRUE_BOOL;
	}
	return FALSE_BOOL;
}

int isLastCharInStr(char *str);
int isValidLoneChar(char c);
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

int testIsValidCharString();
int main() {
	printf("TEST START\n");
	testIsValidCharString();
	printf("TEST OVER\n");
	return 0;
}

int testIsValidCharString() {
	char *test1 = "a";
	char *test2 = "\\";
	assert(isValidCharString(test1) == TRUE_BOOL);
	assert(isValidCharString(test2) == FALSE_BOOL);

	char *test3 = "\\.";
	char *test4 = "\\a";
	char *test5 = "\\^";
	char *test6 = "\\W";
	char *test7 = "\\\\";
	//assert(isValidCharString(test3) == TRUE_BOOL);
	//assert(isValidCharString(test4) == TRUE_BOOL);
	//assert(isValidCharString(test5) == TRUE_BOOL);
	//assert(isValidCharString(test6) == TRUE_BOOL);
	//assert(isValidCharString(test7) == TRUE_BOOL);

	char *test8 = "[";
	char *test9 = "]";
	char *test10 = "(";
	char *test11 = ")";
	//assert(isValidCharString(test8) == FALSE_BOOL);
	//assert(isValidCharString(test9) == TRUE_BOOL);
	//assert(isValidCharString(test10) == TRUE_BOOL);
	//assert(isValidCharString(test11) == TRUE_BOOL);

	char *test12 = "[]";
	char *test13 = "[^]"
	char *test14 = "[e";
	char *test15 = "[^";
	//assert(isValidCharString(test12) == FALSE_BOOL);
	//assert(isValidCharString(test13) == FALSE_BOOL);
	//assert(isValidCharString(test14) == FALSE_BOOL);
	//assert(isValidCharString(test15) == FALSE_BOOL);

	char *test16 = "[e]";
	char *test17 = "[f]x";
	char *test18 = "[\\-a]";
	char *test19 = "[a-\\]";
	//assert(isValidCharString(test16) == TRUE_BOOL);
	//assert(isValidCharString(test17) == FALSE_BOOL);
	//assert(isValidCharString(test18) == TRUE_BOOL);
	//assert(isValidCharString(test19) == FALSE_BOOL);

	char *test20 = "[-#-g]";
	char *test21 = "[#-g-]";
	char *test22 = "[--x]";
	char *test23 = "[x--]";
	//assert(isValidCharString(test20) == TRUE_BOOL);
	//assert(isValidCharString(test21) == TRUE_BOOL);
	//assert(isValidCharString(test22) == TRUE_BOOL);
	//assert(isValidCharString(test23) == FALSE_BOOL);

	char *test24 = "[^e]";
	char *test25 = "[^^]";
	//assert(isValidCharString(test24) == TRUE_BOOL);
	//assert(isValidCharString(test25) == TRUE_BOOL);

	char *test26 = "[^bi-n]";
	char *test27 = "[^H-O-l-y]";
	char *test28 = "[3Y-e5-8\\]";
	//assert(isValidCharString(test26) == TRUE_BOOL);
	//assert(isValidCharString(test27) == TRUE_BOOL);
	//assert(isValidCharString(test28) == TRUE_BOOL);
}
