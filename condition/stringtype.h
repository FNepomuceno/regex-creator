#ifndef CONDITION_STRINGTYPE_H
#define CONDITION_STRINGTYPE_H
#ifdef  CONDITION_PACKAGE

#include "../utils/bool.h"
int isEmptyCharString(char *str);
int isLoneCharString(char *str);
int isWildcardCharString(char *str);
int isEscapedCharString(char *str);
int isCharCategory(char *str);
int isNegatedCharCategory(char *str);
int getAmtCharsInClass(char *str);
char *getCharClassStartPosition(char *str);
const struct NegationTag *getCharClassNegationTag(char *str);
int nextDataSize(char *str, int num_chars);
int isClosedBracketExpr(char *str);
int isValidCharClass(char *str);
int isCharClass(char *str);
int isNegatedCharClass(char *str);
int getAmtDataInCharClass(char *str);

#endif
#endif
