#ifndef MODULE_GRAPHREGEX_H
#define MODULE_GRAPHREGEX_H

char *getGraphRegex(char *str);
void cleanGraphRegex(char *graphRegex);

char *INVALID_STRING = "";
char *DEFAULT_STRING = "WRONG";

#ifdef MODULE_GRAPHREGEX_MOD

typedef struct GraphRegexList GraphList;
static void cleanGraphList(GraphList *list);

static GraphList *newGraphList(char *str);
static int lengthOfGraphListNode(GraphList *list);
static GraphList *nextGraphListNode(GraphList *list,
	char *str, int length);
static int nextGraphListLength(char *str, int length);
static char nextGraphListType(char *str, int length);
static GraphList *addGraphListNode(GraphList *list,
	int new_length, char data_type);
static GraphList *linkGraphListNodes(GraphList *src,
	GraphList *inp);
static GraphList *setGraphListNodeData(GraphList *list,
	int length, char data_type);

#include "testing.h"
#include "utils/test.h"
static TEST_CASE(GetGraphRegex);
static TEST_RES(char *, CopyStr, char *str);

static TEST_CASE(GetGraphRegexNull);
static TEST_CASE(GetGraphRegexLoneChar);
static TEST_CASE(GetGraphRegexLoneCharCategory);
static TEST_CASE(GetGraphRegexLoneSimpleCharClass);
static TEST_CASE(GetGraphRegexLoneComplexCharClass);
static TEST_CASE(GetGraphRegexStringOfChars);
static TEST_CASE(GetGraphRegexStringOfMixedItems);
static TEST_CASE(GetGraphRegexOr);
static TEST_CASE(GetGraphRegexGroup);
static TEST_CASE(GetGraphRegexGroupWithOr);
static TEST_CASE(GetGraphRegexLineBeginning);
static TEST_CASE(GetGraphRegexLineEnding);
static TEST_CASE(GetGraphRegexUsingStar);
static TEST_CASE(GetGraphRegexUsingPlus);
static TEST_CASE(GetGraphRegexUsingQuestion);
static TEST_CASE(GetGraphRegexUsingFixedRepetition);
static TEST_CASE(GetGraphRegexUsingVariableRepetition);
static TEST_CASE(GetGraphRegexUsingInfiniteRepetition);

#endif
#endif
