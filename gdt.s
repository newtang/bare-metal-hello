.global gdt_flush
gdt_flush:
   # Get the pointer to the GDT, passed as a parameter.
   #mov %eax [esp+4]  
   mov 4(%esp), %eax

   # Load the new GDT pointer
   lgdt (%eax)        

   # 0x10 is the offset in the GDT to our data segment
   mov $0x10, %ax

   # Load all data segment selectors   
   mov %ax, %ds     
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   mov %ax, %ss

   # 0x08 is the offset to our code segment: Far jump!
   ljmp $0x08, $.flush   
.flush:
   ret


.global idt_flush
idt_flush:
   mov 4(%esp), %eax  # Get the pointer to the IDT, passed as a parameter. 
   lidt (%eax)        # Load the IDT pointer.
   ret

#.extern isr_handler

isr_common_stub:
   pusha                    #Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov $0x10, %ax           #load the kernel data segment descriptor
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs

   call isr_handler

   popa                     #Pops edi,esi,ebp...
   add $8, %esp     #Cleans up the pushed error code and pushed ISR number
   iret           #pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


#In isr.c
#[EXTERN irq_handler]

#This is our common IRQ stub. It saves the processor state, sets
#up for kernel mode segments, calls the C-level fault handler,
#and finally restores the stack frame. 
irq_common_stub:
   pusha                   #Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov $0x10, %ax          #load the kernel data segment descriptor
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs

   call irq_handler

   popa                    #Pops edi,esi,ebp...
   add $8, %esp             #Cleans up the pushed error code and pushed ISR number
   sti
   iret                    #pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


#.global isr0
#isr0:
#  cli            # Disable interrupts
#  push 0         # Push a dummy error code (if ISR0 doesn't push it's own error code)
#  push 0         # Push the interrupt number (0)
#  jmp isr_common_stub # Go to our common handler.

.macro ISR_NOERRCODE num  # define a macro, taking one parameter
.global isr\num      # \num accesses the first parameter.
  isr\num:
    cli
    push 0
    push \num
    jmp isr_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_NOERRCODE 8
ISR_NOERRCODE 9
ISR_NOERRCODE 10
ISR_NOERRCODE 11
ISR_NOERRCODE 12
ISR_NOERRCODE 13
ISR_NOERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 32

.macro ISR_ERRCODE num
.isr\num 
  .isr\num :
    cli
    push \num 
    jmp isr_common_stub
.endm





#This macro creates a stub for an IRQ - the first parameter is
#the IRQ number, the second is the ISR number it is remapped to.
.macro IRQ num remap
  .global irq\num
  irq\num:
    cli
    push $0
    push $\remap
    jmp irq_common_stub
.endm

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ   10,    42
IRQ   11,    43
IRQ   12,    44
IRQ   13,    45
IRQ   14,    46
IRQ   15,    47

