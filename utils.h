#ifndef UTILS_H_   /* Include guard */
#define UTILS_H_

#include <stdint.h>
#include <stddef.h>

void *memset(void *, int, size_t);
void outb(unsigned char value, unsigned short int);
unsigned char inb (unsigned short int);

#endif