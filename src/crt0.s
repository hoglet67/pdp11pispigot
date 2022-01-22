        .text
        .even
        .globl  _program
        .globl  ___main
        .globl  _start

/ _start: initialize stack pointer,
/         clear vector memory area,
/         save program entry in vector 0
/         call C main() function
_start:
/        mov     $00776,sp
/        clr     r0
/L_0:
/        clr     (r0)+
/        cmp     r0, $400
/        bne     L_0
/        mov     $000137,*$0     / Store JMP _start in vector 0
/        mov     $_start,*$2
        jsr     pc,_program
/ EMT 0
        .byte 0, 0210

/ ___main: called by C main() function. Currently does nothing
___main:
        rts     pc
