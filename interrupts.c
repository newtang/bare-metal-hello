#include <stdint.h>
#include "interrupts.h"
#include "io.h"
#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1

static isr_t interruptHandlers[256];

static void irqUnmask(uint8_t irq){
	irq -= 32;
	uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
	outb(port, inb(port) & ~(1 << irq % 8));
}

// This gets called from our ASM interrupt handler stub.
void irqHandler(registers_t regs){
	if (interruptHandlers[regs.int_no] != 0){
    	isr_t handler = interruptHandlers[regs.int_no];
    	handler(regs);
	}
}

void registerInterruptHandler(uint8_t n, isr_t handler){
	irqUnmask(n);
	interruptHandlers[n] = handler;
}

void endInterrupt (registers_t regs){
	outb(0x20, 0x20);
	if (regs.int_no >= 40){
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
} 
