#define CONDITION_PACKAGE

#include "builder.h"
#include "list.h"
#include "maker.h"
#include "node.h"
#include "parsestring.h"
#include "../testing.h"

CondNode *getCondition(char *str) {
	return buildCond(str);
}

void cleanCondition(CondNode *node) {
	cleanCondNode(node);
}

#ifdef TEST_CONDITION_PACKAGE
int main() {
	testNodes();
	testMakr();
	testGetCondList();
	testGetParseString();
	testBuildCond();
	return 0;
}
#endif
