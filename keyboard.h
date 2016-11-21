#ifndef _KEYBOARD_H_   /* Include guard */
#define _KEYBOARD_H_

#include <stdint.h>

void init_keyboard();


enum KEYBOARD_EVENT_TYPE {
	PRINTABLE_KEY_DOWN_EVENT,
	KEY_DOWN_EVENT,
	KEY_UP_EVENT
};

void registerKeyboardCallback(enum KEYBOARD_EVENT_TYPE type, void callback(char));

#endif