BITS 16
ORG 0x7C00

KERNEL_LOAD_SEG  EQU 0x1000
KERNEL_LOAD_OFF  EQU 0x0000
KERNEL_SECTORS   EQU 64

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl

    call enable_a20
    call set_video_mode
    call load_kernel

    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEL:protected_mode_entry

set_video_mode:
    mov ax, 0x0013
    int 0x10
    ret

enable_a20:
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ret

load_kernel:
    mov ax, KERNEL_LOAD_SEG
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, disk_error_msg
.print:
    lodsb
    test al, al
    jz .halt
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x0F
    int 0x10
    jmp .print
.halt:
    hlt
    jmp .halt

[BITS 32]
protected_mode_entry:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp CODE_SEL:0x00010000

[BITS 16]
gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEL EQU 0x08
DATA_SEL EQU 0x10

boot_drive db 0
disk_error_msg db 'Disk read error', 0

times 510-($-$$) db 0
dw 0xAA55
