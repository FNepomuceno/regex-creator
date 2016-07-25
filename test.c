#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"

static const char *truetext = "true";
static const char *falsetext = "false";

void shouldBe(const char *text, const char *expected,
	const char *actual) {
	printf("%s: should be %s, is actually %s\n",
		text, expected, actual);
}

const char *truthText(int truth) {
	return truth != 0? truetext: falsetext;
}

/*
 * Condition Test 1:
 * Create and test the regex [A-Za-z]
 * as a list of CondNodes
 */
void condTest01() {
	printf("Condition Test 1: [A-Za-z]\n");
	PathNode *path = malloc(sizeof(PathNode));
	path->conds = 
		appendCond(newCond(inrange, 0, 'a', 'z'),
		newCond(inrange, 0, 'A', 'Z'));
	
	int test01 = satisfiesPath('B', path);
	int test02 = satisfiesPath('x', path);
	int test03 = satisfiesPath('_', path);
	shouldBe("Test 1 with 'B'", truetext, truthText(test01));
	shouldBe("Test 1 with 'x'", truetext, truthText(test02));
	shouldBe("Test 1 with '_'", falsetext, truthText(test03));

	cleanCond(path->conds);
	free(path);
}
