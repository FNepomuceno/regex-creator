#define MODULE_REGEX_MOD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex.h"
#include "testing.h"

char **getParseRegex(char *str) {
	return DEFAULT_STRING_ARRAY;
}

void testRegex(void);
#ifdef TESTING_REGEX
int main(void) {
	testRegex();
	return 0;
}
#endif

//TODO: Make tests for getParseRegex
//TODO: Finish implementation of getParseRegex

