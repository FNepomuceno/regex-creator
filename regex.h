#ifndef REGEX_H
#define REGEX_H

typedef struct RegexNode RegexNode;

struct RegexNode {
	char **name;
	char **label;
	RegexNode *nextregex;
};

#endif
