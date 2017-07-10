#include "../includes/func.h"

#include <stdio.h>

void LOG(char* str) {
	printf("\033[33m");		// 文字色を黄色にする
	printf("LOG %s\n",str);
	printf("\033[93m");		// 文字色を標準にする
}
