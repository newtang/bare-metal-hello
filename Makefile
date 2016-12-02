program_NAME := driver
CC_AS=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

OUTPUT_OPTION = -o output/$@

program_ASM_SRCS := $(wildcard *.s)
program_C_SRCS := $(wildcard *.c)
program_OBJS := ${program_C_SRCS:.c=.o}
program_OBJS_OUTPUT := $(wildcard output/*.o)

all: compile_asm $(program_NAME)

$(program_NAME): $(program_OBJS_OUTPUT)
	i686-elf-gcc -T linker.ld -o output/myos.bin -ffreestanding -O2 -nostdlib  $^ -lgcc;
	qemu-system-i386 -serial file:serial.log  -kernel output/myos.bin;

compile_asm:
	$(CC_AS) gdt.s -o output/gdt.o;
	$(CC_AS) interrupt_asm.s -o output/interrupt_asm.o;
	$(CC_AS) boot.s -o output/boot.o;
