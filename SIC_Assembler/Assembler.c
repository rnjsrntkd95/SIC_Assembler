#define _CRT_SECURE_NO_WARNINGS
#define MAX_INPUT_LINE 100
#define MAX_CHAR_LINE 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "my_function.h"

typedef struct _Statement {
	char line[MAX_CHAR_LINE];
	char label[MAX_CHAR_LINE];
	char opcode[MAX_CHAR_LINE];
	char operand[MAX_CHAR_LINE];
} State;

static int input_counter;
static int starting_address;
static int locctr;

State* make_intermediate();
void write_file(State* intermediate);
Symbol* insert_symtab(State *sic, int length);

int main(void) {
	int locctr_length = 4; // Location의 값을 몇 자리의 16진수로 표현할 것인지
	locctr = 0;
	State* intermediate = make_intermediate();
	Symbol* symtab = insert_symtab(intermediate, locctr_length);
	write_file(intermediate);
	// 1PASS 끝//


	////SYMTAB 카운트 조건 //이거되면 전역변수에 static int symtab_counter필요없음
	//for (int i = 0; *symtab[i].label != NULL; i++)
	//	printf("%s\n", symtab[i].label);
	return 0;
}


Symbol* insert_symtab(State* sic, int length) {
	static Symbol symtab[MAX_INPUT_LINE] = { 0, };
	int check, overlap, i, j;
	int present = 0, counter=0;
	char* hexa = (char*)malloc(sizeof(char)*length+1);

		
	for (i = 0; i < input_counter; i++) {
		// SYMTAB 중복 검사 후 삽입
		if (*sic[i].label == NULL) {
			continue;
		}
		overlap = 0;
		for (check = 0; check < counter; check++) {
			if (!strcmp(symtab[check].label, sic[i].label)) {
				symtab[check].error_flag = 1;
				overlap = 1;
				break;
			}
		}
		if (overlap != 1) {
			strcopy(symtab[present].label, sic[i].label);
			convert_Hx(locctr, hexa, length);
			strcopy(symtab[present].location, hexa);
			symtab[present].error_flag = 0;
			counter++;
		}

		// OPTAB 검색 후 다음 명령어의 Locctr값 설정


			
		present++;
	}
	free(hexa);

	return symtab;
}


// read SIC input file
State* make_intermediate() {
	static State intermediate[MAX_INPUT_LINE] = { 0, };
	FILE* sic;
	int i = 0;
	char get_line[MAX_CHAR_LINE];
	input_counter = 0;

	sic = fopen("./sic_input.txt", "r");
	if (sic == NULL) {
		printf("\nERROR : Can't open the file!\n");
		return 0;
	}
	while (fgets(get_line, sizeof(get_line), sic) != NULL) {
		input_counter++;
		printf("%s", get_line);
		strcopy(intermediate[i].line, strtok(get_line, " \t\n"));
		strcopy(intermediate[i].label, strtok(NULL, " \t\n"));
		strcopy(intermediate[i].opcode, strtok(NULL, " \t\n"));
		strcopy(intermediate[i].operand, strtok(NULL, " \t\n"));

		// (...)주석 라인 넘김 처리
		if (*intermediate[i].label == '.') {
			//i++;			//intermediate file에서 주석라인은 저장x
			continue;
		}
		if (*intermediate[i].operand == NULL) {
			strcopy(intermediate[i].operand, intermediate[i].opcode);
			strcopy(intermediate[i].opcode, intermediate[i].label);
			*intermediate[i].label = NULL;
		}
		i++;
	}
	fclose(sic);

	return intermediate;
}


void write_file(State *intermediate) {
	FILE* sic;
	sic = fopen("intermediate_file.txt", "w");
	for (int i = 0; i < MAX_CHAR_LINE; i++)
		fprintf(sic, "%s\t%s\t%s\t%s\n", intermediate[i].line, intermediate[i].label,
			intermediate[i].opcode, intermediate[i].operand);

	fclose(sic);
}
