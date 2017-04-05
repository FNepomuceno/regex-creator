#define CONDITION_PACKAGE

#include <ctype.h>
#include <stdlib.h>
#include "stringtype.h"
#include "tags.h"
#include "../utils/bool.h"
#include "../utils/test.h"

static int isLastCharInStr(char *str);
static int getAmtDataInBrackets(char *str, int num_chars);
static int getNumCharsInBrackets(char *str);

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

int isCharClass(char *str);
int isNegatedCharClass(char *str);
int getAmtCharsInClass(char *str) {
	if(isCharClass(str) == TRUE_BOOL) {
		return getNumCharsInBrackets(str);
	} else if(isNegatedCharClass(str) == TRUE_BOOL) {
		return getNumCharsInBrackets(str)-1;
	} else {
		return -1;
	}
}

char *getCharClassStartPosition(char *str) {
	if(isCharClass(str) == TRUE_BOOL) {
		return str + 1;
	} else if(isNegatedCharClass(str) == TRUE_BOOL) {
		return str + 2;
	} else {
		return "";
	}
}

const NegationTag *getCharClassNegationTag(char *str) {
	if(isCharClass(str) == TRUE_BOOL) {
		return NO_NEGATION;
	} else if(isNegatedCharClass(str) == TRUE_BOOL) {
		return YES_NEGATION;
	} else {
		return INVALID_NEGATION;
	}
}

int getAmtDataInCharClass(char *str) {
	char *cur_str = getCharClassStartPosition(str);
	int num_chars = getAmtCharsInClass(str);
	if(num_chars < 0) {
		return -1;
	}
	return getAmtDataInBrackets(cur_str, num_chars);
}

enum {
	SIZE_INVALID = -1,
	SIZE_LONE_CHAR = 1,
	SIZE_ESCAPED_CHAR = 2,
	SIZE_CHAR_RANGE = 3
};

static int isSelectionRange(char *str, int num_chars) {
	return num_chars >= SIZE_CHAR_RANGE && *(str+1) == '-';
}

static int isSelectionValidRange(char *str, int num_chars) {
	return isSelectionRange(str, num_chars) &&
		*(str) <= *(str+SIZE_CHAR_RANGE-1);
}

static int isSelectionEscapedChar(char *str, int num_chars) {
	return num_chars >= SIZE_ESCAPED_CHAR && *str == '\\';
}

static int isSelectionLoneChar(char *str, int num_chars) {
	return num_chars >= SIZE_LONE_CHAR;
}

int nextDataSize(char *str, int num_chars) {
	if(isSelectionValidRange(str, num_chars)) {
		return SIZE_CHAR_RANGE;
	} else if(isSelectionRange(str, num_chars)) {
		return SIZE_INVALID;
	} else if(isSelectionEscapedChar(str, num_chars)) {
		return SIZE_ESCAPED_CHAR;
	} else if(isSelectionLoneChar(str, num_chars)) {
		return SIZE_LONE_CHAR;
	} else {
		return SIZE_INVALID;
	}
}

static int getAmtDataInBrackets(char *str, int num_chars) {
	char *cur_str = str;
	int cur_num_chars = num_chars;
	int amt_data = 0;
	if(num_chars == 0) return -1;
	while(cur_num_chars > 0) {
		int data_size = nextDataSize(cur_str, cur_num_chars);
		if(data_size == SIZE_INVALID) {
			return -1;
		}
		amt_data++;
		cur_str = cur_str + data_size;
		cur_num_chars -= data_size;
	}
	return amt_data;
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
