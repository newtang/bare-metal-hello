#include <stdbool.h> 
#include <stdint.h>
#include "keyboard.h"
#include "interrupts.h"
#include "io.h"


enum KEYBOARD_ENCODER_IO {
	INPUT_ENCODE_BUF	=	0x60,
	KYBRD_ENC_CMD_REG	=	0x60
};


//! original xt scan code set. Array index==make code
//! change what keys the scan code corrospond to if your scan code set is different
uint32_t _kkybrd_scancode_std [] = {
	//! key			scancode
	KEY_UNKNOWN,	//0
	KEY_ESCAPE,		//1
	KEY_1,			//2
	KEY_2,			//3
	KEY_3,			//4
	KEY_4,			//5
	KEY_5,			//6
	KEY_6,			//7
	KEY_7,			//8
	KEY_8,			//9
	KEY_9,			//0xa
	KEY_0,			//0xb
	KEY_MINUS,		//0xc
	KEY_EQUAL,		//0xd
	KEY_BACKSPACE,	//0xe
	KEY_TAB,		//0xf
	KEY_Q,			//0x10
	KEY_W,			//0x11
	KEY_E,			//0x12
	KEY_R,			//0x13
	KEY_T,			//0x14
	KEY_Y,			//0x15
	KEY_U,			//0x16
	KEY_I,			//0x17
	KEY_O,			//0x18
	KEY_P,			//0x19
	KEY_LEFTBRACKET,//0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		//0x1cKEY_BACKSPACE,	//0xe
	KEY_LCTRL,		//0x1d
	KEY_A,			//0x1e
	KEY_S,			//0x1f
	KEY_D,			//0x20
	KEY_F,			//0x21
	KEY_G,			//0x22
	KEY_H,			//0x23
	KEY_J,			//0x24
	KEY_K,			//0x25
	KEY_L,			//0x26
	KEY_SEMICOLON,	//0x27
	KEY_QUOTE,		//0x28
	KEY_GRAVE,		//0x29
	KEY_LSHIFT,		//0x2a
	KEY_BACKSLASH,	//0x2b
	KEY_Z,			//0x2c
	KEY_X,			//0x2d
	KEY_C,			//0x2e
	KEY_V,			//0x2f
	KEY_B,			//0x30
	KEY_N,			//0x31
	KEY_M,			//0x32
	KEY_COMMA,		//0x33
	KEY_DOT,		//0x34
	KEY_SLASH,		//0x35
	KEY_RSHIFT,		//0x36
	KEY_KP_ASTERISK,//0x37
	KEY_RALT,		//0x38
	KEY_SPACE,		//0x39
	KEY_CAPSLOCK,	//0x3a
	KEY_F1,			//0x3b
	KEY_F2,			//0x3c
	KEY_F3,			//0x3d
	KEY_F4,			//0x3e
	KEY_F5,			//0x3f
	KEY_F6,			//0x40
	KEY_F7,			//0x41
	KEY_F8,			//0x42
	KEY_F9,			//0x43
	KEY_F10,		//0x44
	KEY_KP_NUMLOCK,	//0x45
	KEY_SCROLLLOCK,	//0x46
	KEY_HOME,		//0x47
	KEY_KP_8,		//0x48	//keypad up arrow
	KEY_PAGEUP,		//0x49
	KEY_KP_2,		//0x50	//keypad down arrow
	KEY_KP_3,		//0x51	//keypad page down
	KEY_KP_0,		//0x52	//keypad insert key
	KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KEY_UNKNOWN,	//0x54
	KEY_UNKNOWN,	//0x55
	KEY_UNKNOWN,	//0x56
	KEY_F11,		//0x57
	KEY_F12			//0x58
};


//! read keyboard encoder buffer
static uint8_t keyboard_enc_read_buf () {
	return inb (INPUT_ENCODE_BUF);
}

//bool shiftIsPressed = false;
static uint8_t shiftIsPressed = 0;
static const uint8_t RIGHT_SHIFT_MASK = 1;
static const uint8_t LEFT_SHIFT_MASK = 2;


static bool shiftPressed(){
	return shiftIsPressed > 0;
}

//capitalize letters, convert numbers and symbols to their shift equivalent.
static char convertWithShift(char ch1) {
	if(ch1 >= 'a' && ch1 <= 'z'){
		ch1 += 'A' - 'a';
	}
	else{
		switch(ch1){
			case '1':
				ch1 = '!';
				break;
			case '2':
				ch1 = '@';
				break;
			case '3':
				ch1 = '#';
				break;
			case '4':
				ch1 = '$';
				break;
			case '5':
				ch1 = '%';
				break;
			case '6':
				ch1 = '^';
				break;
			case '7':
				ch1 = '&';
				break;
			case '8':
				ch1 = '*';
				break;
			case '9':
				ch1 = '(';
				break;
			case '0':
				ch1 = ')';
				break;
			case '`':
				ch1 = '~';
				break;
			case '[':
				ch1 = '{';
				break;
			case ']':
				ch1 = '}';
				break;
			case '\\':
				ch1 = '|';
				break;
			case ';':
				ch1 = ':';
				break;
			case '\'':
				ch1 = '"';
				break;
			case ',':
				ch1 = '<';
				break;
			case '.':
				ch1 = '>';
				break;
			case '/':
				ch1 = '?';
				break;
			case '-':
				ch1 = '_';
				break;
			case '=':
				ch1 = '+';
				break;
		}
	}

    return ch1;
}

static uint32_t (*callbacks[4]) (char);

static bool isPrintable(uint8_t code){
	
	uint32_t key = _kkybrd_scancode_std [code];
	if(code >= 0x3a){
		return false;
	}
	else {
		switch(key){
			case KEY_UNKNOWN:
			case KEY_ESCAPE:
			case KEY_RETURN:
			case KEY_BACKSPACE:
			case KEY_GRAVE:
			case KEY_LSHIFT:
			case KEY_RSHIFT:
			case KEY_LCTRL:
			case KEY_RCTRL:
			case KEY_LALT:
			case KEY_RALT:
				return false;
			default:
				return true;
		}
	}
	
	return true;
}

static void key_callback(registers_t regs)
{
   //if you don't read from the buffer you won't get any more interrupts!
	uint8_t code = keyboard_enc_read_buf ();
	//descriptor_writestring(code);


   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }

   //! test if this is a break code (Original XT Scan Code Set specific)
   //break code is when a key is released
	if (code & 0x80) {	//test bit 7
		code -= 0x80;
		uint32_t key = _kkybrd_scancode_std [code];
		callbacks[KEY_UP_EVENT]((char)key);

		switch(key){
			case KEY_RSHIFT:
				shiftIsPressed &=  (shiftIsPressed & ~RIGHT_SHIFT_MASK);
				break;
			case KEY_LSHIFT:
				shiftIsPressed &=  (shiftIsPressed & ~LEFT_SHIFT_MASK);
				break;
		}
	}
	else {
		//a make code is sent when a key is pressed or held down
		uint32_t key = _kkybrd_scancode_std [code];
		callbacks[KEY_DOWN_EVENT]((char)key);
		switch(key){
			case KEY_RSHIFT:
				shiftIsPressed |= RIGHT_SHIFT_MASK;
				break;
			case KEY_LSHIFT:
				shiftIsPressed |= LEFT_SHIFT_MASK;
				break;
			default:
				if(isPrintable(code)){
					if(shiftPressed()){
						key = convertWithShift(key);
					}
					//if check or prepopulate value
					callbacks[PRINTABLE_KEY_DOWN_EVENT]((char)key);
				}
				break;
		}

	}
}

void registerKeyboardCallback(enum KEYBOARD_EVENT_TYPE type, void callback(char)){
	callbacks[(uint32_t)type] = callback;
}

void init_keyboard(){
	asm volatile ("sti");
	register_interrupt_handler(IRQ1, &key_callback);
}