#pragma once
#include <string.h>
#include <stdlib.h>

// 10������ 16������ �ٲ��ִ� �Լ�
// '0'�� ä���� size ���̸�ŭ ��ȯ
char* convert_Hx(int counter) {
	int size = 4; // ��ȯ�� 16������ �ڸ���
	char convert[10] = { 0, };
	int hexa, digit = 0;
	char temp;
	char* locctr;
	locctr = (char*)malloc(sizeof(char)*size);

	while (digit < size) {
		hexa = counter % 16;
		if (hexa < 10)
			convert[digit] = hexa + 48;
		else
			convert[digit] = 65 + (hexa - 10);

		counter /= 16;
		digit++;
	}
	// �������� ����
	for (int i = 0, j = size - 1; i <= j; i++, j--) {
		temp = convert[j];
		convert[j] = convert[i];
		convert[i] = temp;
	}
	strcpy(locctr, convert);
	return locctr;
}