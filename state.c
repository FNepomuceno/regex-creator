#include <stdio.h>
#include <stdlib.h>

typedef struct StateNode StateNode;
typedef struct PathNode PathNode;
typedef struct CondNode CondNode;

//"state/state.c"
struct StateNode {
	char *result_type;
	PathNode *paths;
}

//TODO: Implement all these functions, will ya?

//Make a new state
//A null result type -> INVALID_STATE
StateNode *newState(char *result_type);

//If dst is null, send error and return
//If condition is null, send error and return
//Make new path with dst and condition
//add new path to src's paths
PathNode *linkToState(PathNode *src_paths, StateNode *dst,
		CondNode *condition);

//"state/state.c"
struct PathNode {
	PathNode *next_path;
	StateNode *dst_state;
	CondNode *condition;
}

//Make a new path
PathNode *newPath(StateNode *dst, CondNode *cond);

//If to is null, return from
//If from is null, return to
//If from's state and to's state are the same, merge paths
//If last is an else, add before that
//Otherwise add to end
PathNode *addPath(PathNode *from, PathNode *to);

//If either are null, send error and return
//If dst states are different, send error and return
PathNode *mergePaths(PathNode *a, PathNode *b);
