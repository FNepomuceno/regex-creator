#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"
#include "stak.h"
#include "state.h"
//#define __DEBUG__

int shouldBe(const char *text, const char *expected,
	const char *actual) {
	int diff = strcmp(expected, actual);
	if(diff == 0) {
		printf("%s: Test Passed\n", text);
		return 1;
	} else {
		printf("%s: Test Failed\n", text);
		printf("Output should be \"%s\", but is currently "
			"\"%s\"\n", expected, actual);
		return 0;
	}
}

void endTest(int passed, int total) {
	printf("End Test: %d of %d tests passed\n\n", passed, total);
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
	endTest(0, 0);
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
	int passed = 0, total = 3;
	
	passed += shouldBe("Test with 'B'", "true", truthText(test01));
	passed += shouldBe("Test with 'x'", "true", truthText(test02));
	passed += shouldBe("Test with '_'", "false",
		truthText(test03));
	endTest(passed, total);

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
	int passed = 0, total = 7;

	passed += shouldBe("Test with 'D'", "true",
		truthText(test01));
	passed += shouldBe("Test with 'P'", "false",
		truthText(test02));
	passed += shouldBe("Test with 'Y'", "true", truthText(test03));
	passed += shouldBe("Test with 'b'", "false",
		truthText(test04));
	passed += shouldBe("Test with 's'", "true", truthText(test05));
	passed += shouldBe("Test with '%'", "false",
		truthText(test06));
	passed += shouldBe("Test with '_'", "true", truthText(test07));
	endTest(passed, total);

	cleanPath(path);
}

int testNum(char *text, int expected, int actual) {
	char str1[30], str2[30];
	sprintf(str1, "%d", expected);
	sprintf(str2, "%d", actual);
	return shouldBe(text, str1, str2);
}

PathNode *testPath(char *text, char input, PathNode *path,
	StateNode *expected, int *correct) {
	PathNode *cur = nextAppropriatePath(input, path);
	StateNode *actual = getPathDest(cur);
	*correct += testNum(text, stateId(expected), stateId(actual));
	return nextPath(cur);
}

void stateTest01() {
	printf("State Test 1: Graph of states\n");
	StateNode *node0 = newState(NULL);
	StateNode *node1 = newState(NULL);
	StateNode *node2 = newState(NULL);
	StateNode *node3 = newState(NULL);
	addStatePath(node0, node2, NULL);
	addStatePath(node0, node1, NULL);
	addStatePath(node1, node3, NULL);
	addStatePath(node2, node3, NULL);

	PathNode *path = getPaths(node0);
	int passed = 0, total = 8;
	path = testPath("Root's Branch 1", 'a', path, node1, &passed);
	path = testPath("Root's Branch 2", 'a', path, node2, &passed);
	path = testPath("Root Has No More Branches", 'a',
		path, NULL, &passed);
	path = getPaths(node1);
	path = testPath("Branch 1's Branch", 'a', path,
		node3, &passed);
	path = testPath("Branch 1 Has No More Branches",
		'a', path, NULL, &passed);
	path = getPaths(node2);
	path = testPath("Branch 2's Branch", 'a', path,
		node3, &passed);
	path = testPath("Branch 2 Has No More Branches",
		'a', path, NULL, &passed);
	path = getPaths(node3);
	path = testPath("Branch 3 Has No Branches", 'a',
		path, NULL, &passed);
	endTest(passed, total);

	node0 = removeState(node0);
	node1 = removeState(node1);
	node2 = removeState(node2);
	node3 = removeState(node3);
}

static CondNode *sT02cond01() {
	CondStakNode *stak = startStak();
	stak = addCond(stak, inrange, 0, 'a', 'z');
	CondNode *result = extractCond(stak);
	cleanStak(stak);
	return result;
}

static CondNode *sT02cond02() {
	CondStakNode *stak = startStak();
	stak = addCond(stak, inrange, 0, 'A', 'Z');
	CondNode *result = extractCond(stak);
	cleanStak(stak);
	return result;
}

static void sT02state01(StateNode **res0, StateNode **res1,
	StateNode **res2) {
	*res0 = newState(NULL);
	*res1 = newState(NULL);
	*res2 = newState(NULL);
	addStatePath(*res0, *res1, sT02cond01());
	addStatePath(*res0, *res2, sT02cond02());
}

void stateTest02() {
	printf("State Test 2: Distinguishing case\n");
	StateNode *node0, *node1, *node2;
	sT02state01(&node0, &node1, &node2);

	PathNode *path = getPaths(node0);
	int passed = 0, total = 4;
	testPath("Uppercase", 'A', path, node2, &passed);
	testPath("Lowercase", 'a', path, node1, &passed);
	testPath("Digit", '8', path, NULL, &passed);
	testPath("Special Character", '*', path, NULL, &passed);
	endTest(passed, total);

	node0 = removeState(node0);
	node1 = removeState(node1);
	node2 = removeState(node2);
}
