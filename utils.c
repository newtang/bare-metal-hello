#include "utils.h"

void *memset(void *dst, int c, size_t n){
	if (n) {
    	char *d = dst;

	    do {
	    	*d++ = c;
	    } while (--n);
 	}
 	return dst;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}