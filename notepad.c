#include <stdint.h>
#include "notepad.h"
#include "terminal.h"
#include "keyboard.h"

static void onPrintableKeyDown(char c){
	terminal_putchar(c);
}

/*
static void onKeyDown(int8_t code){

}

static void onKeyUp(int8_t code){

	switch (code){
		case KEY_RETURN:
			terminal_newline();
			break;
	}
}
*/

void notepad_init(){
	terminal_initialize(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED);
	terminal_writestring("You should type stuff> ");
	registerKeyboardCallback(PRINTABLE_KEY_DOWN_EVENT, onPrintableKeyDown);
	registerKeyboardCallback(KEY_DOWN_EVENT, onKeyDown);
	registerKeyboardCallback(KEY_UP_EVENT, onKeyUp);
}