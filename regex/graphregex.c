#define MODULE_GRAPHREGEX_MOD

#include <stdlib.h>
#include <string.h>
#include "../utils/chartype.h"
#include "graphregex.h"
#include "../utils/bool.h"

struct GraphRegexList {
	char *_src_str;
	int _start_index;
	int _end_index;
	char _data_type;
	int _node_count;
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
	if(list->_prev_item != nil_graphlist) {
		list->_prev_item->_next_item = nil_graphlist;
	}
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
		if(new_node_length < 0) {
			cleanGraphList(list);
			return INVALID_STRING;
		}
		cur_str = cur_str + new_node_length;
		cur_length -= new_node_length;
	}
	char *result = extractStrFromGraphList(list);
	cleanGraphList(list);
	return result;
}

static GraphList *newGraphList(char *str) {
	GraphList *result = malloc(sizeof (GraphList));
	result->_src_str = str;
	result->_start_index = -1;
	result->_end_index = 0;
	result->_data_type = INVALID_CHARTYPE;
	result->_node_count = 0;
	result->_next_item = result;
	result->_prev_item = result;
	return result;
}

static int lengthOfGraphListNode(GraphList *list) {
	return list->_end_index - list->_start_index;
}

static char *extractStrFromGraphList(GraphList *list) {
	if(list->_prev_item != nil_graphlist) {
		list->_prev_item->_next_item = nil_graphlist;
	}
	int str_length = list->_node_count;
	char *result = malloc((str_length+1) * sizeof(char));
	*(result+str_length) = '\0';

	GraphList *cur_list = list;
	int index = 0;
	while(cur_list != nil_graphlist) {
		*(result+index) = cur_list->_data_type;

		cur_list = cur_list->_next_item;
		if(index >= str_length)abort();
		index++;
	}
	return result;
}

static GraphList *nextGraphListNode(GraphList *list,
		char *str, int length) {
	int new_node_length = nextGraphListLength(str, length);
	char new_node_type = nextGraphListType(str, length);
	if(new_node_type ==  PLUS_CHARTYPE) {
		return addPlusNode(list);
	} else if(new_node_type ==  FIXED_REPETITION_CHARTYPE) {
		return addFixedRepetitionNode(list, str,
			new_node_length);
	} else if(new_node_type ==  VARIABLE_REPETITION_CHARTYPE) {
		return addVariableRepetitionNode(list, str,
			new_node_length);
	} else if(new_node_type ==  INFINITE_REPETITION_CHARTYPE) {
		return addInfiniteRepetitionNode(list, str,
			new_node_length);
	}
	return addGraphListNode(list, new_node_length,
		new_node_type);
}

static int nextGraphListLength(char *str, int length) {
	if(hasGraphLoneChar(str, length) == TRUE_BOOL ||
			hasWildcardChar(str, length) == TRUE_BOOL) {
		return 1;
	} else if(hasCharCategory(str, length) == TRUE_BOOL ||
			hasNegatedCharCategory(str, length) == TRUE_BOOL ||
			hasNormalEscapedChar(str, length) == TRUE_BOOL) {
		return 2;
	} else if(charClassLength(str, length) > 0) {
		return charClassLength(str, length);
	} else if(hasOrChar(str, length) == TRUE_BOOL ||
			hasGroupBeginning(str, length) == TRUE_BOOL ||
			hasGroupEnding(str, length) == TRUE_BOOL ||
			hasTextBeginning(str, length) == TRUE_BOOL ||
			hasTextEnding(str, length) == TRUE_BOOL ||
			hasKleeneStar(str, length) == TRUE_BOOL ||
			hasPlusChar(str, length) == TRUE_BOOL ||
			hasQuestionMark(str, length) == TRUE_BOOL) {
		return 1;
	} else if(hasFixedRepetition(str, length) == TRUE_BOOL ||
			hasVariableRepetition(str, length) == TRUE_BOOL ||
			hasInfiniteRepetition(str, length) == TRUE_BOOL) {
		return enclosedBraceLength(str, length);
	}
	return -1;
}

static char nextGraphListType(char *str, int length) {
	if(hasGraphLoneChar(str, length) == TRUE_BOOL ||
			hasWildcardChar(str, length) == TRUE_BOOL) {
		return DATA_CHARTYPE;
	} else if(hasCharCategory(str, length) == TRUE_BOOL ||
			hasNegatedCharCategory(str, length) == TRUE_BOOL ||
			hasNormalEscapedChar(str, length) == TRUE_BOOL) {
		return DATA_CHARTYPE;
	} else if(charClassLength(str, length) > 0) {
		return DATA_CHARTYPE;
	} else if(hasOrChar(str, length) == TRUE_BOOL) {
		return OR_CHARTYPE;
	} else if(hasGroupBeginning(str, length) == TRUE_BOOL) {
		return BEGIN_GROUP_CHARTYPE;
	} else if(hasGroupEnding(str, length) == TRUE_BOOL) {
		return END_GROUP_CHARTYPE;
	} else if(hasTextBeginning(str, length) == TRUE_BOOL) {
		return BEGIN_TEXT_CHARTYPE;
	} else if(hasTextEnding(str, length) == TRUE_BOOL) {
		return END_TEXT_CHARTYPE;
	} else if(hasKleeneStar(str, length) == TRUE_BOOL) {
		return KLEENE_STAR_CHARTYPE;
	} else if(hasPlusChar(str, length) == TRUE_BOOL) {
		return PLUS_CHARTYPE;
	} else if(hasQuestionMark(str, length) == TRUE_BOOL) {
		return OPTIONAL_MARK_CHARTYPE;
	} else if(hasFixedRepetition(str, length) == TRUE_BOOL) {
		return FIXED_REPETITION_CHARTYPE;
	} else if(hasVariableRepetition(str, length) == TRUE_BOOL) {
		return VARIABLE_REPETITION_CHARTYPE;
	} else if(hasInfiniteRepetition(str, length) == TRUE_BOOL) {
		return INFINITE_REPETITION_CHARTYPE;
	}
	return INVALID_CHARTYPE;
}

static GraphList *addGraphListNode(GraphList *list,
		int new_length, char data_type) {
	GraphList *new_list;
	if(list->_start_index < 0) {
		list->_start_index = 0;
		new_list = list;
	} else {
		new_list = newGraphList(list->_src_str);
	}
	GraphList *result = linkGraphListNodes(list, new_list);
	new_list = setGraphListNodeData(new_list,
		new_length, data_type);
	result->_node_count++;
	return result;
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

static GraphList *addPlusNode(GraphList *list) {
	GraphList *result = addRepeatedNode(list);
	return addGraphListNode(result, 1, KLEENE_STAR_CHARTYPE);
}

static GraphList *addRepeatedNode(GraphList *list) {
	int has_group = FALSE_BOOL;
	int group_stack = 0;
	int num_items = 1;
	GraphList *group_beginning = list->_prev_item;
	while(group_beginning->_start_index > 0) {
		if(has_group == FALSE_BOOL &&
				group_beginning->_data_type == DATA_CHARTYPE) {
			break;
		} else if(group_beginning->_data_type ==
				END_GROUP_CHARTYPE) {
			group_stack++;
			has_group = TRUE_BOOL;
		} else if(group_beginning->_data_type ==
				BEGIN_GROUP_CHARTYPE) {
			group_stack--;
		}
		if(has_group == TRUE_BOOL && group_stack == 0) {
			break;
		}
		num_items++;
		group_beginning = group_beginning->_prev_item;
	}

	GraphList *cur_node = group_beginning;
	GraphList *result = list;
	for(int i = 0; i < num_items; i++) {
		list = addGraphListNode(list, 0,
			cur_node->_data_type);
		cur_node = cur_node->_next_item;
	}
	return result;
}

static GraphList *addFixedRepetitionNode(GraphList *list,
		char *str, int length) {
	int num_reps = -1;
	setFixedRepetitions(str, &num_reps);

	GraphList *result = list;
	for(int i = 2; i <= num_reps; i++) {
		list = addRepeatedNode(list);
	}
	list->_prev_item->_end_index += length;
	return result;
}

static GraphList *addVariableRepetitionNode(GraphList *list,
		char *str, int length) {
	int min_reps = -1, max_reps = -1;
	setVariableRepetitions(str, &min_reps, &max_reps);

	GraphList *result = list;
	for(int i = 2; i <= max_reps; i++) {
		list = addRepeatedNode(list);
		if(i == min_reps+1) {
			list = addGraphListNode(list, length,
				OPTIONAL_MARK_CHARTYPE);
		}
	}
	list->_prev_item->_end_index += length;
	return result;
}

static GraphList *addInfiniteRepetitionNode(GraphList *list,
		char *str, int length) {
	int min_reps = -1;
	setInfiniteRepetitions(str, &min_reps);

	GraphList *result = list;
	for(int i = 2; i <= min_reps+1; i++) {
		list = addRepeatedNode(list);
	}
	list = addGraphListNode(list, length, KLEENE_STAR_CHARTYPE);
	return result;
}

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
	COMPARE_TEST(char *, UnfinishedCharClass,
		getGraphRegex("["), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EmptyCharClass,
		getGraphRegex("[]"), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, EmptyNegatedCharClass,
		getGraphRegex("[^]"), INVALID_STRING,
		strcmp, 0,
		cleanGraphRegex);
}

static TEST_CASE(GetGraphRegexLoneChar) {
	COMPARE_TEST(char *, LoneChar,
		getGraphRegex("a"), USE_RES(CopyStr, "#"),
		strcmp, 0,
		cleanGraphRegex);
	COMPARE_TEST(char *, WildcardChar,
		getGraphRegex("."), USE_RES(CopyStr, "#"),
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
		USE_RES(CopyStr,"((#)#(#))"),
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
