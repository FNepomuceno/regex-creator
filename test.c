#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"
#include "stak.h"
//#define __DEBUG__

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
	CondStakNode *stak = startStak();
	stak = addOr(stak);
	stak = addCond(stak, inrange, 0, 'A', 'Z');
	stak = addCond(stak, inrange, 0, 'a', 'z');
	
	CondNode *cond = extractCond(stak);
	PathNode *path = addPath(NULL, cond);
	cleanStak(stak);
	
	int test01 = satisfiesPath('B', path);
	int test02 = satisfiesPath('x', path);
	int test03 = satisfiesPath('_', path);
	shouldBe("Test 1 with 'B'", "true", truthText(test01));
	shouldBe("Test 1 with 'x'", "true", truthText(test02));
	shouldBe("Test 1 with '_'", "false", truthText(test03));
	endTest();

	cleanPath(path);
}

void startTest() {
#ifdef __DEBUG__
	printf("\nstart test\n");
#endif
}

void condTest02() {
	printf("Condition Test 2: Tree of conditions\n");
	CondStakNode *stak = startStak();
	stak = addOr(stak); stak = addAnd(stak);
	stak = addCond(stak, inrange, 0, 'A', 'Z');
	stak = addOr(stak); stak = addAnd(stak);
	stak = addCond(stak, inrange, 0, 'A', 'N');
	stak = addCond(stak, inrange, 0, 'A', 'F');
	stak = addCond(stak, inrange, 0, 'V', 'Z');
	stak = addOr(stak); stak = addAnd(stak);
	stak = addCond(stak, inrange, 0, 'a', 'z');
	stak = addCond(stak, inrange, 0, 'o', 'u');
	stak = addCond(stak, matches, 0, '_', 0);
	
	CondNode *cond = extractCond(stak);
	PathNode *path = addPath(NULL, cond);
	cleanStak(stak);
	
	int test01 = satisfiesPath('D', path);
	int test02 = satisfiesPath('P', path);
	int test03 = satisfiesPath('Y', path);
	int test04 = satisfiesPath('b', path);
	int test05 = satisfiesPath('s', path);
	int test06 = satisfiesPath('%', path);
	int test07 = satisfiesPath('_', path);

	shouldBe("Test 1 with 'D'", "true", truthText(test01));
	shouldBe("Test 1 with 'P'", "false", truthText(test02));
	shouldBe("Test 1 with 'Y'", "true", truthText(test03));
	shouldBe("Test 1 with 'b'", "false", truthText(test04));
	shouldBe("Test 1 with 's'", "true", truthText(test05));
	shouldBe("Test 1 with '%'", "false", truthText(test06));
	shouldBe("Test 1 with '_'", "true", truthText(test07));
	endTest();

	cleanPath(path);
}

void pathTest01() {/*
	CondStakNode *stak = startStak();
	stak = addCond(stak, inrange, 0, 'A', 'Z');
	
	CondNode *cond = extractCond(stak);
	cleanStak(stak);

	StateNode *root = newState(NULL);
	StateNode *branch1 = newState(NULL);
	StateNode *branch2 = newState(NULL);
	PathNode *path = addPath(NULL, cond);
	cleanPath(path);*/
}
