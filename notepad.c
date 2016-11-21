#include <stdint.h>
#include <stdbool.h> 
#include "notepad.h"
#include "terminal.h"
#include "keyboard.h"

static bool charsDown[256];
static const uint8_t MAX_KEYS_DOWN = 6;
static bool relaxMsg = false;
static uint8_t totalDown = 0;

static void onPrintableKeyDown(char c){
	if(totalDown < MAX_KEYS_DOWN){
		terminal_putchar(c);
	}
}

static void onKeyDown(char code){

	//we get this event when a key is held down, so this prevents us from overcounting keys that are held down.
	if(!charsDown[(uint32_t) code]){
		charsDown[(uint32_t) code] = true;
		++totalDown;
		//debug("onKeyDown: ");
		//debug_int(totalDown);
	}

	if(totalDown >= MAX_KEYS_DOWN){
		if(!relaxMsg){
			terminal_writestring(" Relax. It's going to be ok. ");
			//debug("print relax: ");
			//debug_int(totalDown);
			relaxMsg = true;
		}

	}
	else{
		relaxMsg = false;
		switch (code){
			case KEY_RETURN:
				terminal_newline();
				break;
		}
	}
}

static void onKeyUp(char code){
	charsDown[(uint32_t) code] = false;
	--totalDown;
//	debug("onKeyUp: ");
//	debug_int(totalDown);
}


void notepad_init(){
	terminal_initialize(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED);
	terminal_writestring("You should type stuff> ");
	registerKeyboardCallback(PRINTABLE_KEY_DOWN_EVENT, onPrintableKeyDown);
	registerKeyboardCallback(KEY_DOWN_EVENT, onKeyDown);
	registerKeyboardCallback(KEY_UP_EVENT, onKeyUp);
}