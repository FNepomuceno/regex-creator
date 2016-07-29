#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"

struct PathNode {
	StateNode *dst;
	CondNode *conds;
	PathNode *nextpath;
};

int satisfiesPath(char input, PathNode *path) {
	return satisfiesCond(input, path->conds);
}

PathNode *nextPath(PathNode *path) {
	if(path == NULL) return NULL;
	return path->nextpath;
}

static PathNode *appendPath(PathNode *oldPath, PathNode *newPath) {
	if(newPath == NULL) return oldPath;
	newPath->nextpath = oldPath;
	return newPath;
}

static PathNode *newPath(CondNode *conds) {
	PathNode *result = (PathNode *) malloc(sizeof(PathNode));
	result->dst = NULL;
	result->conds = conds;
	result->nextpath = NULL;
	return result;
}

PathNode *addPath(PathNode *oldPath, CondNode *conds) {
	if(oldPath == NULL) return newPath(conds);
	PathNode *result = newPath(conds);
	return appendPath(oldPath, result);
}

PathNode *addDestToPath(PathNode *path, StateNode *dest) {
	if(path->dst == NULL) {
		path->dst = dest;
	}
	return path;
}

StateNode *getPathDest(PathNode *path) {
	if(path == NULL) return NULL;
	return path->dst;
}

void cleanPath(PathNode *path) {
	PathNode *cur = path;
	while(cur != NULL) {
		PathNode *prev = cur;
		cleanCond(prev->conds);
		removeState(prev->dst);
		cur = nextPath(cur);
		free(prev);
	}
}
