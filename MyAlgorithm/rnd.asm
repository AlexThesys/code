;constants
A: equ 13
B: equ 7
M: equ 11  ; ieee 754 float 0x41300000  

section .data

    x: dd 51

section .text

    global rnd ; int rnd(int min, int max)
rnd:
    enter 0,0
    
    xor rax, rax
    mov eax, dword [x]
    mov rdx, qword A
    mul edx
    ; imul edx
    add eax, dword B
    mov rcx, qword M  
    ; cdq
    ; idiv ecx
    div ecx
    ; or edx, 0xEF
    mov dword [x], edx

    cvtsi2ss xmm0, edx
    cvtsi2ss xmm1, ecx       
    divss xmm0, xmm1
    cvtss2si rax, xmm0
    add rax, rdi
    sub rsi, rdi
    imul rax, rsi

    leave
    ret

