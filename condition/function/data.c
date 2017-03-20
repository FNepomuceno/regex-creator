#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "info.h"
#include "../../utils/bool.h"

struct FuncData {
	const FuncInfo *_func_info;
	char _arg1, _arg2;
};

static FuncData nil_funcdata;
FuncData *nil_data = &nil_funcdata;
static FuncData *invalid_data = &nil_funcdata;

FuncData *newFuncData(CondFunc *func, char arg1, char arg2) {
	if(isValidFunc(getInfo(func), arg1, arg2) == FALSE_BOOL) {
		return invalid_data;
	}
	FuncData *result = malloc(sizeof (FuncData));
	result->_func_info = getInfo(func);
	
	result->_arg1 = arg1;
	result->_arg2 = arg2;
	return result;
}

void cleanFuncData(FuncData *data) {
	if(data == nil_data || data == invalid_data) {
		return;
	}
	free(data);
}

int applyFunc(FuncData *data, char input) {
	assert(data != nil_data && data != invalid_data);
	return toBool(getFunc(data->_func_info)(input,
		data->_arg1, data->_arg2));
}

int areFuncsEquivalent(FuncData *data1, FuncData *data2) {
	assert(data1 != invalid_data && data2 != invalid_data);
	return (data1 == nil_data && data2 == nil_data) ||
		(isEqualInfo(data1->_func_info, data2->_func_info)
			== TRUE_BOOL &&
		data1->_arg1 == data2->_arg1 &&
		data1->_arg2 == data2->_arg2);
}
