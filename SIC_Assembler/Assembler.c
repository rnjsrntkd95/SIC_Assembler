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

typedef struct  _Objectcode {
	char code[MAX_CHAR_LINE];
} Object;

static int input_counter;
static int starting_address;
static int locctr;
static LOCCTR Location[MAX_INPUT_LINE];

State* read_file();
void write_file(State* sic);
Symbol* pass_1(State* sic);
Object* pass_2(Symbol* symtab, int program_length);

int main(void) {
	int program_length;
	locctr = 0;;

	printf("\n\t[ 1PASS 시작 ]\n\n");
	printf("%-8s%-8s%-8s%-8s\n", "LINE", "LABEL", "OPCODE", "OPERAND");
	printf("--------------------------------\n");
	State* sic = read_file();
	Symbol* symtab = pass_1(sic);
	write_file(sic);
	program_length = locctr - starting_address;
	// 1PASS 끝//
	// Print
	printf("\n\n\n\t[ SYMTAB ]\n\n");
	printf("%-10s%-10s%-10s\n", "LABEL", "LOCATION", "ERROR");
	printf("--------------------------\n");
	for (int j = 0; *symtab[j].label != '\0'; j++) {
		printf("%-10s%-10s%-10d\n", symtab[j].label, symtab[j].location, symtab[j].error_flag);
	}

	printf("\n\n\n\t\t[ 1PASS 종료 ]\n\n");
	printf("%-8s%-10s%-10s%-10s%-10s%-15s\n", "LINE", "LOCATION", "LABEL", "OPCODE", "OPERAND", "OBJECT CODE");
	printf("------------------------------------------------------------\n");
	for (int i = 0; i < input_counter; i++) {
		printf("%-8s%-10s%-10s%-10s%-10s\n", sic[i].line, Location[i].loc,
			sic[i].label, sic[i].opcode, sic[i].operand);
	}
	printf("\t\t[ 2PASS 시작 ]\n");
	Object* code_list = pass_2(symtab, program_length);
	// 2PASS 끝 //



	// Print

	printf("\n\n\t\t[ 1PASS - 2PASS 종료 ]\n\n");
	printf("%-8s%-10s%-10s%-10s%-10s%-15s\n", "LINE", "LOCATION", "LABEL", "OPCODE", "OPERAND", "OBJECT CODE");
	printf("------------------------------------------------------------\n");
	for (int i = 0; i < input_counter; i++) {
		printf("%-8s%-10s%-10s%-10s%-10s%-15s\n", sic[i].line, Location[i].loc,
			sic[i].label, sic[i].opcode, sic[i].operand, code_list[i].code);
	}

	return 0;
}


Object* pass_2(Symbol* symtab, int program_length) {
	FILE* fileIn;
	FILE* fileOut;
	int i, zero, check_op, check_sym, buffer;
	int text_counter = 0, text_length = 0, text_start = 0;
	static Object code_list[MAX_INPUT_LINE];
	static State intermediate[MAX_INPUT_LINE];
	char* classify;
	char* point;
	char* extract;
	char max_text[100];
	char get_line[MAX_CHAR_LINE];
	char convert[MAX_CHAR_LINE];

	i = 0;
	fileIn = fopen("./intermediate_file.sic", "r");
	fileOut = fopen("./object_program.sic", "w");
	if (fileIn == NULL || fileOut == NULL) {
		printf("\nERROR : Can't open the file!\n");
		exit(1);
	}

	printf("\n\n\t[ Object Program ]\n\n");															// Object Program Header Print
	do {
		fgets(get_line, sizeof(get_line), fileIn);
		strcopy(intermediate[i].line, strtok(get_line, " \t\n"));
		strcopy(intermediate[i].label, strtok(NULL, " \t\n"));
		strcopy(intermediate[i].opcode, strtok(NULL, " \t\n"));
		strcopy(intermediate[i].operand, strtok(NULL, " \t\n"));
		if (*intermediate[i].operand == '\0') {
			strcopy(intermediate[i].operand, intermediate[i].opcode);
			strcopy(intermediate[i].opcode, intermediate[i].label);
			*intermediate[i].label = '\0';
		}
		if (!strcmp(intermediate[i].opcode, "START")) {
			fprintf(fileOut, "%s %s", "H", intermediate[i].label);
			printf("%s %s", "H",intermediate[i].label);										// Object Program Header Print
			for (int space = 0; space < 6 - strlen(intermediate[i].label); space++) {
				fprintf(fileOut, "%s", " ");
				printf(" ");																// Object Program Header Print
			}
			fprintf(fileOut, "%s", " ");
			printf(" ");																	// Object Program Header Print

		}
		i++;
	} while (!strcmp(intermediate[i - 1].opcode, "START"));

	i--;
	convert_Hx(starting_address, convert, 6);
	fprintf(fileOut, "%s ", convert);
	printf("%s ", convert);																	// Object Program Header Print
	convert_Hx(program_length, convert, 6);
	fprintf(fileOut, "%s \n", convert);
	printf("%s \n", convert);																// Object Program Header Print
	// OBJECT PROGRAM HEADER LINE finish

	while (strcmp(intermediate[i].opcode, "END")) {
		for (check_op = 0; check_op < (sizeof(OPTAB) / 2) / sizeof(OPTAB[0].mnemonic); check_op++) {
			if (!strcmp(intermediate[i].opcode, OPTAB[check_op].mnemonic)) {
				strcopy(code_list[i].code, OPTAB[check_op].code);

				for (check_sym = 0; *symtab[check_sym].label != '\0'; check_sym++) {
					if (!strcmp(intermediate[i].operand, symtab[check_sym].label)) {
						strcat(code_list[i].code, symtab[check_sym].location);
						break;
					}
				}
				if (!strcmp(intermediate[i].opcode, "RSUB"))
					strcat(code_list[i].code, "0000");

				text_counter++;
			}
		}
		if (!strcmp(intermediate[i].opcode, "BYTE")) {
			classify = strchr(intermediate[i].operand, 39);
			if (classify != NULL) {
				point = classify - 1;
				if (*point == 'C') {
					point += 2;
					while (*point != 39) {
						convert_Hx((int)* point, convert, 2);
						strcat(code_list[i].code, convert);
						point++;
					}
				}
				else if (*point == 'X') {
					point += 2;
					strcopy(code_list[i].code, strtok(point, "'"));
				}
			}
			else {
				buffer = atoi(intermediate[i].operand);
				convert_Hx(buffer, code_list[i].code, 6);
			}
			text_counter++;
		}
		else if (!strcmp(intermediate[i].opcode, "WORD")) {
			buffer = atoi(intermediate[i].operand);
			convert_Hx(buffer, code_list[i].code, 6);
			text_counter++;
		}
		i++;
		fgets(get_line, sizeof(get_line), fileIn);
		strcopy(intermediate[i].line, strtok(get_line, " \t\n"));
		strcopy(intermediate[i].label, strtok(NULL, " \t\n"));
		strcopy(intermediate[i].opcode, strtok(NULL, " \t\n"));
		strcopy(intermediate[i].operand, strtok(NULL, " \t\n"));
		if (*intermediate[i].operand == '\0') {
			strcopy(intermediate[i].operand, intermediate[i].opcode);
			strcopy(intermediate[i].opcode, intermediate[i].label);
			*intermediate[i].label = '\0';
		}
	}

	// OBJECT PROGRAM TEXT LINE
	for (text_start = 0; *code_list[text_start].code == '\0'; text_start++);

	while (text_counter != 0) {
		if (*code_list[text_start].code == '\0') {
			text_start++;
			continue;
		}
		*max_text = '\0';
		fprintf(fileOut, "%c ", 'T');
		printf("%c ", 'T');																					// Object Program Text Print
		for (zero = 0; zero < 6 - strlen(Location[zero].loc); zero++) {
			fprintf(fileOut, "%c", '0');
			printf("%c", '0');
		}
		fprintf(fileOut, "%s ", Location[text_start].loc);
		printf("%s ", Location[text_start].loc);
		while (*code_list[text_start].code != '\0' && text_length + strlen(code_list[text_start + 1].code) <= 60) {
			strcat(max_text, code_list[text_start].code);
			strcat(max_text, " ");
			text_length += strlen(code_list[text_start].code);
			text_start++;
			text_counter--;
		}
		convert_Hx(text_length / 2, convert, 2);
		fprintf(fileOut, "%s %s\n", convert, max_text);
		printf("%s %s\n", convert, max_text);																// Object Program Text Print
		text_length = 0;
	}

	// OBJECT PROGRAM END LINE
	fprintf(fileOut, "%c ", 'E');
	printf("%c ", 'E');																						// Object Program End Print
	for (check_sym = 0; *symtab[check_sym].label != '\0'; check_sym++) {
		if (strcmp(intermediate[i].operand, symtab[check_sym].label))
			continue;
		for (zero = 0; zero < 6 - strlen(symtab[check_sym].location); zero++) {
			fprintf(fileOut, "%c", '0');																	// Object Program End Print
			printf("%c", '0');
		}
		fprintf(fileOut, "%s", symtab[check_sym].location);
		printf("%s", symtab[check_sym].location);															// Object Program End Print
	}
	fclose(fileIn);
	fclose(fileOut);

	return code_list;
}



Symbol* pass_1(State* sic) {
	static Symbol symtab[MAX_INPUT_LINE] = { 0, };
	int check, overlap, i;
	int row = 0, present = 0, counter = 0;
	char* hexa = (char*)malloc(sizeof(char) * LOCCTR_LENGTH + 1);
	char* classify;
	char* point;
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
		if (*sic[i].label != '\0') {
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
			classify = strchr(sic[i].operand, 39);
			if (classify != NULL) {
				point = classify - 1;
				if (*point == 'C') {
					point += 2;
					while (*point != 39) {
						locctr++;
						point++;
					}
				}
				else if (*point == 'X') {
					int count = 0;
					point += 2;
					while (*point != 39) {
						count++;
						point++;
					}
					locctr += count / 2;
				}
			}
			else {
				locctr += atoi(sic[i].operand);
			}
		}
		else {
			printf("ERROR : Invalid OPCODE (LINE: %s)\n", sic[i].line);
			exit(1);
		}
		i++;
	}
	free(hexa);

	return symtab;
}


// read SIC input file
State* read_file() {
	static State sic[MAX_INPUT_LINE] = { 0, };
	FILE* fileIn;
	int i = 0;
	char get_line[MAX_CHAR_LINE];
	input_counter = 0;

	fileIn = fopen("./sic_input2.sic", "r");
	if (fileIn == NULL) {
		printf("\nERROR : Can't open the file!\n");
		exit(1);
	}
	while (fgets(get_line, sizeof(get_line), fileIn) != NULL) {
		printf("%s", get_line);
		input_counter++;
		strcopy(sic[i].line, strtok(get_line, " \t\n"));
		strcopy(sic[i].label, strtok(NULL, " \t\n"));
		strcopy(sic[i].opcode, strtok(NULL, " \t\n"));
		strcopy(sic[i].operand, strtok(NULL, " \t\n"));

		// (...)주석 라인 넘김 처리
		if (*sic[i].label == '.') {
			//i++;			//intermediate file에서 주석라인은 저장x
			continue;
		}
		if (*sic[i].operand == '\0') {
			strcopy(sic[i].operand, sic[i].opcode);
			strcopy(sic[i].opcode, sic[i].label);
			*sic[i].label = '\0';
		}
		i++;
	}
	fclose(fileIn);

	return sic;
}


void write_file(State* intermediate) {
	FILE* sic;
	sic = fopen("intermediate_file.sic", "w");
	if (sic == NULL) {
		printf("\nERROR : Can't open the file!\n");
		exit(1);
	}
	for (int i = 0; i < MAX_INPUT_LINE; i++)
		fprintf(sic, "%s\t%s\t%s\t%s\n", intermediate[i].line, intermediate[i].label,
			intermediate[i].opcode, intermediate[i].operand);

	fclose(sic);
}
