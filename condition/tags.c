#include "tags.h"
#include "../utils/bool.h"

#define INVALID_TAG -2

typedef struct OperationTag OperationTag;
struct OperationTag {
	int identifier;
};

static const OperationTag op_tags[] = { {0}, {1}, {-1} };
const OperationTag *OR_OPERATION = op_tags+0;
const OperationTag *AND_OPERATION = op_tags+1;
const OperationTag *IGNORE_OPERATION = op_tags+2;
const OperationTag *INVALID_OPERATION;

int isUsableOperationTag(const OperationTag *tag);
int isValidOperationTag(const OperationTag *tag) {
	return isUsableOperationTag(tag) ||
		tag == IGNORE_OPERATION;
}

int isUsableOperationTag(const OperationTag *tag) {
	return tag == OR_OPERATION ||
		tag == AND_OPERATION;
}

typedef struct NegationTag NegationTag;
struct NegationTag {
	int identifier;
};

static const NegationTag neg_tags[] = { {0}, {1}, {-1} };
const NegationTag *YES_NEGATION = neg_tags+0;
const NegationTag *NO_NEGATION = neg_tags+1;
const NegationTag *IGNORE_NEGATION = neg_tags+2;
const NegationTag *INVALID_NEGATION;

int isUsableNegationTag(const NegationTag *tag);
int isValidNegationTag(const NegationTag *tag) {
	return isUsableNegationTag(tag) ||
		tag == IGNORE_NEGATION;
}

int isUsableNegationTag(const NegationTag *tag) {
	return tag == YES_NEGATION ||
		tag == NO_NEGATION;
}

const NegationTag *negateNegationTag(const NegationTag *tag) {
	if(tag == YES_NEGATION) {
		return NO_NEGATION;
	}
	if(tag == NO_NEGATION) {
		return YES_NEGATION;
	}
	return INVALID_NEGATION;
}

const NegationTag *getNegationTag(int bool_val) {
	if(bool_val == TRUE_BOOL) {
		return YES_NEGATION;
	}
	if(bool_val == FALSE_BOOL) {
		return NO_NEGATION;
	}
	return INVALID_NEGATION;
}
