.text
.even
.globl  _program
.globl  ___main
.globl  _start

_start:
        jsr pc,_program
        emt 0

/ ___main: called by C main() function. Currently does nothing
___main:
        rts     pc

// Client ROM Interface

.globl _outc

_outc:
    mov r5, -(sp)
    mov sp, r5
	mov 4(r5), r0
    emt 4
    mov (sp)+,r5
    rts pc
