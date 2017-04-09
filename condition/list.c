#define CONDITION_PACKAGE
#define CONDITION_CONDLIST_MOD

#include <stdlib.h>
#include "../testing.h"
#include "list.h"
#include "node.h"
#include "tags.h"
#include "function/info.h"
#include "../utils/bool.h"
#include "../utils/chartype.h"
#include "../utils/test.h"

struct CondList {
	CondNode *_data_node;
	CondList *_next_item;
	CondList *_prev_item;
};
CondList nil_list_data;
CondList *nil_list = &nil_list_data;

CondList *getCondList(char *str) {
	int length = lengthOfString(str);
	if(hasNoChar(str, length) == TRUE_BOOL) {
		return nil_list;
	}
	if(hasWildcardChar(str, length) == TRUE_BOOL &&
			length == LONE_CHAR_LENGTH) {
		return newLoneCharList("\n", YES_NEGATION);
	}
	if(isParseSingleChar(str, length) == TRUE_BOOL) {
		return newLoneCharList(str, NO_NEGATION);
	}
	if(hasCharCategory(str, length) == TRUE_BOOL) {
		return newCharCategoryList(str);
	}
	if(hasNegatedCharCategory(str, length) == TRUE_BOOL) {
		return newNegatedCharCategoryList(str);
	}
	if(hasNormalEscapedChar(str, length) == TRUE_BOOL) {
		return newEscapedLiteralCharList(str);
	}
	if(charClassLength(str, length) != length) {
		return nil_list;
	}
	return newLinkedDataList(str, length);
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
		cur_list = advanceList(cur_list);
	}
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

static CondList *newLinkedDataList(char *str, int length) {
	int num_chars = classDataLength(str, length);
	if(num_chars <= 0) return nil_list;

	CondList *result = initializeList();
	const NegationTag *tag = getNegationTag(
		isCharClassNegated(str, length));
	int cur_num_chars = num_chars;
	char *cur_str = str+length-num_chars-1;
	while(cur_num_chars > 0) {
		int data_size = dataSize(cur_str, cur_num_chars);
		if(data_size == LONE_CHAR_LENGTH) {
			result = addNodeToList(result,
				newCondNode(equalsChar, *(cur_str), '\0', tag));
		} else if(data_size == ESCAPED_CHAR_LENGTH) {
			result = addEscapedCharNodeToList(result,
				*(cur_str+1), tag);
		} else if(data_size == CHAR_RANGE_LENGTH) {
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

static CondList *initializeList() {
	CondList *new_list = malloc(sizeof (CondList));
	new_list->_data_node = nil_node;
	new_list->_next_item = new_list;
	new_list->_prev_item = new_list;
	return new_list;
}

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

static CondList *addNewListItem(CondList *list, CondNode *node) {
	CondList *new_list = malloc(sizeof (CondList));
	new_list->_data_node = node;
	new_list->_next_item = list;
	new_list->_prev_item = list->_prev_item;
	insertListItem(list, new_list);
	return list;
}

static void insertListItem(CondList *dst, CondList *src) {
	dst->_prev_item->_next_item = src;
	dst->_prev_item = src;
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

#ifdef TESTING_CONDITION_CONDLIST
#ifdef TEST_THIS
int main() {
	USE_CASE(GetCondList);
	return 0;
}
#endif

TEST_CASE(GetCondList) {
	USE_CASE(GetCondListNull);
	USE_CASE(GetCondListSingleChar);
	USE_CASE(GetCondListCharCategory);
	USE_CASE(GetCondListSimpleCharClass);
	USE_CASE(GetCondListComplexCharClass);
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

static TEST_CASE(GetCondListNull) {
	COMPARE_TEST(CondList *, Null,
		getCondList(NULL),
		nil_list,
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, Nil,
		getCondList(""),
		nil_list,
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
}

static TEST_CASE(GetCondListSingleChar) {
	COMPARE_TEST(CondList *, LoneChar,
		getCondList("a"),
		USE_RES(LoneChar,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, EscapedLiteral,
		getCondList("\\g"),
		USE_RES(EscapedLiteral,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, EscapedMeta,
		getCondList("\\."),
		USE_RES(EscapedMeta,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, WildcardChar,
		getCondList("."),
		USE_RES(WildcardChar,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
}

static TEST_RES(CondList *, LoneChar, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, 'a', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, EscapedLiteral, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, 'g', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, EscapedMeta, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, '.', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, WildcardChar, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, '\n', '\0', YES_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_CASE(GetCondListCharCategory) {
	COMPARE_TEST(CondList *, DigitCategory,
		getCondList("\\d"),
		USE_RES(DigitCategory,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, NegatedDigitCategory,
		getCondList("\\D"),
		USE_RES(NegatedDigitCategory,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, WordCategory,
		getCondList("\\w"),
		USE_RES(WordCategory,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
}

static TEST_RES(CondList *, DigitCategory, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(isDigitChar, '\0', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, NegatedDigitCategory, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(isDigitChar, '\0', '\0', YES_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, WordCategory, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(isWordChar, '\0', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_CASE(GetCondListSimpleCharClass) {
	COMPARE_TEST(CondList *, SingleCharClass,
		getCondList("[C]"),
		USE_RES(SingleCharClass,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, NegatedSingleCharClass,
		getCondList("[^q]"),
		USE_RES(NegatedSingleCharClass,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, SingleRangeClass,
		getCondList("[R-b]"),
		USE_RES(SingleRangeClass,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, EscapedCharClass,
		getCondList("[^\\d]"),
		USE_RES(EscapedCharClass,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
}

static TEST_RES(CondList *, SingleCharClass, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, 'C', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, NegatedSingleCharClass, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, 'q', '\0', YES_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, SingleRangeClass, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(inRangeChar, 'R', 'b', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, EscapedCharClass, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(isDigitChar, '\0', '\0', YES_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_CASE(GetCondListComplexCharClass) {
	COMPARE_TEST(CondList *, DoubleLetterClass,
		getCondList("[eZ]"),
		USE_RES(DoubleLetterClass,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, ClassWithDash,
		getCondList("[-#-%!]"),
		USE_RES(ClassWithDash,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
	COMPARE_TEST(CondList *, NegatedComplexClass,
		getCondList("[^Wua-h~-]"),
		USE_RES(NegatedComplexClass,),
		listsAreEquivalent, TRUE_BOOL,
		cleanCondList);
}

static TEST_RES(CondList *, DoubleLetterClass, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, 'e', '\0', NO_NEGATION));
	result = addNodeToList(result,
		newCondNode(equalsChar, 'Z', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, ClassWithDash, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, '-', '\0', NO_NEGATION));
	result = addNodeToList(result,
		newCondNode(inRangeChar, '#', '%', NO_NEGATION));
	result = addNodeToList(result,
		newCondNode(equalsChar, '!', '\0', NO_NEGATION));
	result = finishList(result);
	return result;
}

static TEST_RES(CondList *, NegatedComplexClass, void) {
	CondList *result = initializeList();
	result = addNodeToList(result,
		newCondNode(equalsChar, 'W', '\0', YES_NEGATION));
	result = addNodeToList(result,
		newCondNode(equalsChar, 'u', '\0', YES_NEGATION));
	result = addNodeToList(result,
		newCondNode(inRangeChar, 'a', 'h', YES_NEGATION));
	result = addNodeToList(result,
		newCondNode(equalsChar, '~', '\0', YES_NEGATION));
	result = addNodeToList(result,
		newCondNode(equalsChar, '-', '\0', YES_NEGATION));
	result = finishList(result);
	return result;
}
#endif
