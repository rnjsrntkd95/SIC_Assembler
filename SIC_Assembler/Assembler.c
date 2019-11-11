#define _CRT_SECURE_NO_WARNINGS
#define MAX_INPUT_LINE 100

#include <stdio.h>
#include "optab.h"
#include "symtab.h"
#include "my_function.h"

typedef struct _Statement {
	char *line;
	char *label;
	char *opcode;
	char *operand;
} State;

int main(void) {
	FILE* sic;
	char get_line[MAX_CHAR_LINE];
	static State intermediate[MAX_INPUT_LINE];
	int i = 0;
	sic = fopen("./sic_input.txt","r");
	if (sic == NULL) {
		printf("ERROR : Can't open the file!\n");
		return 0;
	}

	while (fgets(get_line, sizeof(get_line), sic) != NULL) {
		printf("%s", get_line);
		intermediate[i].line = strtok(get_line, " \t\n");
		intermediate[i].label = strtok(NULL, " \t\n");
		intermediate[i].opcode = strtok(NULL, " \t\n");
		intermediate[i].operand = strtok(NULL, " \t\n");

		// 주석 라인 저장 후 continue
		if (intermediate[i].label == '.')
			continue;

		if (intermediate[i].operand == NULL) {
			intermediate[i].operand = intermediate[i].opcode;
			intermediate[i].opcode = intermediate[i].label;
			intermediate[i].label = NULL;
		}
		i++;
	}


	
	//int counter = 1000;
	//char* loc = convert_Hx(counter);
	//printf("%s", loc);
	//free(loc);
	fclose(sic);

	return 0;
}
