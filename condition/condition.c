#define CONDITION_PACKAGE
#define TEST_CONDITION

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

#ifdef TEST_CONDITION
int main() {
	testNodes();
	testMakr();
	testGetCondList();
	testGetParseString();
	testBuildCond();
	return 0;
}
#endif
