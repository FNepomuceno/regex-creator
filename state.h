#ifndef STATE_H
#define STATE_H

typedef struct StateNode StateNode;
typedef struct RegexNode RegexNode;
typedef struct PathNode  PathNode;

struct StateNode {
	RegexNode *matchedRegexes;
	PathNode *paths;
	int id;
};

PathNode *nextAppropriatePath(char, PathNode *);

#endif
