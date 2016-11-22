
#include <stdbool.h> //built into the compiler /* C doesn't have booleans by default. *
#include <stddef.h>
#include <stdint.h>
#include "io.h"
#include "terminal.h"
#include "utils.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
static size_t terminalRow;
static size_t terminalColumn;
static uint8_t terminalColor;
static uint16_t* terminalBuffer;


static size_t getIndex(size_t x, size_t y){
	return y * VGA_WIDTH + x;
}

//http://wiki.osdev.org/Text_Mode_Cursor
static void updateCursor(){
 	//the cursor logic inverts our notion of rows and columns
    unsigned short position = getIndex(terminalColumn, terminalRow);

    // cursor LOW port to vga INDEX register
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}

static void terminalPutEntryAt(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = getIndex(x,y);
	terminalBuffer[index] = vga_entry(c, color);
}

void terminalInitialize(enum vga_color fg, enum vga_color bg) {
	terminalRow = 0;
	terminalColumn = 0;
	terminalColor = vga_entry_color(fg, bg);
	terminalBuffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			terminalPutEntryAt(' ', terminalColor, x, y);
		}
	}
}

void terminalNewline(){
	++terminalRow;
	terminalColumn = 0;
	updateCursor();
}


void terminalBackspace(){
	if(terminalColumn != 0){
		--terminalColumn;
		terminalPutEntryAt(' ', terminalColor, terminalColumn, terminalRow );
	}
	else{
		const uint16_t space = vga_entry(' ', terminalColor);

		if(terminalRow !=0){
			--terminalRow;
		}
		else{
			//we're at the beginning.
			return;
		}

		terminalColumn = VGA_WIDTH-1;

		bool charFound = false;
		while(true){
			if (terminalBuffer[getIndex(terminalColumn, terminalRow)] != space) {
				charFound = true;
				break;
			}

			if(terminalColumn == 0){
				break;
			}

			--terminalColumn;
		}

		if(charFound){
			++terminalColumn;
		}
	}

	updateCursor();
}

void terminalPutChar(char c) {

	terminalPutEntryAt(c, terminalColor, terminalColumn, terminalRow);
	if (++terminalColumn == VGA_WIDTH) {
		terminalColumn = 0;
		if (++terminalRow == VGA_HEIGHT)
			terminalRow = 0;
	}

	updateCursor();
}
 
void terminalWrite(const char* data) {
	size_t size = strlen(data);
	for (size_t i = 0; i < size; i++)
		terminalPutChar(data[i]);
}
 
void terminalWriteString(const char* data) {
	terminalWrite(data);
}