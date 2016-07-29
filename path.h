#ifndef PATH_H
#define PATH_H

typedef struct PathNode  PathNode;
typedef struct StateNode StateNode;
typedef struct CondNode  CondNode;

int satisfiesPath(char, PathNode *);
PathNode *nextPath(PathNode *);
PathNode *addPath(PathNode *, CondNode *);
PathNode *addDestToPath(PathNode *, StateNode *);
void cleanPath(PathNode *);

#endif
