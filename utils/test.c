#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

void doNothing(void) {
	;
}

void printTest(char *file, int line, char *expr_str, int expr) {
	fprintf(stdout, "%s:%d TEST `%s` %s\n", file,
		line, expr_str, (expr)? "PASSED": "FAILED");
}

void printError(char *file, int line, char *err_str) {
	fprintf(stderr, "%s.%d: %s\n", file, line, err_str);
}

TEST_RES(char *, CopyStr, char *str) {
	int size = strlen(str) + 1;
	char *result = malloc(size * sizeof(char));
	return memcpy(result, str, size * sizeof(char));
}
