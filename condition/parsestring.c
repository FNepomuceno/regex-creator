#define CONDITION_PACKAGE
#define CONDITION_PARSESTRING_MODULE

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../testing.h"
#include "parsestring.h"
#include "../utils/bool.h"
#include "../utils/chartype.h"
#include "../utils/test.h"

char *INVALID_STRING = "";

void cleanParseString(char *str) {
	if(str != NULL && str != INVALID_STRING) {
		free(str);
	}
}

char *getParseString(char *str) {
	int length = lengthOfString(str);
	if(hasNoChar(str, length) == TRUE_BOOL) {
		return INVALID_STRING;
	}
	if(isParseSingleChar(str, length) == TRUE_BOOL ||
			isEscapedChar(str, length) == TRUE_BOOL) {
		return newSingleCharParseString();
	}
	if(charClassLength(str, length) != length) {
		return INVALID_STRING;
	}
	return newLinkedDataParseString(str, length);
}

static char *newSingleCharParseString() {
	return newParseStringBase(1);
}

static char *newParseStringBase(int length) {
	char *result = malloc((length+1) * sizeof (char));
	*(result+length) = '\0';
	memset(result, '#', length);
	return result;
}

static char *newLinkedDataParseString(char *str, int length) {
	int amt_data = amountDataInCharClass(str, length);
	if(amt_data <= 0) return INVALID_STRING;

	char *result = newParseStringBase(2*amt_data-1);
	if(hasCharClass(str, length) == TRUE_BOOL) {
		return setOrLink(result, 2*amt_data);
	} else if(hasNegatedCharClass(str, length) == TRUE_BOOL) {
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

#ifdef TESTING_CONDITION_PARSESTRING
#ifdef TEST_THIS
int main() {
	USE_CASE(GetParseString);
	return 0;
}
#endif

TEST_CASE(GetParseString) {
	USE_CASE(GetParseStringNull);
	USE_CASE(GetParseStringSingleChar);
	USE_CASE(GetParseStringCharCategory);
	USE_CASE(GetParseStringSimpleCharClass);
	USE_CASE(GetParseStringComplexCharClass);
}

static TEST_CASE(GetParseStringNull) {
	COMPARE_TEST(char *, Null,
		getParseString(NULL),
		INVALID_STRING,
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, Nil,
		getParseString(""),
		INVALID_STRING,
		strcmp, 0,
		cleanParseString);
}

static TEST_CASE(GetParseStringSingleChar) {
	COMPARE_TEST(char *, LoneChar,
		getParseString("a"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, EscapedLiteral,
		getParseString("\\g"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, EscapedMeta,
		getParseString("\\."),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
}

static TEST_CASE(GetParseStringCharCategory) {
	COMPARE_TEST(char *, DigitCategory,
		getParseString("\\d"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, NegatedDigitCategory,
		getParseString("\\D"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, WordCategory,
		getParseString("\\w"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
}

static TEST_CASE(GetParseStringSimpleCharClass) {
	COMPARE_TEST(char *, SingleCharClass,
		getParseString("[C]"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, NegatedSingleCharClass,
		getParseString("[^q]"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, SingleRangeClass,
		getParseString("[R-b]"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, EscapedCharClass,
		getParseString("[^\\d]"),
		USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanParseString);
}

static TEST_CASE(GetParseStringComplexCharClass) {
	COMPARE_TEST(char *, DoubleLetterClass,
		getParseString("[eZ]"),
		USE_RES(CopyStr,"|##"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, ClassWithDash,
		getParseString("[-#-%!]"),
		USE_RES(CopyStr,"|#|##"),
		strcmp, 0,
		cleanParseString);
	COMPARE_TEST(char *, NegatedComplexClass,
		getParseString("[^Wua-h?]"),
		USE_RES(CopyStr,"&&&####"),
		strcmp, 0,
		cleanParseString);
}
#endif
