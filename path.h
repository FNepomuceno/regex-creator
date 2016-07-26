#ifndef PATH_H
#define PATH_H

typedef struct PathNode  PathNode;
typedef struct StateNode StateNode;
typedef struct CondNode  CondNode;

struct PathNode {
	StateNode *dst;
	CondNode *conds;
	PathNode *nextpath;
};

int satisfiesPath(char, PathNode *);
PathNode *nextPath(PathNode *);
PathNode *addPath(PathNode *, StateNode *, CondNode *);
void cleanPath(PathNode *);

#endif
