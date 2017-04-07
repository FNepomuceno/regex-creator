#define CONDITION_PACKAGE

#include <stdlib.h>
#include "node.h"
#include "stringtype.h"
#include "tags.h"
#include "function/info.h"
#include "../testing.h"
#include "../utils/bool.h"
#include "../utils/test.h"

typedef struct CondList CondList;
struct CondList {
	CondNode *_data_node;
	CondList *_next_item;
	CondList *_prev_item;
};
CondList nil_list_data;
CondList *nil_list = &nil_list_data;

static CondList *initializeList() {
	CondList *new_list = malloc(sizeof (CondList));
	new_list->_data_node = nil_node;
	new_list->_next_item = new_list;
	new_list->_prev_item = new_list;
	return new_list;
}

CondList *advanceList(CondList *list);
static CondList *finishList(CondList *list);
void cleanCondList(CondList *list) {
	if(list == NULL || list == nil_list) {
		return;
	}
	CondList *cur_list = finishList(list);
	if(cur_list->_prev_item != nil_list) {
		abort();
	}
	while(cur_list != nil_list) {
		cur_list = advanceList(cur_list);
	}
}

CondList *advanceList(CondList *list) {
	CondList *cur_list = list;
	CondList *nxt_list = list->_next_item;
	cleanCondNode(cur_list->_data_node);
	free(cur_list);
	return nxt_list;
}

CondNode *extractNode(CondList *list) {
	CondNode *result = list->_data_node;
	list->_data_node = nil_node;
	return result;
}

static CondList *addNewListItem(CondList *list, CondNode *node);
static CondList *addNodeToList(CondList *list, CondNode *node) {
	ASSURE(list != NULL && list != nil_list &&
			node != NULL && node != nil_node &&
			list->_prev_item != nil_list,
		LOG_ERROR("INVALID INPUT. DISCARDING NODE");
		cleanCondNode(node);
		return list;
	);
	if(list->_data_node == nil_node) {
		list->_data_node = node;
		return list;
	}
	return addNewListItem(list, node);
}

static void insertListItem(CondList *dst, CondList *src) {
	dst->_prev_item->_next_item = src;
	dst->_prev_item = src;
}

static CondList *addNewListItem(CondList *list, CondNode *node) {
	CondList *new_list = malloc(sizeof (CondList));
	new_list->_data_node = node;
	new_list->_next_item = list;
	new_list->_prev_item = list->_prev_item;
	insertListItem(list, new_list);
	return list;
}

static CondList *finishList(CondList *list) {
	ASSURE(list != NULL && list!= nil_list,
		LOG_ERROR("INVALID INPUT");
		return list;
	);
	if(list->_prev_item == nil_list) {
		return list;
	}
	insertListItem(list, nil_list);
	return list;
}

static int listsAreEquivalent(CondList *list1, CondList *list2) {
	CondList *cur_list1 = list1;
	CondList *cur_list2 = list2;
	int safety_counter = 0;
	do {
		if(cur_list1 == NULL || cur_list2 == NULL) {
			return FALSE_BOOL;
		}
		if(cur_list1 == cur_list2) {
			return TRUE_BOOL;
		}
		if(cur_list1 == nil_list ||
				cur_list2 == nil_list) {
			return FALSE_BOOL;
		}
		if(isEquivalent(cur_list1->_data_node,
				cur_list2->_data_node) == FALSE_BOOL) {
			return FALSE_BOOL;
		}
		if(safety_counter > 50) {
			LOG_ERROR("CRITICAL ERROR");
			abort();
		}
		cur_list1 = cur_list1->_next_item;
		cur_list2 = cur_list2->_next_item;
		safety_counter++;
	} while(cur_list1 != nil_list && cur_list2 != nil_list);
	return TRUE_BOOL;
}

static CondList *newLoneCharList(char *str,
	const NegationTag *tag);
static CondList *newCharCategoryList(char *str);
static CondList *newNegatedCharCategoryList(char *str);
static CondList *newEscapedLiteralCharList(char *str);
static CondList *newLinkedDataList(char *str);
CondList *getCondList(char *str) {
	if(isEmptyCharString(str) == TRUE_BOOL) {
		return nil_list;
	}
	if(isWildcardCharString(str) == TRUE_BOOL) {
		return newLoneCharList("\n", YES_NEGATION);
	}
	if(isLoneCharString(str) == TRUE_BOOL) {
		return newLoneCharList(str, NO_NEGATION);
	}
	if(isCharCategory(str) == TRUE_BOOL) {
		return newCharCategoryList(str);
	}
	if(isNegatedCharCategory(str) == TRUE_BOOL) {
		return newNegatedCharCategoryList(str);
	}
	if(isEscapedCharString(str) == TRUE_BOOL) {
		return newEscapedLiteralCharList(str);
	}
	if(isClosedBracketExpr(str) == FALSE_BOOL) {
		return nil_list;
	}
	return newLinkedDataList(str);
}

static CondList *newLoneCharList(char *str,
		const NegationTag *tag) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, *str, '\0', tag));
	return finishList(result);
}

static CondList *addEscapedCharNodeToList(CondList *list,
	char ch, const NegationTag *tag);
static CondList *newCharCategoryList(char *str) {
	CondList *result = initializeList();
	result = addEscapedCharNodeToList(result, *(str+1),
		NO_NEGATION);
	return finishList(result);
}

static CondList *newNegatedCharCategoryList(char *str) {
	CondList *result = initializeList();
	result = addEscapedCharNodeToList(result, *(str+1),
		NO_NEGATION);
	return finishList(result);
}

static CondList *newEscapedLiteralCharList(char *str) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, *(str+1), '\0', NO_NEGATION));
	return finishList(result);
}

static CondList *addEscapedCharNodeToList(CondList *list,
		char ch, const NegationTag *tag) {
	CondNode *new_node;
	switch(ch) {
		case '0':
			new_node = newCondNode(equalsChar, '\0', '\0',
				tag);
			break;
		case 'D':
			new_node = newCondNode(isDigitChar, '\0', '\0',
				negateNegationTag(tag));
			break;
		case 'S':
			new_node = newCondNode(isWhitespace, '\0', '\0',
				negateNegationTag(tag));
			break;
		case 'W':
			new_node = newCondNode(isWordChar, '\0', '\0',
				negateNegationTag(tag));
			break;
		case 'd':
			new_node = newCondNode(isDigitChar, '\0', '\0',
				tag);
			break;
		case 's':
			new_node = newCondNode(isWhitespace, '\0', '\0',
				tag);
			break;
		case 'w':
			new_node = newCondNode(isWordChar, '\0', '\0',
				tag);
			break;
		default:
			new_node = newCondNode(equalsChar, ch, '\0', tag);
	}
	return addNodeToList(list, new_node);
}

static CondList *newLinkedDataList(char *str) {
	int num_chars = getAmtCharsInClass(str);
	if(num_chars <= 0) return nil_list;

	CondList *result = initializeList();
	const NegationTag *tag = getCharClassNegationTag(str);
	int cur_num_chars = num_chars;
	char *cur_str = getCharClassStartPosition(str);
	while(cur_num_chars > 0) {
		int data_size = nextDataSize(cur_str, cur_num_chars);
		if(data_size == 1) {
			result = addNodeToList(result,
				newCondNode(equalsChar, *(cur_str), '\0', tag));
		} else if(data_size == 2) {
			result = addEscapedCharNodeToList(result,
				*(cur_str+1), tag);
		} else if(data_size == 3) {
			result = addNodeToList(result,
				newCondNode(inRangeChar, *(cur_str), *(cur_str+2),
				tag));
		} else {
			cleanCondList(result);
			return nil_list;
		}
		cur_num_chars -= data_size;
		cur_str = cur_str + data_size;
	}
	return finishList(result);
}

void testGetCondList();
#ifdef TEST_CONDITION_CONDLIST
int main() {
	testGetCondList();
	return 0;
}
#endif

static void testGetCondListNull();
static void testGetCondListSingleChar();
static void testGetCondListCharCategory();
static void testGetCondListSimpleCharClass();
static void testGetCondListComplexCharClass();
void testGetCondList() {
	testGetCondListNull();
	testGetCondListSingleChar();
	testGetCondListCharCategory();
	testGetCondListSimpleCharClass();
	testGetCondListComplexCharClass();
}

static void testGetCondListNull() {
	CondList *test1 = getCondList(NULL);
	CondList *expected1 = nil_list;
	TEST(listsAreEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondList(test1);
	cleanCondList(expected1);

	CondList *test2 = getCondList("");
	CondList *expected2 = nil_list;
	TEST(listsAreEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondList(test2);
	cleanCondList(expected2);
}

static void testGetCondListSingleChar() {
	CondList *test1 = getCondList("a");
	CondList *expected1 = initializeList();
	expected1 = addNodeToList(expected1,
		newCondNode(equalsChar, 'a', '\0', NO_NEGATION));
	expected1 = finishList(expected1);
	TEST(listsAreEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondList(test1);
	cleanCondList(expected1);

	CondList *test2 = getCondList("\\\\");
	CondList *expected2 = initializeList();
	expected2 = addNodeToList(expected2,
		newCondNode(equalsChar, '\\', '\0', NO_NEGATION));
	expected2 = finishList(expected2);
	TEST(listsAreEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondList(test2);
	cleanCondList(expected2);

	CondList *test3 = getCondList("\\.");
	CondList *expected3 = initializeList();
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, '.', '\0', NO_NEGATION));
	expected3 = finishList(expected3);
	TEST(listsAreEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondList(test3);
	cleanCondList(expected3);

	CondList *test4 = getCondList(".");
	CondList *expected4 = initializeList();
	expected4 = addNodeToList(expected4,
		newCondNode(equalsChar, '\n', '\0', YES_NEGATION));
	expected4 = finishList(expected4);
	TEST(listsAreEquivalent(test4, expected4) == TRUE_BOOL);
	cleanCondList(test4);
	cleanCondList(expected4);
}

static void testGetCondListCharCategory() {
	CondList *test1 = getCondList("\\d");
	CondList *expected1 = initializeList();
	expected1 = addNodeToList(expected1,
		newCondNode(isDigitChar, '\0', '\0', NO_NEGATION));
	expected1 = finishList(expected1);
	TEST(listsAreEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondList(test1);
	cleanCondList(expected1);

	CondList *test2 = getCondList("\\D");
	CondList *expected2 = initializeList();
	expected2 = addNodeToList(expected2,
		newCondNode(isDigitChar, '\0', '\0', YES_NEGATION));
	expected2 = finishList(expected2);
	TEST(listsAreEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondList(test2);
	cleanCondList(expected2);

	CondList *test3 = getCondList("\\w");
	CondList *expected3 = initializeList();
	expected3 = addNodeToList(expected3,
		newCondNode(isWordChar, '\0', '\0', NO_NEGATION));
	expected3 = finishList(expected3);
	TEST(listsAreEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondList(test3);
	cleanCondList(expected3);
}

static void testGetCondListSimpleCharClass() {
	CondList *test1 = getCondList("[C]");
	CondList *expected1 = initializeList();
	expected1 = addNodeToList(expected1,
		newCondNode(equalsChar, 'C', '\0', NO_NEGATION));
	expected1 = finishList(expected1);
	TEST(listsAreEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondList(test1);
	cleanCondList(expected1);

	CondList *test2 = getCondList("[^q]");
	CondList *expected2 = initializeList();
	expected2 = addNodeToList(expected2,
		newCondNode(equalsChar, 'q', '\0', YES_NEGATION));
	expected2 = finishList(expected2);
	TEST(listsAreEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondList(test2);
	cleanCondList(expected2);

	CondList *test3 = getCondList("[R-b]");
	CondList *expected3 = initializeList();
	expected3 = addNodeToList(expected3,
		newCondNode(inRangeChar, 'R', 'b', NO_NEGATION));
	expected3 = finishList(expected3);
	TEST(listsAreEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondList(test3);
	cleanCondList(expected3);

	CondList *test4 = getCondList("[^\\d]");
	CondList *expected4 = initializeList();
	expected4 = addNodeToList(expected4,
		newCondNode(isDigitChar, '\0', '\0', YES_NEGATION));
	expected4 = finishList(expected4);
	TEST(listsAreEquivalent(test4, expected4) == TRUE_BOOL);
	cleanCondList(test4);
	cleanCondList(expected4);
}

static void testGetCondListComplexCharClass() {
	CondList *test1 = getCondList("[eZ]");
	CondList *expected1 = initializeList();
	expected1 = addNodeToList(expected1,
		newCondNode(equalsChar, 'e', '\0', NO_NEGATION));
	expected1 = addNodeToList(expected1,
		newCondNode(equalsChar, 'Z', '\0', NO_NEGATION));
	expected1 = finishList(expected1);
	TEST(listsAreEquivalent(test1, expected1) == TRUE_BOOL);
	cleanCondList(test1);
	cleanCondList(expected1);

	CondList *test2 = getCondList("[-#-%!]");
	CondList *expected2 = initializeList();
	expected2 = addNodeToList(expected2,
		newCondNode(equalsChar, '-', '\0', NO_NEGATION));
	expected2 = addNodeToList(expected2,
		newCondNode(inRangeChar, '#', '%', NO_NEGATION));
	expected2 = addNodeToList(expected2,
		newCondNode(equalsChar, '!', '\0', NO_NEGATION));
	expected2 = finishList(expected2);
	TEST(listsAreEquivalent(test2, expected2) == TRUE_BOOL);
	cleanCondList(test2);
	cleanCondList(expected2);

	CondList *test3 = getCondList("[^Wua-h~-]");
	CondList *expected3 = initializeList();
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, 'W', '\0', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, 'u', '\0', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(inRangeChar, 'a', 'h', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, '~', '\0', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, '-', '\0', YES_NEGATION));
	expected3 = finishList(expected3);
	TEST(listsAreEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondList(test3);
	cleanCondList(expected3);
}
