BITS 32

GLOBAL idt_load
GLOBAL isr_stub_table
EXTERN isr_handler_c

%macro ISR_NOERR 1
isr_stub_%1:
    push dword 0
    push dword %1
    jmp isr_common
%endmacro

isr_common:
    pusha

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, [esp + 48]
    mov ebx, [esp + 52]
    push ebx
    push eax
    call isr_handler_c
    add esp, 8

    pop gs
    pop fs
    pop es
    pop ds

    popa
    add esp, 8
    iretd

%assign i 0
%rep 256
ISR_NOERR i
%assign i i+1
%endrep

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

isr_stub_table:
%assign j 0
%rep 256
    dd isr_stub_%+j
%assign j j+1
%endrep
