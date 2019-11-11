#pragma once
#define MAX_CHAR_LINE 20

typedef struct _Symbol_Table {
	char label[MAX_CHAR_LINE];
	char location[MAX_CHAR_LINE];
	int error_flag;
	char data_type[MAX_CHAR_LINE];
} Symbol;