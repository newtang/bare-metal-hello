//from http://wiki.osdev.org/Bare_Bones
//also from http://raw.knusbaum.com/jamesmolloy/tutorial_html/4.-The%20GDT%20and%20IDT.html

/* Checks to make sure we are not in a hosted environment */
#if !defined(__STDC_HOSTED__) //check for a hosted var instead of __linux__
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include <stdbool.h> //built into the compiler /* C doesn't have booleans by default. */
#include <stddef.h>
#include <stdint.h>
#include "interrupts.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "notepad.h"

void kernel_main(void) {
	init_descriptor_tables();
	init_keyboard();
	notepad_init();
}




