A basic keyboard driver

To learn some low level programming, I embarked on the creation of this keyboard driver demo. It runs on Qemu, and was built using a cross-compiler on macOS Sierra.

This uses C and ATT (GAS) Assembly. Some knowledge of both is recommended.

Assembly resources:
Chapter 3 of Computer Systems, a Programmer's Perspective has a great tutorial on assembly. [PDF Download](http://www.nilei.net/cs.pdf)
[Compiler Explorer](https://godbolt.org/)
A quick primer on [Learning to Read Assembly](http://patshaughnessy.net/2016/11/26/learning-to-read-x86-assembly-language)

Setup:
Download [Qemu](http://qemu.org) This is for running the software - [brew](http://brew.sh) install qemu
Download necessities for building a cross compiler. [Links can be found here.](http://wiki.osdev.org/GCC_Cross-Compiler)
 - binutils
 - GNU GMP
 - GNU MPFR
 - ISL
 - ClooG
 - GCC (I used 6.2.0)

Build BinUtils from [here](http://wiki.osdev.org/GCC_Cross-Compiler)

Pushed the last five downloads into the gcc directory, but stripped off the version after the move
so, cloog.x.y.z -> gcc-6.2.0/cloog

Follow Build GCC from [here](http://wiki.osdev.org/GCC_Cross-Compiler)
I put the resulting files here: /usr/local/cross/
Add this to your .bash_profile: export PATH="/usr/local/cross/bin:$PATH"

Get Bare Bones working
Now this: [http://wiki.osdev.org/Bare_Bones]
Skip Grub
32bit is fine. 64bit is unnecessary

You should be able to run this:
`i686-elf-as gdt.s -o output/gdt.o;
i686-elf-as boot.s -o output/boot.o;
i686-elf-gcc -c kernel.c  -o output/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra;
qemu-system-i386  -kernel output/myos.bin`

Get GDT working
(http://raw.knusbaum.com/jamesmolloy/tutorial_html/4.-The%20GDT%20and%20IDT.html)
(http://wiki.osdev.org/GDT_Tutorial)

Interupt requests
(http://raw.knusbaum.com/jamesmolloy/tutorial_html/5.-IRQs%20and%20the%20PIT.html)
Exclude PIT

Keyboard 
(http://www.brokenthorn.com/Resources/OSDev19.html)
(http://archive.is/HogJS)[Cached of the above]

Text mode cursor
http://wiki.osdev.org/Text_Mode_Cursor