#include <stdio.h>
#include <stdlib.h>

//condition.h
typedef struct CondNode CondNode;
typedef struct CondMakr CondMakr;
typedef (int)CondFunc(char, char, char);//TODO: look up syntax
static const int OR_OP = 1;
static const int AND_OP = 0;
static const int IGNORE_OP = -1;
static const int YES_NEGATE = 1;
static const int NO_NEGATE = 0;
static const int IGNORE_NEGATE = -1;


