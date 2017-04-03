#define MODULE_REGEX_MOD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regex_mod.h"
#include "testing.h"

char *getGraphRegex(char *str) {
	return DEFAULT_STRING;
}

char **getParseRegex(char *str) {
	return DEFAULT_STRING_ARRAY;
}

void cleanGraphRegex(char *graphRegex) {
	if(graphRegex == DEFAULT_STRING ||
			graphRegex == INVALID_STRING) {
		return;
	}
	free(graphRegex);
}

void testRegex();
#ifdef TESTING_REGEX
int main() {
	testRegex();
	return 0;
}
#endif

//TODO: Finish implementation of getGraphRegex
//TODO: Make tests for getParseRegex
//TODO: Finish implementation of getParseRegex

void testRegex() {
	USE_CASE(GetGraphRegex);
}

TEST_CASE(GetGraphRegex) {
	USE_CASE(GetGraphRegexNull);
	USE_CASE(GetGraphRegexLoneChar);
	USE_CASE(GetGraphRegexLoneCharCategory);
	USE_CASE(GetGraphRegexLoneSimpleCharClass);
	USE_CASE(GetGraphRegexLoneComplexCharClass);
	USE_CASE(GetGraphRegexStringOfChars);
	USE_CASE(GetGraphRegexStringOfMixedItems);
	USE_CASE(GetGraphRegexOr);
	USE_CASE(GetGraphRegexGroup);
	USE_CASE(GetGraphRegexGroupWithOr);
	USE_CASE(GetGraphRegexLineBeginning);
	USE_CASE(GetGraphRegexLineEnding);
	USE_CASE(GetGraphRegexUsingStar);
	USE_CASE(GetGraphRegexUsingPlus);
	USE_CASE(GetGraphRegexUsingQuestion);
	USE_CASE(GetGraphRegexUsingFixedRepetition);
	USE_CASE(GetGraphRegexUsingVariableRepetition);
	USE_CASE(GetGraphRegexUsingInfiniteRepetition);
}

TEST_CASE(GetGraphRegexNull) {
	COMPARE_TEST(char *, Null,
		getGraphRegex(NULL), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, Nil,
		getGraphRegex(""), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
}

char *getCopyStr(char *str) {
	int size = strlen(str) + 1;
	char *result = malloc(size * sizeof(char));
	return memcpy(result, str, size * sizeof(char));
}

TEST_CASE(GetGraphRegexLoneChar) {
	COMPARE_TEST(char *, LoneChar,
		getGraphRegex("a"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedLiteral,
		getGraphRegex("\\\\"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedMeta,
		getGraphRegex("\\."), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexLoneCharCategory) {
	COMPARE_TEST(char *, DigitCategory,
		getGraphRegex("\\d"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, NegatedDigit,
		getGraphRegex("\\D"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, WordCategory,
		getGraphRegex("\\w"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexLoneSimpleCharClass) {
	COMPARE_TEST(char *, LoneCharClass,
		getGraphRegex("[C]"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, NegatedCharClass,
		getGraphRegex("[^q]"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, LoneRangeClass,
		getGraphRegex("[R-b]"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexLoneComplexCharClass) {
	COMPARE_TEST(char *, MultiCharClass,
		getGraphRegex("[eZ]"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MultiPunctClass,
		getGraphRegex("[-#-%!]"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, NegatedMultiCharClass,
		getGraphRegex("[^Wua-h?]"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexStringOfChars) {
	COMPARE_TEST(char *, StringOfChar,
		getGraphRegex("No"), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, StringOfEscapedChar,
		getGraphRegex("\\g\\x\\."), getCopyStr("#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, StringOfCharCategory,
		getGraphRegex("\\d\\d\\D\\w"), getCopyStr("####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, StringOfCharClass,
		getGraphRegex("[this][is][s-o][^nice!][-#-%?]"),
		getCopyStr("#####"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexStringOfMixedItems) {
	COMPARE_TEST(char *, MixedEscapedCharAndCategory,
		getGraphRegex("\\d\\.\\S\\\\"), getCopyStr("####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MixedCharAndEscaped,
		getGraphRegex("aa\\aa\\a"), getCopyStr("#####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MixedWithCharClass,
		getGraphRegex("Gr[ae]y\\s"), getCopyStr("#####"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexOr) {
	COMPARE_TEST(char *, OrSingleChar,
		getGraphRegex("o|u"), getCopyStr("#|#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrMultipleChar,
		getGraphRegex("ha|ting"), getCopyStr("##|####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrMixedChar,
		getGraphRegex("d[ia]g\\W|sh[a-e]v"),
		getCopyStr("####|####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MultipleOr,
		getGraphRegex("Choice A|Choice B|Choice C"),
		getCopyStr("########|########|########"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexGroup) {
	COMPARE_TEST(char *, LoneGroup,
		getGraphRegex("(group)"), getCopyStr("(#####)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupAtBeginning,
		getGraphRegex("(start) over"),
		getCopyStr("(#####)#####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupAtEnd,
		getGraphRegex("in the (end)"),
		getCopyStr("#######(###)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MultipleGroups,
		getGraphRegex("(it doesn't) even (matter)"),
		getCopyStr("(##########)######(######)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupsAdjacent,
		getGraphRegex("(just)(fine)"),
		getCopyStr("(####)(####)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupsNested,
		getGraphRegex("((a)n(t))"),
		getCopyStr("(#)#(#)"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexGroupWithOr) {
	COMPARE_TEST(char *, OrInsideGroup,
		getGraphRegex("(make|destroy) the world"),
		getCopyStr("(####|#######)##########"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrOutsideGroup,
		getGraphRegex("(maybe) now|not"),
		getCopyStr("(#####)####|###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrMixedWithGroup,
		getGraphRegex("(maybe later|not now)|sure"),
		getCopyStr("(###########|#######)|####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrInNestedGroup,
		getGraphRegex("((0|1|2)[0-9]|3[0-1])"),
		getCopyStr("((#|#|#)#|##)"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexLineBeginning) {
	COMPARE_TEST(char *, StringBeginning,
		getGraphRegex("^Begin"), getCopyStr("^#####"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexLineEnding) {
	COMPARE_TEST(char *, StringEnding,
		getGraphRegex("End$"), getCopyStr("###$"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexUsingStar) {
	COMPARE_TEST(char *, CharStar,
		getGraphRegex("A*"), getCopyStr("#*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharStar,
		getGraphRegex("\\d*"), getCopyStr("#*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassStar,
		getGraphRegex("[no]*"), getCopyStr("#*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupStar,
		getGraphRegex("(hey)*"), getCopyStr("(###)*"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexUsingPlus) {
	COMPARE_TEST(char *, CharPlus,
		getGraphRegex("A+"), getCopyStr("##*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharPlus,
		getGraphRegex("\\d+"), getCopyStr("##*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassPlus,
		getGraphRegex("[no]+"), getCopyStr("##*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupPlus,
		getGraphRegex("(hey)+"), getCopyStr("(###)(###)*"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexUsingQuestion) {
	COMPARE_TEST(char *, CharQuestion,
		getGraphRegex("A?"), getCopyStr("#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharQuestion,
		getGraphRegex("\\d?"), getCopyStr("#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassQuestion,
		getGraphRegex("[no]?"), getCopyStr("#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupQuestion,
		getGraphRegex("(hey)?"), getCopyStr("(###)?"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexUsingFixedRepetition) {
	COMPARE_TEST(char *, CharFixedRepetition,
		getGraphRegex("A{3}"), getCopyStr("###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharFixedRepetition,
		getGraphRegex("\\d{3}"), getCopyStr("###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassFixedRepetition,
		getGraphRegex("[no]{3}"), getCopyStr("###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupFixedRepetition,
		getGraphRegex("(hey){3}"),
		getCopyStr("(###)(###)(###)"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexUsingVariableRepetition) {
	COMPARE_TEST(char *, CharVariableRepetition,
		getGraphRegex("A{3,5}"), getCopyStr("####?#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharVariableRepetition,
		getGraphRegex("\\d{3,5}"), getCopyStr("####?#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassVariableRepetition,
		getGraphRegex("[no]{3,5}"), getCopyStr("####?#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupVariableRepetition,
		getGraphRegex("(hey){3,5}"),
		getCopyStr("(###)(###)(###)(###)?(###)?"),
		strcmp, 0,
		cleanGraphRegex);
}

TEST_CASE(GetGraphRegexUsingInfiniteRepetition) {
	COMPARE_TEST(char *, CharInfiniteRepetition,
		getGraphRegex("A{3,}"), getCopyStr("####*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharInfiniteRepetition,
		getGraphRegex("\\d{3,}"), getCopyStr("####*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassInfiniteRepetition,
		getGraphRegex("[no]{3,}"), getCopyStr("####*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupInfiniteRepetition,
		getGraphRegex("(hey){3,}"),
		getCopyStr("(###)(###)(###)(###)*"),
		strcmp, 0,
		cleanGraphRegex);
}
