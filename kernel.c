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

//! read status from keyboard controller
uint8_t keyboard_read_status(){
	return inb(0x64);
}

//! read keyboard encoder buffer
uint8_t kybrd_enc_read_buf () {
	return inb (0x60);
}


// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{

	uint8_t status  = keyboard_read_status();

	//if you don't read from the buffer you won't get any more interrupts!
	uint8_t code = kybrd_enc_read_buf ();


   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1


void register_interrupt_handler(uint8_t n, isr_t handler){
  interrupt_handlers[n] = handler;
}

void irq_unmask(uint8_t irq)
{

  descriptor_writestring(irq);
  debug("irq a:");
  debug_int(irq);


  irq -= 32;
  uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
  debug("irq b:");
  debug_int(irq);
 
  debug("port: " );
  debug_int(port);



  outb(port, inb(port) & ~(1 << irq % 8));

}


static void key_callback(registers_t regs)
{
   terminal_writestring("keypress");
}


#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {

	init_descriptor_tables();

	/* Initialize terminal interface */
	terminal_initialize();
 
	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n\ntest");


	register_interrupt_handler(IRQ1, &key_callback);
	irq_unmask(IRQ1);


	asm volatile ("sti");

	for(;;){
		asm volatile ("hlt");
	}
}




