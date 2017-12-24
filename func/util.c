#include "../includes/util.h"

#include <ctype.h>

int hasprint(char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (isprint(str[i]) > 0) {
			return 1;
		}
	}
	return 0;
}

