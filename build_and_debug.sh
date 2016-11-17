rm output/*
i686-elf-as gdt.s -o output/gdt.o;
i686-elf-as boot.s -o output/boot.o;
i686-elf-gcc -g -c kernel.c  -o output/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -g -c utils.c  -o output/utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -g -c terminal.c  -o output/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
i686-elf-gcc -g -T linker.ld -o output/myos.bin -ffreestanding -O2 -nostdlib output/terminal.o output/utils.o output/boot.o output/gdt.o output/kernel.o -lgcc;
qemu-system-i386 -s -S -kernel output/myos.bin;