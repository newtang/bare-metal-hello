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
