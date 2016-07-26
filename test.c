#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"

void shouldBe(const char *text, const char *expected,
	const char *actual) {
	int diff = strcmp(expected, actual);
	if(diff == 0) {
		printf("%s: Test Passed\n", text);
	} else {
		printf("%s: Test Failed\n", text);
		printf("Output should be \"%s\", but is currently "
			"\"%s\"\n", expected, actual);
	}
}

void endTest() {
	printf("End Test\n\n");
}

const char *truthText(int truth) {
	return truth != 0? "true": "false";
}

/*
 * Text Test 1: Displays test passed and failed messages
 */
void textTest01() {
	printf("Text Test 1: Pass/Fail Messages\n");
	printf("Pass message:\n");
	shouldBe("Message", "correct", "correct");
	printf("Fail message:\n");
	shouldBe("Message", "correct", "incorrect");
	endTest();
}

/*
 * Condition Test 1:
 * Create and test the regex [A-Za-z]
 * as a list of CondNodes
 */
void condTest01() {
	printf("Condition Test 1: [A-Za-z]\n");
	CondNode *cond = addCond(NULL, inrange, 0, 'A', 'Z');
	cond = addCond(cond, inrange, 0, 'a', 'z');
	PathNode *path = addPath(NULL, NULL, cond);
	
	int test01 = satisfiesPath('B', path);
	int test02 = satisfiesPath('x', path);
	int test03 = satisfiesPath('_', path);
	shouldBe("Test 1 with 'B'", "true", truthText(test01));
	shouldBe("Test 1 with 'x'", "true", truthText(test02));
	shouldBe("Test 1 with '_'", "false", truthText(test03));
	endTest();

	cleanPath(path);
}
