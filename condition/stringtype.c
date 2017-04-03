#include <ctype.h>
#include <stdlib.h>
#include "stringtype.h"
#include "../utils/bool.h"
#include "../utils/test.h"

static int isLastCharInStr(char *str);
static int isCharRange(char *str);
static int getAmtDataInBrackets(char *str, int num_chars);
static int getNumCharsInBrackets(char *str);
static int isValidRange(char *str);

int isEmptyCharString(char *str) {
	return toBool(str == NULL || *str == '\0');
}

int isLoneCharString(char *str) {
	if(isLastCharInStr(str) == FALSE_BOOL) {
		return FALSE_BOOL;
	}
	return toBool(isprint(*str) && *str != '\\' && *str != '[');
}

static int isLastCharInStr(char *str) {
	return toBool(*(str+1) == '\0');
}

int isWildcardCharString(char *str) {
	return boolAnd(isLoneCharString(str),
		toBool(*str == '.'));
}

int isEscapedCharString(char *str) {
	return boolAnd(toBool(*str == '\\'),
		isLastCharInStr(str+1));
}

int isCharCategory(char *str) {
	return boolAnd(isEscapedCharString(str),
		toBool(*(str+1) == 'w' || *(str+1) == 'd' ||
		*(str+1) == 's'));
}

int isNegatedCharCategory(char *str) {
	return boolAnd(isEscapedCharString(str),
		toBool(*(str+1) == 'W' || *(str+1) == 'D' ||
		*(str+1) == 'S'));
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

static int isCharRange(char *str) {
	return toBool(*(str+1) == '-');
}

int isCharClass(char *str);
int isNegatedCharClass(char *str);
int getAmtDataInCharClass(char *str) {
	char *cur_str;
	int num_chars;
	if(isCharClass(str) == TRUE_BOOL) {
		cur_str = str+1;
		num_chars = getNumCharsInBrackets(str);
	}
	else if(isNegatedCharClass(str) == TRUE_BOOL) {
		cur_str = str+2;
		num_chars = getNumCharsInBrackets(str)-1;
	}
	else {
		return -1;
	}
	return getAmtDataInBrackets(cur_str, num_chars);
}

static int getAmtDataInBrackets(char *str, int num_chars) {
	char *cur_str = str;
	int cur_num_chars = num_chars;
	int amt_data = 0;
	if(num_chars == 0) return -1;
	while(cur_num_chars >= 3) {
		if(isCharRange(cur_str) == FALSE_BOOL) {
			cur_str = cur_str+1;
			cur_num_chars -= 1;
			amt_data += 1;
		} else if(isValidRange(cur_str) == FALSE_BOOL) {
			return -1;
		} else {
			cur_str = cur_str+3;
			cur_num_chars -= 3;
			amt_data += 1;
		}
	}
	return amt_data + cur_num_chars;
}

int isValidCharClass(char *str) {
	return boolAnd(boolOr(isCharClass(str),
		isNegatedCharClass(str)),
		toBool(getAmtDataInCharClass(str) > 0));
}

int isCharClass(char *str) {
	return boolAnd(isClosedBracketExpr(str),
		toBool(*(str+1) != '^'));
}

int isNegatedCharClass(char *str) {
	return boolAnd(isClosedBracketExpr(str),
		toBool(*(str+1) == '^'));
}

static int getNumCharsInBrackets(char *str) {
	if(isClosedBracketExpr(str) == FALSE_BOOL) abort();
	int result = 0;
	char *cur_str = str+1;
	while(*cur_str != ']') {
		result++;
		cur_str = cur_str+1;
	}
	return result;
}

static int isValidRange(char *str) {
	if(*(str+1) != '-') abort();
	char start = *str;
	char end = *(str+2);
	return toBool(start <= end);
}
