#ifndef TERMINAL_H   /* Include guard */
#define TERMINAL_H

void terminal_putchar(char);
void terminal_initialize(void);
void terminal_writestring(const char*);
void descriptor_writestring (uint8_t);


#endif