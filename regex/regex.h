#ifndef MODULE_REGEX_H
#define MODULE_REGEX_H

void testRegex();

#ifdef MODULE_REGEX_MOD

#include "../utils/test.h"
char *INVALID_STRING_ARRAY_DATA[] = {""};
char *DEFAULT_STRING_ARRAY_DATA[] = {"WRONG"};
char **INVALID_STRING_ARRAY = INVALID_STRING_ARRAY_DATA;
char **DEFAULT_STRING_ARRAY = DEFAULT_STRING_ARRAY_DATA;

#endif
#endif
