#ifndef FUNCTION_INFO_H
#define FUNCTION_INFO_H

typedef int CondFunc(char, char, char);
typedef struct FuncInfo FuncInfo;
int isValidFunc(const FuncInfo *info, char arg1, char arg2);
int isEqualInfo(const FuncInfo *info1, const FuncInfo *info2);
const FuncInfo *getInfo(CondFunc *func);
CondFunc *getFunc(const FuncInfo *info);

int isDigitChar(char input, char, char);
int isWordChar(char input, char, char);
int isWhitespace(char input, char, char);
int equalsChar(char input, char target, char);
int inRangeChar(char input, char first, char last);
int otherwiseChar(char, char, char);


#endif
