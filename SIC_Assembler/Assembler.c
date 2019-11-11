#define _CRT_SECURE_NO_WARNINGS
#define MAX_INPUT_LINE 100

#include <stdio.h>
#include "optab.h"
#include "symtab.h"
#include "my_function.h"

typedef struct _Statement {
	char line[MAX_CHAR_LINE];
	char label[MAX_CHAR_LINE];
	char opcode[MAX_CHAR_LINE];
	char operand[MAX_CHAR_LINE];
} State;

State* make_intermediate();

int main(void) {
	int locctr_length = 4;
	State* intermediate = make_intermediate();

	//int counter = 1000;
	//char aa[10];
	//char* loc = (char*)malloc(sizeof(char)*10);
	//convert_Hx(counter, aa, locctr_length);
	//printf("%s\n", aa);
	//free(loc);

	return 0;
}

State* make_intermediate() {
	State intermediate[MAX_INPUT_LINE] = { 0, };
	FILE* sic;
	int i = 0;
	char get_line[MAX_CHAR_LINE];
	sic = fopen("./sic_input.txt", "r");
	if (sic == NULL) {
		printf("ERROR : Can't open the file!\n");
		return 0;

	}
	while (fgets(get_line, sizeof(get_line), sic) != NULL) {
		printf("%s", get_line);
		strcpy(intermediate[i].line, strtok(get_line, " \t\n"));
		strcpy(intermediate[i].label, strtok(NULL, " \t\n"));
		strcpy(intermediate[i].opcode, strtok(NULL, " \t\n"));
		strcpy(intermediate[i].operand, strtok(NULL, " \t\n"));

		// (...)주석 라인 넘김 처리
		if (*intermediate[i].label == '.') {
			continue;
		}

		if (*intermediate[i].operand == NULL) {
			strcpy(intermediate[i].operand, intermediate[i].opcode);
			strcpy(intermediate[i].opcode, intermediate[i].label);
			*intermediate[i].label = NULL;
		}
		i++;
	}
	fclose(sic);

	return intermediate;
}