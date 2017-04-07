#ifndef MODULE_CHARTYPEREGEX_H
#define MODULE_CHARTYPEREGEX_H

enum {
	INVALID_CHARTYPE = 'X',
	DATA_CHARTYPE = '#',
	OR_CHARTYPE = '|',
	AND_CHARTYPE = '&',
	BEGIN_GROUP_CHARTYPE = '(',
	END_GROUP_CHARTYPE = ')',
	BEGIN_TEXT_CHARTYPE = '^',
	END_TEXT_CHARTYPE = '$',
	KLEENE_STAR_CHARTYPE = '*',
	PLUS_CHARTYPE = '+',
	OPTIONAL_MARK_CHARTYPE = '?',
	FIXED_REPETITION_CHARTYPE = '!',
	VARIABLE_REPETITION_CHARTYPE = '@',
	INFINITE_REPETITION_CHARTYPE = '%'
};

int hasParseLoneChar(char *str, int length);
int hasGraphLoneChar(char *str, int length);
int hasWildcardChar(char *str, int length);
int hasCharCategory(char *str, int length);
int hasNegatedCharCategory(char *str, int length);
int hasNormalEscapedChar(char *str, int length);
int charClassLength(char *str, int length);
int hasCharClass(char *str, int length);
int hasNegatedCharClass(char *str, int length);
int hasOrChar(char *str, int length);
int hasGroupBeginning(char *str, int length);
int hasGroupEnding(char *str, int length);
int hasTextBeginning(char *str, int length);
int hasTextEnding(char *str, int length);
int hasKleeneStar(char *str, int length);
int hasPlusChar(char *str, int length);
int hasQuestionMark(char *str, int length);
int hasFixedRepetition(char *str, int length);
int hasVariableRepetition(char *str, int length);
int hasInfiniteRepetition(char *str, int length);
int enclosedBraceLength(char *str, int length);
void setFixedRepetitions(char *str, int *p_num_reps);
void setVariableRepetitions(char *str, int *p_min_reps,
	int *p_max_reps);
void setInfiniteRepetitions(char *str, int *p_min_reps);

#ifdef MODULE_CHARTYPEREGEX_MOD

static int hasSingleChar(char *str, int length);
static int hasParseMetaChar(char *str, int length);
static int hasMetaChar(char *str, int length);
static int hasEscapedChar(char *str, int length);
static int enclosedBracketLength(char *str, int length);
static int indexOfCommaInBraces(char *str, int length);

#endif
#endif
