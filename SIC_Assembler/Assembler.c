#define _CRT_SECURE_NO_WARNINGS
#define MAX_INPUT_LINE 100
#define MAX_CHAR_LINE 30
#define LOCCTR_LENGTH 4

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
static LOCCTR Location[MAX_INPUT_LINE];

State* make_intermediate();
void write_file(State* intermediate);
Symbol* insert_symtab(State *sic);

int main(void) {
	int program_length;
	locctr = 0;;

	State* intermediate = make_intermediate();
	Symbol* symtab = insert_symtab(intermediate);
	write_file(intermediate);
	program_length = locctr - starting_address;
	// 1PASS 끝//



	// Print
	printf("\n\n\t\t1PASS 종료\n\n");
	printf("%-8s%-10s%-10s%-10s%-10s\n", "LINE","LOCATION","LABEL","OPCODE","OPERAND");
	printf("------------------------------------------------\n");
	for (int i = 0; i < input_counter; i++) {
		printf("%-8s%-10s%-10s%-10s%-10s\n", intermediate[i].line, Location[i].loc, 
			intermediate[i].label, intermediate[i].opcode, intermediate[i].operand);
	}
	printf("\n\n\tSYMTAB\n\n");
	printf("%-10s%-10s%-10s\n", "LABEL", "LOCATION", "ERROR");
	printf("--------------------------\n");
	for (int j = 0; *symtab[j].label != NULL; j++) {
		printf("%-10s%-10s%-10d\n", symtab[j].label, symtab[j].location, symtab[j].error_flag);
	}


	////SYMTAB 카운트 조건 //이거되면 전역변수에 static int symtab_counter필요없음
	//for (int i = 0; *symtab[i].label != NULL; i++)
	//	printf("%s\n", symtab[i].label);
	return 0;
}


Symbol* insert_symtab(State* sic) {
	static Symbol symtab[MAX_INPUT_LINE] = { 0, };
	int check, overlap, i;
	int row = 0, present = 0, counter=0;
	char temp[LOCCTR_LENGTH+1];
	char* hexa = (char*)malloc(sizeof(char)* LOCCTR_LENGTH +1);
	i = 0;

	while (strcmp(sic[i].opcode, "END")) {
		if (!strcmp(sic[i].opcode, "START")) {
			starting_address = convert_dec(sic[i].operand);
			locctr = starting_address;
			// START의 현재 location 저장
			convert_Hx(locctr, hexa, LOCCTR_LENGTH);
			strcopy(Location[i].loc, hexa);
			i++;
			continue;
		} 
		// 현재 location을 저장
		convert_Hx(locctr, hexa, LOCCTR_LENGTH);
		strcopy(Location[i].loc, hexa);

		// SYMTAB 중복 검사 후 삽입		
		if (*sic[i].label != NULL) {
			overlap = 0;
			for (check = 0; check < counter; check++) {
				if (!strcmp(symtab[check].label, sic[i].label)) {
					symtab[check].error_flag = 1;
					overlap = 1;
					printf("ERROR : duplicate SYMBOL");
					break;
				}
			}
			if (overlap != 1) {
				strcopy(symtab[present].label, sic[i].label);
				convert_Hx(locctr, hexa, LOCCTR_LENGTH);
				strcopy(symtab[present].location, hexa);
				symtab[present].error_flag = 0;
				counter++;
			}
			present++;
		}

		overlap = 0;
		// OPTAB 검색 후 다음 명령어의 Locctr값 설정
		for (check = 0; check < (sizeof(OPTAB) / 2) / sizeof(OPTAB[0].mnemonic); check++) {
			if (!strcmp(sic[i].opcode, OPTAB[check].mnemonic)) {
				locctr += 3;
				overlap = 1;
				break;
			}
		}
		if (overlap == 1) {
			i++;
			continue;
		}
		if (!strcmp(sic[i].opcode, "WORD")) {
			locctr += 3;
		}	
		else if (!strcmp(sic[i].opcode, "RESW")) {
			locctr += 3 * atoi(sic[i].operand);
		}
		else if (!strcmp(sic[i].opcode, "RESB")) {
			locctr += atoi(sic[i].operand);
		}
		else if (!strcmp(sic[i].opcode, "BYTE")) {
			char* classify = strchr(sic[i].operand, 39);
			if (classify != NULL) {
				char* type = classify - 1;
				if (*type == 'C') {
					type += 2;
					while (*type != 39) {
						locctr++;
						type++;
					}
				}
				else if (*type == 'X') {
					int count = 0;
					type += 2;
					while (*type != 39) {
						count++;
						type++;
					}
					locctr += count / 2;
				}
			}
			else {
				locctr += atoi(sic[i].operand);
			}
		}
		else {
			printf("ERROR : Invalid OPERATION code (LINE: %s)\n", sic[i].line);
			exit(1);
		}
		i++;
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

	sic = fopen("./sic_input2.txt", "r");
	if (sic == NULL) {
		printf("\nERROR : Can't open the file!\n");
		exit(1);
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
