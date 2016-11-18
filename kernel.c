//from http://wiki.osdev.org/Bare_Bones
//also from http://raw.knusbaum.com/jamesmolloy/tutorial_html/4.-The%20GDT%20and%20IDT.html

/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> //built into the compiler /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "io.h"
#include "debug.h"
#include "descriptor_tables.h"
#include "terminal.h"
#include "utils.h"
#include "isr.h"


// Lets us access our ASM functions from our C code.
extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

// Internal function prototypes.
static void init_idt();
static void idt_set_gate(uint8_t,uint32_t,uint16_t,uint8_t);
static void init_gdt();
static void gdt_set_gate(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;	
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables()
{
   // Initialise the global descriptor table.
   init_gdt();
   init_idt();
}

static void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5);// - 1;
   gdt_ptr.base  = (uint32_t)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush((uint32_t)&gdt_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
}

static void init_idt()
{
	idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
	idt_ptr.base  = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

	idt_set_gate(0, (uint32_t)isr0 , 0x08, 0x8E);
	idt_set_gate(1, (uint32_t)isr1 , 0x08, 0x8E);
	idt_set_gate(2, (uint32_t)isr2 , 0x08, 0x8E);
	idt_set_gate(3, (uint32_t)isr3 , 0x08, 0x8E);
	idt_set_gate(4, (uint32_t)isr4 , 0x08, 0x8E);
	idt_set_gate(5, (uint32_t)isr5 , 0x08, 0x8E);
	idt_set_gate(6, (uint32_t)isr6 , 0x08, 0x8E);
	idt_set_gate(7, (uint32_t)isr7 , 0x08, 0x8E);
	idt_set_gate(8, (uint32_t)isr8 , 0x08, 0x8E);
	idt_set_gate(9, (uint32_t)isr9 , 0x08, 0x8E);
	idt_set_gate(10, (uint32_t)isr10 , 0x08, 0x8E);
	idt_set_gate(11, (uint32_t)isr11 , 0x08, 0x8E);
	idt_set_gate(12, (uint32_t)isr12 , 0x08, 0x8E);
	idt_set_gate(13, (uint32_t)isr13 , 0x08, 0x8E);
	idt_set_gate(14, (uint32_t)isr14 , 0x08, 0x8E);
	idt_set_gate(15, (uint32_t)isr15 , 0x08, 0x8E);
	idt_set_gate(16, (uint32_t)isr16 , 0x08, 0x8E);
	idt_set_gate(17, (uint32_t)isr17 , 0x08, 0x8E);
	idt_set_gate(18, (uint32_t)isr18 , 0x08, 0x8E);
	idt_set_gate(19, (uint32_t)isr19 , 0x08, 0x8E);
	idt_set_gate(20, (uint32_t)isr20 , 0x08, 0x8E);
	idt_set_gate(21, (uint32_t)isr21 , 0x08, 0x8E);
	idt_set_gate(22, (uint32_t)isr22 , 0x08, 0x8E);
	idt_set_gate(23, (uint32_t)isr23 , 0x08, 0x8E);
	idt_set_gate(24, (uint32_t)isr24 , 0x08, 0x8E);
	idt_set_gate(25, (uint32_t)isr25 , 0x08, 0x8E);
	idt_set_gate(26, (uint32_t)isr26 , 0x08, 0x8E);
	idt_set_gate(27, (uint32_t)isr27 , 0x08, 0x8E);
	idt_set_gate(28, (uint32_t)isr28 , 0x08, 0x8E);
	idt_set_gate(29, (uint32_t)isr29 , 0x08, 0x8E);
	idt_set_gate(30, (uint32_t)isr30 , 0x08, 0x8E);
	idt_set_gate(31, (uint32_t)isr31 , 0x08, 0x8E);


	// Remap the irq table.
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
	idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
	idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
	idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
	idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
	idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
	idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
	idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
	idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
	idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
	idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
	idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
	idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
	idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
	idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
	idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
	idt_flush((uint32_t)&idt_ptr);
}

void isr_handler(registers_t regs)
{
   //terminal_writestring("received interrupt: ");
   //monitor_write_dec(regs.int_no);
   //monitor_put('\n');
}

isr_t interrupt_handlers[256];

enum KEYBOARD_ENCODER_IO {
	INPUT_ENCODE_BUF	=	0x60,
	KYBRD_ENC_CMD_REG	=	0x60
};

enum KEYCODE {

// Alphanumeric keys ////////////////

	KEY_SPACE             = ' ',
	KEY_0                 = '0',
	KEY_1                 = '1',
	KEY_2                 = '2',
	KEY_3                 = '3',
	KEY_4                 = '4',
	KEY_5                 = '5',
	KEY_6                 = '6',
	KEY_7                 = '7',
	KEY_8                 = '8',
	KEY_9                 = '9',

	KEY_A                 = 'a',
	KEY_B                 = 'b',
	KEY_C                 = 'c',
	KEY_D                 = 'd',
	KEY_E                 = 'e',
	KEY_F                 = 'f',
	KEY_G                 = 'g',
	KEY_H                 = 'h',
	KEY_I                 = 'i',
	KEY_J                 = 'j',
	KEY_K                 = 'k',
	KEY_L                 = 'l',
	KEY_M                 = 'm',
	KEY_N                 = 'n',
	KEY_O                 = 'o',
	KEY_P                 = 'p',
	KEY_Q                 = 'q',
	KEY_R                 = 'r',
	KEY_S                 = 's',
	KEY_T                 = 't',
	KEY_U                 = 'u',
	KEY_V                 = 'v',
	KEY_W                 = 'w',
	KEY_X                 = 'x',
	KEY_Y                 = 'y',
	KEY_Z                 = 'z',

	KEY_RETURN            = '\r',
	KEY_ESCAPE            = 0x1001,
	KEY_BACKSPACE         = '\b',

// Arrow keys ////////////////////////

	KEY_UP                = 0x1100,
	KEY_DOWN              = 0x1101,
	KEY_LEFT              = 0x1102,
	KEY_RIGHT             = 0x1103,

// Function keys /////////////////////

	KEY_F1                = 0x1201,
	KEY_F2                = 0x1202,
	KEY_F3                = 0x1203,
	KEY_F4                = 0x1204,
	KEY_F5                = 0x1205,
	KEY_F6                = 0x1206,
	KEY_F7                = 0x1207,
	KEY_F8                = 0x1208,
	KEY_F9                = 0x1209,
	KEY_F10               = 0x120a,
	KEY_F11               = 0x120b,
	KEY_F12               = 0x120b,
	KEY_F13               = 0x120c,
	KEY_F14               = 0x120d,
	KEY_F15               = 0x120e,

	KEY_DOT               = '.',
	KEY_COMMA             = ',',
	KEY_COLON             = ':',
	KEY_SEMICOLON         = ';',
	KEY_SLASH             = '/',
	KEY_BACKSLASH         = '\\',
	KEY_PLUS              = '+',
	KEY_MINUS             = '-',
	KEY_ASTERISK          = '*',
	KEY_EXCLAMATION       = '!',
	KEY_QUESTION          = '?',
	KEY_QUOTEDOUBLE       = '\"',
	KEY_QUOTE             = '\'',
	KEY_EQUAL             = '=',
	KEY_HASH              = '#',
	KEY_PERCENT           = '%',
	KEY_AMPERSAND         = '&',
	KEY_UNDERSCORE        = '_',
	KEY_LEFTPARENTHESIS   = '(',
	KEY_RIGHTPARENTHESIS  = ')',
	KEY_LEFTBRACKET       = '[',
	KEY_RIGHTBRACKET      = ']',
	KEY_LEFTCURL          = '{',
	KEY_RIGHTCURL         = '}',
	KEY_DOLLAR            = '$',
	KEY_LESS              = '<',
	KEY_GREATER           = '>',
	KEY_BAR               = '|',
	KEY_GRAVE             = '`',
	KEY_TILDE             = '~',
	KEY_AT                = '@',
	KEY_CARRET            = '^',

// Numeric keypad //////////////////////

	KEY_KP_0              = '0',
	KEY_KP_1              = '1',
	KEY_KP_2              = '2',
	KEY_KP_3              = '3',
	KEY_KP_4              = '4',
	KEY_KP_5              = '5',
	KEY_KP_6              = '6',
	KEY_KP_7              = '7',
	KEY_KP_8              = '8',
	KEY_KP_9              = '9',
	KEY_KP_PLUS           = '+',
	KEY_KP_MINUS          = '-',
	KEY_KP_DECIMAL        = '.',
	KEY_KP_DIVIDE         = '/',
	KEY_KP_ASTERISK       = '*',
	KEY_KP_NUMLOCK        = 0x300f,
	KEY_KP_ENTER          = 0x3010,

	KEY_TAB               = 0x4000,
	KEY_CAPSLOCK          = 0x4001,

// Modify keys ////////////////////////////

	KEY_LSHIFT            = 0x4002,
	KEY_LCTRL             = 0x4003,
	KEY_LALT              = 0x4004,
	KEY_LWIN              = 0x4005,
	KEY_RSHIFT            = 0x4006,
	KEY_RCTRL             = 0x4007,
	KEY_RALT              = 0x4008,
	KEY_RWIN              = 0x4009,

	KEY_INSERT            = 0x400a,
	KEY_DELETE            = 0x400b,
	KEY_HOME              = 0x400c,
	KEY_END               = 0x400d,
	KEY_PAGEUP            = 0x400e,
	KEY_PAGEDOWN          = 0x400f,
	KEY_SCROLLLOCK        = 0x4010,
	KEY_PAUSE             = 0x4011,

	KEY_UNKNOWN,
	KEY_NUMKEYCODES
};
//! original xt scan code set. Array index==make code
//! change what keys the scan code corrospond to if your scan code set is different
static uint32_t _kkybrd_scancode_std [] = {
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
	KEY_RETURN,		//0x1c
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
uint8_t keyboard_enc_read_buf () {
	return inb (INPUT_ENCODE_BUF);
}

bool isShift(uint8_t code){
	uint32_t key = _kkybrd_scancode_std [code];
	return key ==  KEY_LSHIFT || key == KEY_RSHIFT;
}

//bool shiftIsPressed = false;
uint8_t shiftIsPressed = 0;
const uint8_t RIGHT_SHIFT_MASK = 1;
const uint8_t LEFT_SHIFT_MASK = 2;


bool shiftPressed(){
	return shiftIsPressed > 0;
}

char to_upper(char ch1) {
	if(ch1 >= 'a' && ch1 <= 'z'){
		ch1 += 'A' - 'a';
	}
    return ch1;
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
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

		uint32_t key = _kkybrd_scancode_std [code];
		switch(key){
			case KEY_RSHIFT:
				shiftIsPressed |= RIGHT_SHIFT_MASK;
				break;
			case KEY_LSHIFT:
				shiftIsPressed |= LEFT_SHIFT_MASK;
				break;
			default:
				if(shiftPressed()){
					key = to_upper(key);
				}

				terminal_putchar((char)key);
				break;
		}


	}
}

#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1


void register_interrupt_handler(uint8_t n, isr_t handler){
  interrupt_handlers[n] = handler;
}

void irq_unmask(uint8_t irq)
{
  irq -= 32;
  uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
  outb(port, inb(port) & ~(1 << irq % 8));
}


static void key_callback(registers_t regs)
{
   //terminal_writestring("keypress");
}

void init_keyboard(){
	register_interrupt_handler(IRQ1, &key_callback);
	irq_unmask(IRQ1);
	asm volatile ("sti");
}


#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {

	init_descriptor_tables();

	/* Initialize terminal interface */
	terminal_initialize();
 
	terminal_writestring("Hello, kernel World!\n\ntest");

	init_keyboard();

	for(;;){
		asm volatile ("hlt");
	}
}




