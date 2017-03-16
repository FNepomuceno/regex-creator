#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>

int isDigitChar(char input, char ignore1, char ignore2) {
	return isdigit(input);
}

int isWordChar(char input, char ignore1, char ignore2) {
	return isalpha(input) || input == '_';
}

int isWhitespace(char input, char ignore1, char ignore2) {
	return isspace(input);
}

int equalsChar(char input, char target, char ignore) {
	return input == target;
}

int inRangeChar(char input, char first, char last) {
	return input >= first && input <= last;
}

int otherwiseChar(char ignore, char ignore1, char ignore2) {
	return (0 == 0);
}
