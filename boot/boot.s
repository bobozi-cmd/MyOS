; Multiboot Magic Number
MBOOT_HEADER_MAGIC equ 0x1BADB002

; flags[0] means align with page
MBOOT_PAGE_ALIGN equ 1 << 0

; flags[1] means mem_* effective
; (make GRUB save memory info into Multiboot struct)
MBOOT_MEM_INFO equ 1 << 1

; flags
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; checksum + magic + flags == (u32) 0
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS)

;-----------------------------------------------------------------------------

[BITS 32] ; compile as 32-bit 
section .init.text ; code section

; set Multiboot headers at the beginning of text section

dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start]           ; declare kernel code entry to linker 
[GLOBAL mboot_ptr_tmp]   ; external struct multiboot * variable
[EXTERN kern_entry]      ; declare kernel c code entrypoint

start:
    cli                  ; close interrupt
    
    mov [mboot_ptr_tmp], ebx ; mov pointer in ebx to global variable
    mov esp, STACK_TOP
    and esp, 0FFFFFFF0H
    mov ebp, 0

    call kern_entry          ; call kernel entrypoint

;-----------------------------------------------------------------------------

section .init.data ; tempory data section before paging 
stack: times 1024 db 0    ; tempory kernel stack
STACK_TOP equ $-stack-1

mboot_ptr_tmp: dd 0