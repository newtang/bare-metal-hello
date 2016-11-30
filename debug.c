#include <stdint.h>
#include "utils.h"
#include "debug.h"
#include "asm.h"

void debug_int (uint8_t num){
	int maxPos = 99;
	int lastPos = maxPos;
	char chars[maxPos+1];
	for(int i=1; i<=maxPos; ++i){
		char c = ((num % 10) + '0');
		chars[lastPos] = c;
		num /= 10;
		lastPos--;
	}

	lastPos++;

	//skipping starting zeros
	while(chars[lastPos] == '0'){
		lastPos++;
	}

	if(lastPos >= maxPos){
		debug_char('0');	
	}

	while (lastPos <= maxPos){
		debug_char(chars[lastPos]);
		lastPos++;
	}
	debug_char('\n');
}

void debug_char(char c){
	outb(0x3f8, c);
}

void debug_string(const char* data) {
	size_t size = strlen(data);
	for (size_t i = 0; i < size; i++)
		debug_char(data[i]);
}
 
void debug(const char* data) {
	debug_string(data);
	debug_char('\n');
}
