global loader             ; The entry symbol for ELF
extern main               ; Declare the C function to call

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .bss
    align 16
stack_bottom:
    resb 16384            ; 16 KB stack space
stack_top:

section .text
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov esp, stack_top    ; Initialize stack pointer
    call main             ; Call the C `main` function
.loop:
    jmp .loop             ; Infinite loop
