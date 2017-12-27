#!/bin/sh
/usr/bin/nasm -f elf ../my.asm
/usr/bin/gcc -m32 ../my.o -o asm_exec
./asm_exec
