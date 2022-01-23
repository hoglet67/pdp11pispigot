.sect .text

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0
    mov r0,r0

.extern _main

_main:
    jsr pc, _program
    emt 0

.extern _outc

_outc:
    mov (sp)+, r1
    mov (sp), r0
    emt 4
    jmp (r1)

.extern _osword

_osword:
    mov 2(sp), r0
    mov 4(sp), r1
    emt 3
    mov (sp)+, r1
    add sp, $4
    jmp (r1)
