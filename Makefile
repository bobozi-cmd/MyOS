#!Makefile

C_SOURCES = $(shell find . -name "*.c")
# patsubst is a function in Makefile, used to replace string by pattern matching
# Usage: $(patsubst pattern, replacement, text)
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

# -c: only compile source .c file
# -Wall: open all Warning
# -m32: generate 32-bit object .o file
# -ggdb: generate debug-table info
# -gstabs+: generate complete debug info
# -nostdinc: not search header files in standard path
# -fno-pic: disable Position Independent Code
# -fno-builtin: disable Compiler builtin functions
# -fno-stack-protector: disk stack protector
C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-pic -fno-builtin -fno-stack-protector -I include

# -T scripts/kernel.ld: select target .ld file to link, customizing the layout of execute
# -m elf_i386: the target format for the generated executable file is ELF_i386 format
# -nostdlib: don't link with standard libraries
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib

# -f elf: target file format is elf
# -g: generate debug-table info
# -F stabs: generate STABS(Symbol Table and Debugging System) format debug info 
ASM_FLAGS = -f elf -g -F stabs

all: $(C_OBJECTS) $(S_OBJECTS) link update_image

# The automatic variable `$<` is just the first prerequisite file name in rule
# The automatic variable `$@` is just the target file name in rule
# {target file}:{prequisite files} -> Explicit Rule, each file must be explicit name
# {prequisite files}{target file}: -> Pattern Rule, each file can use pattern matching
.c.o:
	@echo "Compile code file: $< ..."
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo "Assemble asm file $< ..."
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo "Link kernel file ..."
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o hx_kernel

# .PHONY is Phony Target, which defines a command
.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) hx_kernel

# upate image file's kernel
.PHONY:update_image
update_image:
	sudo mount floppy.img /mnt/kernel
	sudo cp hx_kernel /mnt/kernel/hx_kernel
	sleep 1
	sudo umount /mnt/kernel

.PHONY:mount_image
mount_image:
	sudo mount floppy.img /mnt/kernel

.PHONY:qemu
qemu:
	qemu-system-i386 -fda floppy.img -boot a

.PHONY:debug
# -S: Not run auto, wait for gdb commands
# -s: open port 1234 for gdb connectting
debug:
	qemu-system-i386 -S -s -fda floppy.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit