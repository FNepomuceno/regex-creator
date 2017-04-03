#ifdef MODULE_REGEX_MOD

#include "utils/test.h"
char *INVALID_STRING = "";
char *DEFAULT_STRING = "\1\0";
char **INVALID_STRING_ARRAY = &INVALID_STRING;
char **DEFAULT_STRING_ARRAY = &DEFAULT_STRING;

TEST_CASE(GetGraphRegex);
TEST_CASE(GetGraphRegexNull);
TEST_CASE(GetGraphRegexLoneChar);
TEST_CASE(GetGraphRegexLoneCharCategory);
TEST_CASE(GetGraphRegexLoneSimpleCharClass);
TEST_CASE(GetGraphRegexLoneComplexCharClass);
TEST_CASE(GetGraphRegexStringOfChars);
TEST_CASE(GetGraphRegexStringOfMixedItems);
TEST_CASE(GetGraphRegexOr);
TEST_CASE(GetGraphRegexGroup);
TEST_CASE(GetGraphRegexGroupWithOr);
TEST_CASE(GetGraphRegexLineBeginning);
TEST_CASE(GetGraphRegexLineEnding);
TEST_CASE(GetGraphRegexUsingStar);
TEST_CASE(GetGraphRegexUsingPlus);
TEST_CASE(GetGraphRegexUsingQuestion);
TEST_CASE(GetGraphRegexUsingFixedRepetition);
TEST_CASE(GetGraphRegexUsingVariableRepetition);
TEST_CASE(GetGraphRegexUsingInfiniteRepetition);

#endif
