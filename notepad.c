#include <stdint.h>
#include <stdbool.h> 
#include "notepad.h"
#include "terminal.h"
#include "keyboard.h"

//charsDown is for keeping track of the keys that are pressed down in order to show a mash message Not the most efficient way, but
//I didn't want to include a hashtable
static bool charsDown[256];
static const uint8_t MAX_KEYS_DOWN = 6;
static bool relaxMsg = false;
static uint8_t totalDown = 0;

static void onPrintableKeyDown(char c){
	if(totalDown < MAX_KEYS_DOWN){
		terminalPutChar(c);
	}
}

static void onKeyDown(char code){

	//we get this event when a key is held down, so this prevents us from overcounting keys that are held down.
	if(!charsDown[(uint32_t) code]){
		charsDown[(uint32_t) code] = true;
		++totalDown;
	}

	if(totalDown >= MAX_KEYS_DOWN){
		if(!relaxMsg){
			terminalWriteString(" Relax. It's going to be ok. ");
			relaxMsg = true;
		}

	}
	else{
		relaxMsg = false;
		switch (code){
			case KEY_RETURN:
				terminalNewline();
				break;
			case KEY_BACKSPACE:
				terminalBackspace();
				break;
		}
	}
}

static void onKeyUp(char code){
	charsDown[(uint32_t) code] = false;
	//preventing an overflow. We could get an onKeyUp if it didn't have focus.
	if(totalDown > 0){
		--totalDown;
	}
}


void notepad_init(){
	terminalInitialize(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED);
	terminalWriteString("You should type stuff> ");
	registerKeyboardCallback(PRINTABLE_KEY_DOWN_EVENT, onPrintableKeyDown);
	registerKeyboardCallback(KEY_DOWN_EVENT, onKeyDown);
	registerKeyboardCallback(KEY_UP_EVENT, onKeyUp);
}