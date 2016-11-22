//from http://wiki.osdev.org/Bare_Bones
//also from http://raw.knusbaum.com/jamesmolloy/tutorial_html/4.-The%20GDT%20and%20IDT.html

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> //built into the compiler /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "interrupts.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "notepad.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {
	init_descriptor_tables();
	init_keyboard();
	notepad_init();

	for(;;){
		asm volatile ("hlt");
	}
}




