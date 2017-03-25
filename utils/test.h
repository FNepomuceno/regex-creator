#ifndef UTILS_TEST_H
#define UTILS_TEST_H

#include <stdio.h>
#define TEST(x) printf("%s:%d TEST `%s` %s\n", __FILE__,\
	__LINE__, #x, (x)? "PASSED": "FAILED");

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
