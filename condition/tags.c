#include "tags.h"

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
int isValidOperationTag(const OperationTag *tag) {
	return isUsableOperationTag(tag) ||
		tag == IGNORE_OPERATION;
}

int isUsableOperationTag(const OperationTag *tag) {
	return tag == OR_OPERATION ||
		tag == AND_OPERATION;
}

//temporary function for refactoring
int getOperationTagId(const OperationTag *tag) {
	if(!isValidOperationTag(tag)) return INVALID_TAG;
	return tag->identifier;
}
const OperationTag *getOpTagFromId(int id) {
	if(id == OR_OPERATION->identifier) {
		return OR_OPERATION;
	}
	if(id == AND_OPERATION->identifier) {
		return AND_OPERATION;
	}
	if(id == IGNORE_OPERATION->identifier) {
		return IGNORE_OPERATION;
	}
	return INVALID_OPERATION;
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
int isValidNegationTag(const NegationTag *tag) {
	return isUsableNegationTag(tag) ||
		tag == IGNORE_NEGATION;
}

int isUsableNegationTag(const NegationTag *tag) {
	return tag == YES_NEGATION ||
		tag == NO_NEGATION;
}

//temporary function for refactoring
int getNegationTagId(const NegationTag *tag) {
	if(!isValidNegationTag(tag)) return INVALID_TAG;
	return tag->identifier;
}
