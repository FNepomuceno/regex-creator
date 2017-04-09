#ifndef CONDITION_PARSESTRING_H
#define CONDITION_PARSESTRING_H

#ifdef MODULE_TESTING_H
#ifdef TESTING_PROGRAM
#define TESTING_CONDITION_PACKAGE
#endif
#ifdef TESTING_CONDITION_PACKAGE
#define TESTING_CONDITION_PARSESTRING
#else
#define TEST_THIS
#endif
#ifdef TESTING_CONDITION_PARSESTRING
#include "../utils/test.h"
#endif
#endif


#ifdef  CONDITION_PACKAGE

char *getParseString(char *str);
void cleanParseString(char *str);

extern char *INVALID_STRING;

#ifdef TESTING_CONDITION_PARSESTRING
TEST_CASE(GetParseString);
#endif
#endif


#ifdef CONDITION_PARSESTRING_MODULE

static char *newSingleCharParseString();
static char *newParseStringBase(int length);
static char *newLinkedDataParseString(char *str, int length);
static char *setOrLink(char *str, int size);
static char *setAndLink(char *str, int size);

#ifdef TESTING_CONDITION_PARSESTRING
static TEST_CASE(GetParseStringNull);
static TEST_CASE(GetParseStringSingleChar);
static TEST_CASE(GetParseStringCharCategory);
static TEST_CASE(GetParseStringSimpleCharClass);
static TEST_CASE(GetParseStringComplexCharClass);
#endif
#endif

#endif
