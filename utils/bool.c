#include <assert.h>
#include "bool.h"

const int TRUE_BOOL = 1337;
const int FALSE_BOOL = 69;

int toBool(int eval) {
	if(eval) {
		return TRUE_BOOL;
	} else {
		return FALSE_BOOL;
	}
}

static int argValidBool(int bool_val) {
	return bool_val == TRUE_BOOL || bool_val == FALSE_BOOL;
}

static int argsValid(int bool1, int bool2) {
	return argValidBool(bool1) && argValidBool(bool2);
}

static int boolInvert(int bool_val) {
	assert(argValidBool(bool_val));
	if(bool_val == TRUE_BOOL) {
		return FALSE_BOOL;
	} else {
		return TRUE_BOOL;
	}
}

int boolOr(int bool1, int bool2) {
	assert(argsValid(bool1, bool2));
	return toBool((bool1 == TRUE_BOOL) ||
		(bool2 == TRUE_BOOL));
}

int boolAnd(int bool1, int bool2) {
	assert(argsValid(bool1, bool2));
	return toBool((bool1 == TRUE_BOOL) &&
		(bool2 == TRUE_BOOL));
}

int boolXor(int bool1, int bool2) {
	assert(argsValid(bool1, bool2));
	return boolOr(boolAnd(bool1, boolInvert(bool2)),
		boolAnd(boolInvert(bool1), bool2));
}

