#define CONDITION_PACKAGE
#define TEST_CONDITION_CONDLIST

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

CondList *initializeList() {
	CondList *new_list = malloc(sizeof (CondList));
	new_list->_data_node = nil_node;
	new_list->_next_item = new_list;
	new_list->_prev_item = new_list;
	return new_list;
}

CondList *addNewListItem(CondList *list, CondNode *node);
CondList *addNodeToList(CondList *list, CondNode *node) {
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

void insertListItem(CondList *dst, CondList *src) {
	dst->_prev_item->_next_item = src;
	dst->_prev_item = src;
}

CondList *addNewListItem(CondList *list, CondNode *node) {
	CondList *new_list = malloc(sizeof (CondList));
	new_list->_data_node = node;
	new_list->_next_item = list;
	new_list->_prev_item = list->_prev_item;
	insertListItem(list, new_list);
	return list;
}

CondList *finishList(CondList *list) {
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

void cleanCondList(CondList *list) {
	if(list == NULL || list == nil_list) {
		return;
	}
	CondList *cur_list = finishList(list);
	if(cur_list->_prev_item != nil_list) {
		abort();
	}
	while(cur_list != nil_list) {
		CondList *nxt_list = cur_list->_next_item;
		cleanCondNode(cur_list->_data_node);
		free(cur_list);
		cur_list = nxt_list;
	}
}

CondList *getCondList(char *str) {
	return nil_list;
}

int listsAreEquivalent(CondList *list1, CondList *list2) {
	CondList *cur_list1 = list1;
	CondList *cur_list2 = list2;
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
	} while(cur_list1 != nil_list && cur_list2 != nil_list);
	return TRUE_BOOL;
}

//TODO: Make the CondList and extractCondList()
//TODO: Make tests for extractCondList() and make them pass
void testGetCondList();
#ifdef TEST_CONDITION_CONDLIST
int main() {
	testGetCondList();
	return 0;
}
#endif

void testGetCondListNull();
void testGetCondListSingleChar();
void testGetCondListCharCategory();
void testGetCondListSimpleCharClass();
void testGetCondListComplexCharClass();
void testGetCondList() {
	testGetCondListNull();
	testGetCondListSingleChar();
	testGetCondListCharCategory();
	testGetCondListSimpleCharClass();
	testGetCondListComplexCharClass();
}

void testGetCondListNull() {
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

void testGetCondListSingleChar() {
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

void testGetCondListCharCategory() {
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

void testGetCondListSimpleCharClass() {
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

void testGetCondListComplexCharClass() {
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
