#pragma once
#include <math.h>

void convert_Hx(int decNum, char* location, int length);
int convert_dec(char* hexa);
char* strcopy(char* dest, char* src);

// 10������ 16������ �ٲ��ִ� �Լ�
// '0'�� ä���� size ���̸�ŭ ��ȯ
void convert_Hx(int decNum, char* location, int length) {
	char convert[MAX_CHAR_LINE] = { 0, };
	int hexa, digit = 0;
	char temp;

	while (digit < length) {
		hexa = decNum % 16;
		if (hexa < 10)
			convert[digit] = hexa + 48;
		else
			convert[digit] = 65 + (hexa - 10);

		decNum /= 16;
		digit++;
	}
	// �������� ����
	for (int i = 0, j = length - 1; i <= j-1; i++, j--) {
		temp = convert[j];
		convert[j] = convert[i];
		convert[i] = temp;
	}
	strcpy(location, convert);
}

// 16������ 10������ ��ȯ
int convert_dec(char* hexa) {
	char* ptr = NULL;

	return strtol(hexa, &ptr, 16);
}


// NULL ���� �����ϴ� strcopy()
// <string.h>�� strcpy�� �� ��� src�� NULL���� ������ ���
// ������ �߻���Ű�� ������ NULL���� �����ϴ� ����� �߰���.
char *strcopy(char* dest, char* src) {
	int i;
	if (src == NULL) {
		dest[0] = 0;
		return 0;
	}
	for (i = 0; src[i] != '\0'; i++) {
		dest[i] = src[i];
	}
	dest[i] = 0;
	return 0;
}

