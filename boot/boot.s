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
section .text ; code section

; set Multiboot headers at the beginning of text section

dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start]           ; declare kernel code entry to linker 
[GLOBAL glb_mboot_ptr]   ; external struct multiboot * variable
[EXTERN kern_entry]      ; declare kernel c code entrypoint

start:
    cli                  ; close interrupt

    mov esp, STACK_TOP   ; set stack top addr
    mov ebp, 0           ; set stack bottom to 0
    and esp, 0FFFFFFF0H  ; set stack top algin with 16-Byte grain (FFFF FFF0)
    
    mov [glb_mboot_ptr], ebx ; mov pointer in ebx to global variable
    call kern_entry          ; call kernel entrypoint

stop:
    hlt    ; halt
    jmp stop

;-----------------------------------------------------------------------------

section .bss ; uninitialized data section 
stack:
    resb 32768    ; kernel stack, resb = reserve byte
glb_mboot_ptr:
    resb 4

STACK_TOP equ $-stack-1 ; $ means current addr