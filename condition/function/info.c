#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "info.h"
#include "../../utils/bool.h"

struct FuncInfo {
	CondFunc *_func;
	int _num_args;
};

static const int INFO_SIZE = 6;
static const FuncInfo funcInfos[] = {
	{ isDigitChar, 0 },
	{ isWordChar, 0 },
	{ isWhitespace, 0 },
	{ equalsChar, 1 },
	{ inRangeChar, 2 },
	{ otherwiseChar, 0 }
};

static const FuncInfo nil_infodata;
static const FuncInfo *invalid_info = &nil_infodata;

int isValidFunc(const FuncInfo *info, char arg1, char arg2) {
	if(info == invalid_info) {
		return FALSE_BOOL;
	} else if(info->_num_args == 0) {
		return toBool(arg1 == '\0' && arg2 == '\0');
	} else if(info->_num_args == 1) {
		return toBool(arg2 == '\0');
	} else if(info->_func == inRangeChar) {
		return toBool(arg1 <= arg2);
	}
	return FALSE_BOOL;
}

int isEqualInfo(const FuncInfo *info1, const FuncInfo *info2) {
	return toBool(info1 == info2);
}

const FuncInfo *getInfo(CondFunc *func) {
	for(int x = 0; x < INFO_SIZE; x++) {
		if(funcInfos[x]._func == func) {
			return funcInfos + x;
		}
	}
	return invalid_info;
}

CondFunc *getFunc(const FuncInfo *info) {
	return info->_func;
}

int isDigitChar(char input, char ignore1, char ignore2) {
	return isdigit(input);
}

int isWordChar(char input, char ignore1, char ignore2) {
	return isalnum(input) || input == '_';
}

int isWhitespace(char input, char ignore1, char ignore2) {
	return isspace(input);
}

int equalsChar(char input, char target, char ignore) {
	return input == target;
}

int inRangeChar(char input, char first, char last) {
	return input >= first && input <= last;
}

int otherwiseChar(char ignore, char ignore1, char ignore2) {
	return (0 == 0);
}
