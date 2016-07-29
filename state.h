#ifndef STATE_H
#define STATE_H

typedef struct StateNode StateNode;
typedef struct PathNode  PathNode;
typedef struct RegexNode RegexNode;
typedef struct CondNode  CondNode;

StateNode *newState(RegexNode *);
StateNode *removeState(StateNode *);
StateNode *addStatePath(StateNode *, StateNode *, CondNode *);
int stateId(StateNode *);
PathNode *getPaths(StateNode *);
PathNode *nextAppropriatePath(char, PathNode *);

#endif
