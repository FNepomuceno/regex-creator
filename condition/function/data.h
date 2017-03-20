#ifndef FUNCTION_DATA_H
#define FUNCTION_DATA_H

#include "condfunc.h"
typedef struct FuncData FuncData;
extern FuncData *nil_data;

FuncData *newFuncData(CondFunc *func, char arg1, char arg2);
void cleanFuncData(FuncData *data);

int applyFunc(FuncData *data, char input);
int areFuncsEquivalent(FuncData *data1, FuncData *data2);

#endif
