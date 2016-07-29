#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "path.h"

struct StateNode {
	RegexNode *matchedRegexes;
	PathNode *paths;
	int id, links;
};

static int id_gen = 0;
static void cleanState(StateNode *);

StateNode *newState(RegexNode *matchedRegexes) {
	StateNode *result = malloc(sizeof(StateNode));
	result->matchedRegexes = matchedRegexes;
	result->paths = NULL;
	result->id = id_gen;
	result->links = 1;
	id_gen++;
	return result;
}

static StateNode *addLink(StateNode *state) {
	state->links += 1;
	return state;
}

static void *removeLink(StateNode *state) {
	if(state == NULL) return;
	state->links -= 1;
	if(state->links == 0) {
		cleanState(state);
	}
}

StateNode *removeState(StateNode *state) {
	removeLink(state);
	return NULL;
}

StateNode *addStatePath(StateNode *src, StateNode *dst,
	CondNode *conds) {
	PathNode *path = addPath(src->paths, conds);
	path = addDestToPath(path, dst);
	src->paths = path;
	addLink(dst);
	return src;
}

int stateId(StateNode *state) {
	return state == NULL? -1: state->id;
}

PathNode *getPaths(StateNode *state) {
	return state->paths;
}

PathNode *nextAppropriatePath(char input, PathNode *start) {
	PathNode *path = start;
	while(path != NULL) {
		if(satisfiesPath(input, path)) return path;
		path = nextPath(path);
	}
	return NULL;
}

static void cleanState(StateNode *state) {
	if(state == NULL) return;
	//TODO: clean regexes
	cleanPath(state->paths);
	free(state);
}
