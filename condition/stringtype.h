#ifndef CONDITION_STRINGTYPE_H
#define CONDITION_STRINGTYPE_H

#include "../utils/bool.h"
int isEmptyCharString(char *str);
int isLoneCharString(char *str);
int isEscapedCharString(char *str);
int isCharCategory(char *str);
int isNegatedCharCategory(char *str);
int isClosedBracketExpr(char *str);
int isCharClass(char *str);
int isNegatedCharClass(char *str);
int getAmtDataInBrackets(char *str, int num_chars);
int getNumCharsInBrackets(char *str);

#endif