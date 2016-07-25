#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#include "condition.h"

int satisfiesPath(char input, PathNode *path) {
	CondNode *cond = path->conds;
	while(cond != NULL) {
		if(meetsCond(input, cond)) {
			return 1; // true
		}
		cond = nextCond(cond);
	}
	return 0; // false
}

PathNode *nextPath(PathNode *path) {
	if(path == NULL) return NULL;
	return path->nextpath;
}
