.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15HandleInterruptEhj

.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptNum)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptNum)
    pushl $0
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:

    # Store registers on stack
    # pusha
    # pushl %ds
    # pushl %es
    # pushl %fs
    # pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    # Call C++ Handler
    pushl %esp
    push (interruptNum)
    call _ZN16InterruptManager15HandleInterruptEhj
    # add &5, %esp
    mov %eax, %esp # Switch stack

    # Restore registers ok stack
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

    # popl %gs
    # popl %fs
    # popl %es
    # popl %ds
    # popa

    add $4, %esp

.global load_idt
load_idt:
    mov 4(%esp), %eax
    lidt (%eax)
    sti
    ret

.global _ZN16InterruptManager22IgnoreInterruptRequestEv
_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret

.data
    interruptNum: .byte 0
