#define MODULE_CHARTYPEREGEX_MOD

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "chartype.h"
#include "bool.h"

int lengthOfString(char *str) {
	return (str == NULL)?
		INVALID_CHAR_LENGTH:
		strlen(str);
}

int hasNoChar(char *str, int length) {
	return toBool(length <= 0 || str == NULL);
}

int hasParseLoneChar(char *str, int length) {
	return boolAnd(hasSingleChar(str, length),
		boolInvert(hasParseMetaChar(str, length)));
}

int hasGraphLoneChar(char *str, int length) {
	return boolAnd(hasSingleChar(str, length),
		boolInvert(hasMetaChar(str, length)));
}

static int hasSingleChar(char *str, int length) {
	return boolAnd(toBool(length >= 1),
		toBool(isprint(*str)));
}

static int hasParseMetaChar(char *str, int length) {
	return boolAnd(hasSingleChar(str, length),
		toBool(*str == '\\' ||
		*str == '.' ||
		*str == '['));
}

static int hasMetaChar(char *str, int length) {
	return boolAnd(hasSingleChar(str, length),
		boolOr(hasParseMetaChar(str, length),
		toBool(*str == '(' ||
		*str == ')' ||
		*str == '*' ||
		*str == '+' ||
		*str == '?' ||
		*str == '{' ||
		*str == '|' ||
		*str == '^' ||
		*str == '$')));
}

int hasWildcardChar(char *str, int length) {
	return toBool(length >= 1 &&
		*str == '.');
}

int hasParseSingleChar(char *str, int length) {
	return boolOr(hasParseLoneChar(str, length),
		hasWildcardChar(str, length));
}

int isParseSingleChar(char *str, int length) {
	return boolAnd(hasSingleChar(str, length),
		toBool(length == LONE_CHAR_LENGTH));
}

int hasCharCategory(char *str, int length) {
	return boolAnd(hasEscapedChar(str, length),
		toBool(*(str+1) == 'd' ||
		*(str+1) == 's' ||
		*(str+1) == 'w'));
}

int hasNegatedCharCategory(char *str, int length) {
	return boolAnd(hasEscapedChar(str, length),
		toBool(*(str+1) == 'D' ||
		*(str+1) == 'S' ||
		*(str+1) == 'W'));
}

int hasNormalEscapedChar(char *str, int length) {
	return boolAnd(hasEscapedChar(str, length),
		boolAnd(boolInvert(hasCharCategory(str, length)),
		boolInvert(hasNegatedCharCategory(str, length))));
}

int hasEscapedChar(char *str, int length) {
	return toBool(*str == '\\' && length >= ESCAPED_CHAR_LENGTH);
}

int isEscapedChar(char *str, int length) {
	return boolAnd(hasEscapedChar(str, length),
		toBool(length == ESCAPED_CHAR_LENGTH));
}

int charClassLength(char *str, int length) {
	int class_length = enclosedBracketLength(str, length);
	if(class_length < 0 ||
		(*(str+1) == '^' && class_length < 4) ||
		(*(str+1) != '^' && class_length < 3)) {
		return INVALID_CHAR_LENGTH;
	} else {
		return class_length;
	}
}

static int enclosedBracketLength(char *str, int length) {
	if(length < 1 || *str != '[') {
		return INVALID_CHAR_LENGTH;
	}
	for(int i = 1; i < length; i++) {
		if(*(str+i) == ']') {
			return i + 1;
		}
	}
	return INVALID_CHAR_LENGTH;
}

int hasCharClass(char *str, int length) {
	return toBool(charClassLength(str, length) > 0 &&
		*(str+1) != '^');
}

int hasNegatedCharClass(char *str, int length) {
	return toBool(charClassLength(str, length) > 0 &&
		*(str+1) == '^');
}

int amountDataInCharClass(char *str, int length) {
	char *cur_str = charClassStartPosition(str);
	int num_chars = classDataLength(str, length);
	if(num_chars < 0) {
		return INVALID_CHAR_LENGTH;
	}
	return amountDataInBrackets(cur_str, num_chars);
}

static char *charClassStartPosition(char *str) {
	if(*(str+1) == '^') {
		return str + 2;
	} else {
		return str + 1;
	}
}

int classDataLength(char *str, int length) {
	int class_length = charClassLength(str, length);
	if(class_length < 0) {
		return INVALID_CHAR_LENGTH;
	} else if(*(str+1) == '^') {
		return class_length - 3;
	} else {
		return class_length - 2;
	}
}

static int amountDataInBrackets(char *str, int num_chars) {
	char *cur_str = str;
	int cur_num_chars = num_chars;
	int amt_data = 0;
	if(num_chars == 0) return INVALID_CHAR_LENGTH;
	while(cur_num_chars > 0) {
		int data_size = dataSize(cur_str, cur_num_chars);
		if(data_size < 0) {
			return INVALID_CHAR_LENGTH;
		}
		amt_data++;
		cur_str = cur_str + data_size;
		cur_num_chars -= data_size;
	}
	return amt_data;
}

int dataSize(char *str, int length) {
	if(hasValidRange(str, length) == TRUE_BOOL) {
		return CHAR_RANGE_LENGTH;
	} else if(hasInvalidRange(str, length) == TRUE_BOOL) {
		return INVALID_CHAR_LENGTH;
	} else if(hasEscapedChar(str, length) == TRUE_BOOL) {
		return ESCAPED_CHAR_LENGTH;
	} else if(hasParseSingleChar(str, length) == TRUE_BOOL) {
		return LONE_CHAR_LENGTH;
	} else {
		return INVALID_CHAR_LENGTH;
	}
}

static int hasValidRange(char *str, int length) {
	return boolAnd(hasCharRange(str, length),
		toBool(*str <= *(str+CHAR_RANGE_LENGTH-1)));
}

static int hasInvalidRange(char *str, int length) {
	return boolAnd(hasCharRange(str, length),
		toBool(*str > *(str+CHAR_RANGE_LENGTH-1)));
}

static int hasCharRange(char *str, int length) {
	return toBool(length >= CHAR_RANGE_LENGTH &&
		*(str+1) == '-');
}

int isCharClassNegated(char *str,
		int length) {
	if(*(str+1) == '^') {
		return TRUE_BOOL;
	} else {
		return FALSE_BOOL;
	}
}

int hasOrChar(char *str, int length) {
	return toBool(length >= 1 && *str == '|');
}

int hasGroupBeginning(char *str, int length) {
	return toBool(length >= 1 && *str == '(');
}

int hasGroupEnding(char *str, int length) {
	return toBool(length >= 1 && *str == ')');
}

int hasTextBeginning(char *str, int length) {
	return toBool(length >= 1 && *str == '^');
}

int hasTextEnding(char *str, int length) {
	return toBool(length >= 1 && *str == '$');
}

int hasKleeneStar(char *str, int length) {
	return toBool(length >= 1 && *str == '*');
}

int hasPlusChar(char *str, int length) {
	return toBool(length >= 1 && *str == '+');
}

int hasQuestionMark(char *str, int length) {
	return toBool(length >= 1 && *str == '?');
}

int hasFixedRepetition(char *str, int length) {
	return toBool(enclosedBraceLength(str, length) > 0 &&
		indexOfCommaInBraces(str, length) < 0);
}
int hasVariableRepetition(char *str, int length) {
	if(enclosedBraceLength(str, length) <= 0 ||
			indexOfCommaInBraces(str, length) <= 1 ||
			indexOfCommaInBraces(str, length) >=
			(enclosedBraceLength(str, length)-2)) {
		return FALSE_BOOL;
	}
	int min_reps, max_reps;
	setVariableRepetitions(str, &min_reps, &max_reps);
	if(min_reps > max_reps) {
		return FALSE_BOOL;
	} else {
		return TRUE_BOOL;
	}
}
int hasInfiniteRepetition(char *str, int length) {
	return toBool(enclosedBraceLength(str, length) > 0 &&
		indexOfCommaInBraces(str, length) == 
			(enclosedBraceLength(str, length)-2));
}

int enclosedBraceLength(char *str, int length) {
	if(length < 1 || *str != '{') {
		return INVALID_CHAR_LENGTH;
	}
	int has_comma = FALSE_BOOL;
	for(int i = 1; i < length; i++) {
		if(*(str+i) == '}') {
			return i + 1;
		}
		if(*(str+i) == ',') {
			if(has_comma == TRUE_BOOL) {
				return INVALID_CHAR_LENGTH;
			}
			has_comma = TRUE_BOOL;
		} else if(*(str+i) < '0' || *(str+i) > '9') {
			return INVALID_CHAR_LENGTH;
		}
	}
	return INVALID_CHAR_LENGTH;
}

static int indexOfCommaInBraces(char *str, int length) {
	for(int i = 0; i < length; i++) {
		if(*(str+i) == ',') {
			return i;
		}
	}
	return INVALID_INDEX;
}

void setFixedRepetitions(char *str, int *p_num_reps) {
	sscanf(str, "{%d}", p_num_reps);
}

void setVariableRepetitions(char *str, int *p_min_reps,
		int *p_max_reps) {
	sscanf(str, "{%d,%d}", p_min_reps, p_max_reps);
}

void setInfiniteRepetitions(char *str, int *p_min_reps) {
	sscanf(str, "{%d,}", p_min_reps);
}
