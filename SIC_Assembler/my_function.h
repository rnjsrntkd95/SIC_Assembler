#pragma once
#include <math.h>

void convert_Hx(int decNum, char* location, int length);
int convert_dec(char* hexa);
char* strcopy(char* dest, char* src);

// 10진수를 16진수로 바꿔주는 함수
// '0'을 채워서 size 길이만큼 반환
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
	// 역순으로 정렬
	for (int i = 0, j = length - 1; i <= j-1; i++, j--) {
		temp = convert[j];
		convert[j] = convert[i];
		convert[i] = temp;
	}
	strcpy(location, convert);
}

// 16진수를 10진수로 변환
int convert_dec(char* hexa) {
	char* ptr = NULL;

	return strtol(hexa, &ptr, 16);
}


// NULL 값도 복사하는 strcopy()
// <string.h>의 strcpy를 쓸 경우 src에 NULL값이 들어왔을 경우
// 에러를 발생시키기 때문에 NULL값을 복사하는 기능을 추가함.
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

