section .text
global _ZN11FOFB_Filter10filterSIMDEPfP3Gui  ; FOFB_Filter::filterSIMD(float*, Gui*) 

_ZN11FOFB_Filter10filterSIMDEPfP3Gui:
; prologue
    push   rbp
    mov    rbp,rsp
    sub    rsp,0x18
    mov    QWORD [rbp-0x8],rdi          ; *this (delay[4])
    mov    QWORD [rbp-0x10],rsi         ; *buf
    mov    QWORD [rbp-0x18],rdx         ; *gui
; float a0 = gui->fofb_a0;
    mov    rdi,QWORD [rbp-0x18]
    add    rdi,0xaf8
    call   0x5555556be678 ;<ofParameter<float>::operator float const&() const>
    movss  xmm2,DWORD [rax]             ; a
    shufps xmm2, xmm2, 00000000b
; float b1 = gui->fofb_b1
    mov    rdi,QWORD [rbp-0x18]
    add    rdi,0xb30
    call   0x5555556be678 ;<ofParameter<float>::operator float const&() const>
    movss  xmm3,DWORD [rax]             ; b
    shufps xmm3, xmm3, 00000000b
; float buf[4] = (*buf);
    mov    rax,QWORD [rbp-0x10]
    movaps xmm0,OWORD [rax]
; buf[4] = (buf[4] * a0) - (delay[4] * b1);
    mulps  xmm0, xmm2                   ; buf[4]*a0
    mov    rax,QWORD [rbp-0x8]
    movaps xmm1,OWORD [rax]
    mulps  xmm1, xmm3                   ; delay[4]*b1
    subps  xmm0,xmm1                    ; buf[4]-delay[4]
; * buf    = delay.l0 = buf_0;
    mov    rax,QWORD [rbp-0x8]
    movaps OWORD [rax],xmm0
    mov    rax,QWORD [rbp-0x10]
    movaps OWORD [rax],xmm0
; epilogue
    nop
    leave
    ret
