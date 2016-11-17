#ifndef UTILS_H_   /* Include guard */
#define UTILS_H_

#include <stdint.h>
#include <stddef.h>

void *memset(void *, int, size_t);
void outb(uint16_t, uint8_t);
uint8_t inb(uint16_t);

#endif