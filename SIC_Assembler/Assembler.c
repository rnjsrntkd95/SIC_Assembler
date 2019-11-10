#define _CRT_SECURE_NO_WARNINGS
#define MAX_INPUT_LINE 100

#include <stdio.h>
#include <string.h>
#include "optab.h"
#include "symtab.h"
#include "my_function.h"

typedef struct Statement {
	char line[MAX_CHAR_LINE];
	char label[MAX_CHAR_LINE];
	char operator[MAX_CHAR_LINE];
	char operand[MAX_CHAR_LINE];
} State;

int main(void) {
	int counter = 1000;
	//FILE* sic;
	//sic = fopen("./sic_input.txt","wt");

	char *loc = convert_Hx(counter);
	printf("%s", loc);

	return 0;
}
