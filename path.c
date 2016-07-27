#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"

int satisfiesPath(char input, PathNode *path) {
	return satisfiesCond(input, path->conds);
}

PathNode *nextPath(PathNode *path) {
	if(path == NULL) return NULL;
	return path->nextpath;
}

PathNode *appendPath(PathNode *oldPath, PathNode *newPath) {
	if(newPath == NULL) return oldPath;
	newPath->nextpath = oldPath;
	return newPath;
}

PathNode *newPath(StateNode *dest, CondNode *conds) {
	PathNode *result = (PathNode *) malloc(sizeof(PathNode));
	result->dst = dest;
	result->conds = conds;
	result->nextpath = NULL;
	return result;
}

PathNode *addPath(PathNode *oldPath,
	StateNode *dest, CondNode *conds) {
	if(oldPath == NULL) return newPath(dest, conds);
	PathNode *result = newPath(dest, conds);
	return appendPath(oldPath, result);
}

void cleanPath(PathNode *path) {
	PathNode *cur = path;
	while(cur != NULL) {
		PathNode *prev = cur;
		cleanCond(prev->conds);
		cur = nextPath(cur);
		free(prev);
	}
}
