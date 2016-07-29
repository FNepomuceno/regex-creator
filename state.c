#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "path.h"

struct StateNode {
	RegexNode *matchedRegexes;
	PathNode *paths;
	int id;
};

static int id_gen = 0;

StateNode *newState(RegexNode *matchedRegexes) {
	StateNode *result = malloc(sizeof(StateNode));
	result->matchedRegexes = matchedRegexes;
	result->paths = NULL;
	result->id = id_gen;
	id_gen++;
	return result;
}

StateNode *addStatePath(StateNode *src, StateNode *dst,
	CondNode *conds) {
	PathNode *path = addPath(src->paths, conds);
	path = addDestToPath(path, dst);
	src->paths = path;
	return src;
}

int stateId(StateNode *state) {
	return state == NULL? -1: state->id;
}

PathNode *nextAppropriatePath(char input, PathNode *start) {
	PathNode *path = start;
	while(path != NULL) {
		if(satisfiesPath(input, path)) return path;
		path = nextPath(path);
	}
	return NULL;
}

void cleanState(StateNode *state) {
	//cleanPath(path);
}
