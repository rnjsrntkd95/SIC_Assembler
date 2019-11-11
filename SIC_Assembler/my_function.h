#pragma once
#include <string.h>
#include <stdlib.h>

// 10������ 16������ �ٲ��ִ� �Լ�
// '0'�� ä���� size ���̸�ŭ ��ȯ
void convert_Hx(int counter, char* locctr, int length) {
	char convert[MAX_CHAR_LINE] = { 0, };
	int hexa, digit = 0;
	char temp;

	while (digit < length) {
		hexa = counter % 16;
		if (hexa < 10)
			convert[digit] = hexa + 48;
		else
			convert[digit] = 65 + (hexa - 10);

		counter /= 16;
		digit++;
	}
	printf("%s\n", convert);
	// �������� ����
	for (int i = 0, j = length - 1; i <= j-1; i++, j--) {
		temp = convert[j];
		convert[j] = convert[i];
		convert[i] = temp;
	}
	strcpy(locctr, convert);
}

// NULL ���� �����ϴ� strcpy()
// <string.h>�� strcpy�� �� ��� src�� NULL���� ������ ���
// ������ �߻���Ű�� ������ NULL���� �����ϴ� ����� �߰���.
char *strcpy(char* dest, char* src) {
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