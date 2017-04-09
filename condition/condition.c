#define CONDITION_PACKAGE

#include "../testing.h"
#include "builder.h"
#include "list.h"
#include "maker.h"
#include "node.h"
#include "parsestring.h"

CondNode *getCondition(char *str) {
	return buildCond(str);
}

void cleanCondition(CondNode *node) {
	cleanCondNode(node);
}

#ifdef TESTING_CONDITION_PACKAGE
int main() {
	testNode();
	testMakr();
	testGetCondList();
	testGetParseString();
	testBuildCond();
	return 0;
}
#endif
