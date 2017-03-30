#define CONDITION_PACKAGE
//#define TEST_CONDITION_CONDLIST

#include <stdlib.h>
#include "node.h"
#include "stringtype.h"
#include "tags.h"
#include "function/info.h"
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

static CondList *newCharCategoryList(char *str) {
	CondList *result = initializeList();
	char category_type = *(str+1);
	CondFunc *result_func = NULL;
	switch(category_type) {
		case 'w':
			result_func = isWordChar;
			break;
		case 'd':
			result_func = isDigitChar;
			break;
		case 's':
			result_func = isWhitespace;
			break;
		default:
			//Default already set to NULL
			break;
	}
	result = addNodeToList(result,
		newCondNode(result_func, '\0', '\0', NO_NEGATION));
	return finishList(result);
}

static CondList *newNegatedCharCategoryList(char *str) {
	CondList *result = initializeList();
	char category_type = *(str+1);
	CondFunc *result_func = NULL;
	switch(category_type) {
		case 'W':
			result_func = isWordChar;
			break;
		case 'D':
			result_func = isDigitChar;
			break;
		case 'S':
			result_func = isWhitespace;
			break;
		default:
			//Default already set to NULL
			break;
	}
	result = addNodeToList(result,
		newCondNode(result_func, '\0', '\0', YES_NEGATION));
	return finishList(result);
}

static CondList *newEscapedLiteralCharList(char *str) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, *(str+1), '\0', NO_NEGATION));
	return finishList(result);
}

static CondList *newLinkedDataList(char *str) {
	int amt_data = getAmtDataInCharClass(str);
	if(amt_data <= 0) return nil_list;

	CondList *result = initializeList();
	const NegationTag *tag;
	char *cur_pos;
	if(isCharClass(str) == TRUE_BOOL) {
		tag = NO_NEGATION;
		cur_pos = str+1;
	} else if(isNegatedCharClass(str) == TRUE_BOOL) {
		tag = YES_NEGATION;
		cur_pos = str+2;
	} else {
		return nil_list;
	}
	for(int i = 0; i < amt_data; i++) {
		if(*(cur_pos+1) == '-') {
			char start = *cur_pos;
			char end = *(cur_pos+2);
			if(end < start) {
				cleanCondList(result);
				return nil_list;
			}
			result = addNodeToList(result,
				newCondNode(inRangeChar, start, end, tag));
			cur_pos = cur_pos + 3;
		} else {
			result = addNodeToList(result,
				newCondNode(equalsChar, *(cur_pos), '\0', tag));
			cur_pos = cur_pos + 1;
		}
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

	CondList *test3 = getCondList("[^Wua-h?]");
	CondList *expected3 = initializeList();
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, 'W', '\0', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, 'u', '\0', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(inRangeChar, 'a', 'h', YES_NEGATION));
	expected3 = addNodeToList(expected3,
		newCondNode(equalsChar, '?', '\0', YES_NEGATION));
	expected3 = finishList(expected3);
	TEST(listsAreEquivalent(test3, expected3) == TRUE_BOOL);
	cleanCondList(test3);
	cleanCondList(expected3);
}
