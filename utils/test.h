#ifndef UTILS_TEST_H
#define UTILS_TEST_H

void doNothing(void);
void printTest(char *, int, char *, int);
void printError(char *file, int line, char *err_str);

#define USE_CASE(name) { test##name (); }
#define TEST_RES(out_type, name, input) out_type \
	get##name (input)
#define TEST_CASE(name) void test##name (void)
#define USE_RES(name, arg) get##name (arg)

TEST_RES(char *, CopyStr, char *str);

#define TEST(expr) printTest(__FILE__, __LINE__, #expr, (expr));

#define TEST_TRUE(x) TEST((x) == TRUE_BOOL)
#define TEST_FALSE(x) TEST((x) == FALSE_BOOL)

#define COMPARE_TEST(type, id, getTest, getExpected, \
		compare, value, cleanFunc) { \
	type test##id = getTest; \
	type expected##id = getExpected; \
	TEST(compare(test##id, expected##id) == value); \
	cleanFunc(test##id); \
	cleanFunc(expected##id); \
}

#ifdef TEST_ASSERT
#define ASSURE(condition, fail_code) { \
	if(!(condition)) { \
		LOG_ERROR("CONDITION `" #condition "` FAILED"); \
		abort(); \
	} \
}
#else
#define ASSURE(condition, fail_code) { \
	if(!(condition)) { \
		fail_code \
	} \
}
#endif

#define LOG_ERROR(err_str) { \
	printError(__FILE__, __LINE__, (err_str)); \
}

#endif
