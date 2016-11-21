rm output/*
i686-elf-as gdt.s -o output/gdt.o;
i686-elf-as boot.s -o output/boot.o;
i686-elf-gcc -c kernel.c  -o output/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c interrupts.c  -o output/interrupts.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c keyboard.c  -o output/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c descriptor_tables.c  -o output/descriptor_tables.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c io.c  -o output/io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c debug.c  -o output/debug.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c utils.c  -o output/utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c terminal.c  -o output/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -c notepad.c  -o output/notepad.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -T linker.ld -o output/myos.bin -ffreestanding -O2 -nostdlib output/notepad.o output/keyboard.o output/interrupts.o output/descriptor_tables.o output/io.o output/debug.o output/terminal.o output/utils.o output/boot.o output/gdt.o output/kernel.o -lgcc;
qemu-system-i386 -serial file:serial.log  -kernel output/myos.bin; #-serial file:serial.log