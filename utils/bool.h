#ifndef BOOL_PBL
#define BOOL_PBL

extern const int TRUE_BOOL;
extern const int FALSE_BOOL;

int toBool(int eval);
int boolOr(int, int);
int boolAnd(int, int);
int boolXor(int, int);
int boolEquate(void *thing1, void *thing2);
int boolInvert(int);

#endif
