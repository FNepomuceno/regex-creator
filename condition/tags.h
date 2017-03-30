#ifndef CONDITION_TAGS_H
#define CONDITION_TAGS_H
#ifdef  CONDITION_PACKAGE

typedef struct OperationTag OperationTag;
extern const OperationTag *OR_OPERATION;
extern const OperationTag *AND_OPERATION;
extern const OperationTag *IGNORE_OPERATION;
extern const OperationTag *INVALID_OPERATION;
int isValidOperationTag(const OperationTag *tag);
int isUsableOperationTag(const OperationTag *tag);

typedef struct NegationTag NegationTag;
extern const NegationTag *YES_NEGATION;
extern const NegationTag *NO_NEGATION;
extern const NegationTag *IGNORE_NEGATION;
extern const NegationTag *INVALID_NEGATION;
int isValidNegationTag(const NegationTag *tag);
int isUsableNegationTag(const NegationTag *tag);

#endif
#endif
