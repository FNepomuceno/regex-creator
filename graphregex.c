#define MODULE_GRAPHREGEX_MOD

#include <stdlib.h>
#include <string.h>
#include "chartyperegex.h"
#include "graphregex.h"
#include "utils/bool.h"

struct GraphRegexList {
	char *_src_str;
	int _start_index;
	int _end_index;
	char _data_type;
	GraphList *_next_item;
	GraphList *_prev_item;
};
static GraphList nil_graphlistdata;
static GraphList *nil_graphlist = &nil_graphlistdata;

void cleanGraphRegex(char *graphRegex) {
	if(graphRegex == DEFAULT_STRING ||
			graphRegex == INVALID_STRING) {
		return;
	}
	free(graphRegex);
}

static void cleanGraphList(GraphList *list) {
	if(list == NULL || list == nil_graphlist) {
		return;
	}
	list->_prev_item->_next_item = nil_graphlist;
	GraphList *cur_list = list;
	while(cur_list != nil_graphlist) {
		GraphList *nxt_list = cur_list->_next_item;
		free(cur_list);
		cur_list = nxt_list;
	}
}

char *getGraphRegex(char *str) {
	if(str == NULL || strcmp(str,"") == 0) {
		return INVALID_STRING;
	}
	char *cur_str = str;
	int cur_length = strlen(str);
	GraphList *list = newGraphList(str);
	while(cur_length > 0) {
		list = nextGraphListNode(list, cur_str, cur_length);
		int new_node_length =
			lengthOfGraphListNode(list->_prev_item);
		cur_str = cur_str + new_node_length;
		cur_length -= new_node_length;
	}
	cleanGraphList(list);
	return DEFAULT_STRING;
}

static GraphList *newGraphList(char *str) {
	GraphList *result = malloc(sizeof (GraphList));
	result->_src_str = str;
	result->_start_index = -1;
	result->_end_index = 0;
	result->_data_type = INVALID_CHARTYPE;
	result->_next_item = result;
	result->_prev_item = result;
	return result;
}

static int lengthOfGraphListNode(GraphList *list) {
	return list->_end_index - list->_start_index;
}

static GraphList *nextGraphListNode(GraphList *list,
		char *str, int length) {
	int new_node_length = nextGraphListLength(str, length);
	char new_node_type = nextGraphListType(str, length);
	return addGraphListNode(list, new_node_length,
		new_node_type);
}

static int nextGraphListLength(char *str, int length) {
	return 1;
}

static char nextGraphListType(char *str, int length) {
	return INVALID_CHARTYPE;
}

static GraphList *addGraphListNode(GraphList *list,
		int new_length, char data_type) {
	GraphList *new_list =
		(list->_start_index < 0)?
			list:
			newGraphList(list->_src_str);
	new_list = setGraphListNodeData(new_list,
		new_length, data_type);
	return linkGraphListNodes(list, new_list);
}

static GraphList *setGraphListNodeData(GraphList *list,
		int length, char data_type) {
	list->_data_type = data_type;
	list->_start_index = list->_prev_item->_end_index;
	list->_end_index = list->_prev_item->_end_index + length;
	return list;
}

static GraphList *linkGraphListNodes(GraphList *src,
		GraphList *inp) {
	inp->_next_item = src;
	inp->_prev_item = src->_prev_item;
	src->_prev_item->_next_item = inp;
	src->_prev_item = inp;
	return src;
}

//TODO: Finish implementation of getGraphRegex
TEST_CASE(GraphRegex);
#ifdef TESTING_GRAPHREGEX
int main(void) {
	USE_CASE(GraphRegex);
	return 0;
}
#endif

TEST_CASE(GraphRegex) {
	USE_CASE(GetGraphRegex);
}

static TEST_CASE(GetGraphRegex) {
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

static TEST_CASE(GetGraphRegexNull) {
	COMPARE_TEST(char *, Null,
		getGraphRegex(NULL), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, Nil,
		getGraphRegex(""), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_RES(char *, CopyStr, char *str) {
	int size = strlen(str) + 1;
	char *result = malloc(size * sizeof(char));
	return memcpy(result, str, size * sizeof(char));
}

static TEST_CASE(GetGraphRegexLoneChar) {
	COMPARE_TEST(char *, LoneChar,
		getGraphRegex("a"), USE_RES(CopyStr, "#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedLiteral,
		getGraphRegex("\\\\"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedMeta,
		getGraphRegex("\\."), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexLoneCharCategory) {
	COMPARE_TEST(char *, DigitCategory,
		getGraphRegex("\\d"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, NegatedDigit,
		getGraphRegex("\\D"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, WordCategory,
		getGraphRegex("\\w"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexLoneSimpleCharClass) {
	COMPARE_TEST(char *, LoneCharClass,
		getGraphRegex("[C]"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, NegatedCharClass,
		getGraphRegex("[^q]"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, LoneRangeClass,
		getGraphRegex("[R-b]"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexLoneComplexCharClass) {
	COMPARE_TEST(char *, MultiCharClass,
		getGraphRegex("[eZ]"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MultiPunctClass,
		getGraphRegex("[-#-%!]"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, NegatedMultiCharClass,
		getGraphRegex("[^Wua-h?]"), USE_RES(CopyStr,"#"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexStringOfChars) {
	COMPARE_TEST(char *, StringOfChar,
		getGraphRegex("No"), USE_RES(CopyStr,"##"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, StringOfEscapedChar,
		getGraphRegex("\\g\\x\\."), USE_RES(CopyStr,"###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, StringOfCharCategory,
		getGraphRegex("\\d\\d\\D\\w"), USE_RES(CopyStr,"####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, StringOfCharClass,
		getGraphRegex("[this][is][s-o][^nice!][-#-%?]"),
		USE_RES(CopyStr,"#####"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexStringOfMixedItems) {
	COMPARE_TEST(char *, MixedEscapedCharAndCategory,
		getGraphRegex("\\d\\.\\S\\\\"), USE_RES(CopyStr,"####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MixedCharAndEscaped,
		getGraphRegex("aa\\aa\\a"), USE_RES(CopyStr,"#####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MixedWithCharClass,
		getGraphRegex("Gr[ae]y\\s"), USE_RES(CopyStr,"#####"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexOr) {
	COMPARE_TEST(char *, OrSingleChar,
		getGraphRegex("o|u"), USE_RES(CopyStr,"#|#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrMultipleChar,
		getGraphRegex("ha|ting"), USE_RES(CopyStr,"##|####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrMixedChar,
		getGraphRegex("d[ia]g\\W|sh[a-e]v"),
		USE_RES(CopyStr,"####|####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MultipleOr,
		getGraphRegex("Choice A|Choice B|Choice C"),
		USE_RES(CopyStr,"########|########|########"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexGroup) {
	COMPARE_TEST(char *, LoneGroup,
		getGraphRegex("(group)"), USE_RES(CopyStr,"(#####)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupAtBeginning,
		getGraphRegex("(start) over"),
		USE_RES(CopyStr,"(#####)#####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupAtEnd,
		getGraphRegex("in the (end)"),
		USE_RES(CopyStr,"#######(###)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, MultipleGroups,
		getGraphRegex("(it doesn't) even (matter)"),
		USE_RES(CopyStr,"(##########)######(######)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupsAdjacent,
		getGraphRegex("(just)(fine)"),
		USE_RES(CopyStr,"(####)(####)"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupsNested,
		getGraphRegex("((a)n(t))"),
		USE_RES(CopyStr,"(#)#(#)"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexGroupWithOr) {
	COMPARE_TEST(char *, OrInsideGroup,
		getGraphRegex("(make|destroy) the world"),
		USE_RES(CopyStr,"(####|#######)##########"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrOutsideGroup,
		getGraphRegex("(maybe) now|not"),
		USE_RES(CopyStr,"(#####)####|###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrMixedWithGroup,
		getGraphRegex("(maybe later|not now)|sure"),
		USE_RES(CopyStr,"(###########|#######)|####"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, OrInNestedGroup,
		getGraphRegex("((0|1|2)[0-9]|3[0-1])"),
		USE_RES(CopyStr,"((#|#|#)#|##)"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexLineBeginning) {
	COMPARE_TEST(char *, StringBeginning,
		getGraphRegex("^Begin"), USE_RES(CopyStr,"^#####"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexLineEnding) {
	COMPARE_TEST(char *, StringEnding,
		getGraphRegex("End$"), USE_RES(CopyStr,"###$"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexUsingStar) {
	COMPARE_TEST(char *, CharStar,
		getGraphRegex("A*"), USE_RES(CopyStr,"#*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharStar,
		getGraphRegex("\\d*"), USE_RES(CopyStr,"#*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassStar,
		getGraphRegex("[no]*"), USE_RES(CopyStr,"#*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupStar,
		getGraphRegex("(hey)*"), USE_RES(CopyStr,"(###)*"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexUsingPlus) {
	COMPARE_TEST(char *, CharPlus,
		getGraphRegex("A+"), USE_RES(CopyStr,"##*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharPlus,
		getGraphRegex("\\d+"), USE_RES(CopyStr,"##*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassPlus,
		getGraphRegex("[no]+"), USE_RES(CopyStr,"##*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupPlus,
		getGraphRegex("(hey)+"), USE_RES(CopyStr,"(###)(###)*"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexUsingQuestion) {
	COMPARE_TEST(char *, CharQuestion,
		getGraphRegex("A?"), USE_RES(CopyStr,"#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharQuestion,
		getGraphRegex("\\d?"), USE_RES(CopyStr,"#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassQuestion,
		getGraphRegex("[no]?"), USE_RES(CopyStr,"#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupQuestion,
		getGraphRegex("(hey)?"), USE_RES(CopyStr,"(###)?"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexUsingFixedRepetition) {
	COMPARE_TEST(char *, CharFixedRepetition,
		getGraphRegex("A{3}"), USE_RES(CopyStr,"###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharFixedRepetition,
		getGraphRegex("\\d{3}"), USE_RES(CopyStr,"###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassFixedRepetition,
		getGraphRegex("[no]{3}"), USE_RES(CopyStr,"###"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupFixedRepetition,
		getGraphRegex("(hey){3}"),
		USE_RES(CopyStr,"(###)(###)(###)"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexUsingVariableRepetition) {
	COMPARE_TEST(char *, CharVariableRepetition,
		getGraphRegex("A{3,5}"), USE_RES(CopyStr,"####?#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharVariableRepetition,
		getGraphRegex("\\d{3,5}"), USE_RES(CopyStr,"####?#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassVariableRepetition,
		getGraphRegex("[no]{3,5}"), USE_RES(CopyStr,"####?#?"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupVariableRepetition,
		getGraphRegex("(hey){3,5}"),
		USE_RES(CopyStr,"(###)(###)(###)(###)?(###)?"),
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexUsingInfiniteRepetition) {
	COMPARE_TEST(char *, CharInfiniteRepetition,
		getGraphRegex("A{3,}"), USE_RES(CopyStr,"####*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EscapedCharInfiniteRepetition,
		getGraphRegex("\\d{3,}"), USE_RES(CopyStr,"####*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, CharClassInfiniteRepetition,
		getGraphRegex("[no]{3,}"), USE_RES(CopyStr,"####*"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, GroupInfiniteRepetition,
		getGraphRegex("(hey){3,}"),
		USE_RES(CopyStr,"(###)(###)(###)(###)*"),
		strcmp, 0,
		cleanGraphRegex);
}
