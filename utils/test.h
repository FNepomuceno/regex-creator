#ifndef UTILS_TEST_H
#define UTILS_TEST_H

#include <stdio.h>

#define TEST_CASE(x) static void test##x ()
#define USE_CASE(x) { test##x (); }

#define TEST(x) printf("%s:%d TEST `%s` %s\n", __FILE__,\
	__LINE__, #x, (x)? "PASSED": "FAILED");

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
	fprintf(stderr, "%s.%d: %s\n", __FILE__, __LINE__, \
		(err_str)); \
}

#endif
