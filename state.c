#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "path.h"

PathNode *nextAppropriatePath(char input, PathNode *start) {
	PathNode *path = start;
	while(path != NULL) {
		if(satisfiesPath(input, path)) return path;
		path = nextPath(path);
	}
	return NULL;
}
