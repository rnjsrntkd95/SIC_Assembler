#pragma once
#include <string.h>
#include <stdlib.h>

// 10진수를 16진수로 바꿔주는 함수
// '0'을 채워서 size 길이만큼 반환
char* convert_Hx(int counter) {
	int size = 4; // 반환될 16진수의 자릿수
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
	// 역순으로 정렬
	for (int i = 0, j = size - 1; i <= j; i++, j--) {
		temp = convert[j];
		convert[j] = convert[i];
		convert[i] = temp;
	}
	strcpy(locctr, convert);
	return locctr;
}