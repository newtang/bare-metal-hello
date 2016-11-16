#include "utils.h"

void *memset(void *dst, int c, size_t n){
	if (n) {
    	char *d = dst;

	    do {
	    	*d++ = c;
	    } while (--n);
 	}
 	return dst;
 }

void outb(unsigned char value, unsigned short int port)
{
  asm volatile ("outb %b0,%w1": :"a" (value), "Nd" (port));
}

unsigned char inb (unsigned short int port)
{
  unsigned char _v;
  asm volatile ("inb %w1,%0":"=a" (_v):"Nd" (port));
  return _v;
}
