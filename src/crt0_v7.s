.text
.even
.globl  _program
.globl  _main

.=.+0304

_main:
    jsr pc,_program
    // EMT 0
    .byte 0,0210

// Client ROM Interface

.globl _outc

_outc:
    mov r5, -(sp)
    mov sp, r5
    mov 4(r5), r0
    // EMT 4
    .byte 4,0210
    mov (sp)+,r5
    rts pc

.globl _osword

_osword:
    mov r5, -(sp)
    mov sp, r5
    mov 4(r5), r0
    mov 6(r5), r1
    // EMT 3
    .byte 3,0210
    mov (sp)+,r5
    rts pc
