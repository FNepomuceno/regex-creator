#define MODULE_CHARTYPEREGEX_MOD

#include <ctype.h>
#include <stdio.h>
#include "chartyperegex.h"
#include "../utils/bool.h"
//TODO later: migrate this to the utils package

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
		*str == '['));
}

static int hasMetaChar(char *str, int length) {
	return boolAnd(hasSingleChar(str, length),
		boolOr(hasParseMetaChar(str, length),
		toBool(*str == '(' ||
		*str == ')' ||
		*str == '*' ||
		*str == '+' ||
		*str == '.' ||
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

static int hasEscapedChar(char *str, int length) {
	return toBool(*str == '\\' && length >= 2);
}

int charClassLength(char *str, int length) {
	int class_length = enclosedBracketLength(str, length);
	if(class_length < 0 ||
		(*(str+1) == '^' && class_length < 4) ||
		(*(str+1) != '^' && class_length < 3)) {
		return -1;
	} else {
		return class_length;
	}
}

int hasCharClass(char *str, int length) {
	return toBool(charClassLength(str, length) > 0 &&
		*(str+1) != '^');
}

int hasNegatedCharClass(char *str, int length) {
	return toBool(charClassLength(str, length) > 0 &&
		*(str+1) == '^');
}

static int enclosedBracketLength(char *str, int length) {
	if(length < 1 || *str != '[') {
		return -1;
	}
	for(int i = 1; i < length; i++) {
		if(*(str+i) == ']') {
			return i + 1;
		}
	}
	return -1;
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
		return -1;
	}
	int has_comma = FALSE_BOOL;
	for(int i = 1; i < length; i++) {
		if(*(str+i) == '}') {
			return i + 1;
		}
		if(*(str+i) == ',') {
			if(has_comma == TRUE_BOOL) {
				return -1;
			}
			has_comma = TRUE_BOOL;
		} else if(*(str+i) < '0' || *(str+i) > '9') {
			return -1;
		}
	}
	return -1;
}

static int indexOfCommaInBraces(char *str, int length) {
	for(int i = 0; i < length; i++) {
		if(*(str+i) == ',') {
			return i;
		}
	}
	return -1;
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
