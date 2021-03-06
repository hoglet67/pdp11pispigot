.globl	___divsi3
.globl	___modsi3

/ From:
/ https://www.tuhs.org/cgi-bin/utree.pl?file=Xinu7/src/lib/libxc/ldiv.s
/ https://www.tuhs.org/cgi-bin/utree.pl?file=Xinu7/src/lib/libxc/lrem.s

/ Long quotient

___divsi3:
        jsr     r5,csv
	mov	10(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
	bmi	hardldiv
1:
	cmp	r4,8(r5)
	bne	hardldiv
	mov	6(r5),r2
	mov	4(r5),r1
	bge	1f
	neg	r1
	neg	r2
	sbc	r1
	com	r4
1:
	mov	r4,-(sp)
	clr	r0
	div	r3,r0
	mov	r0,r4		/high quotient
	mov	r1,r0
	mov	r2,r1
	mov	r0,-(sp)
	div	r3,r0
	bvc	1f
	mov	(sp),r0
	mov	r2,r1		/ restore clobbered dividend
	sub	r3,r0		/ this is the clever part
	div	r3,r0
	tst	r1
	sxt	r1
	add	r1,r0		/ cannot overflow!
1:
	tst	(sp)+
	mov	r0,r1
	mov	r4,r0
	tst	(sp)+
	bpl	9f
	neg	r0
	neg	r1
	sbc	r0
9:
        jmp     cret

/ The divisor is known to be >= 2^15 so only 17 cycles are needed.
hardldiv:
	clr	-(sp)
	mov	6(r5),r2
	mov	4(r5),r1
	bpl	1f
	com	(sp)
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	mov	8(r5),r3
	bge	1f
	neg	r3
	neg	10(r5)
	sbc	r3
	com	(sp)
1:
	clr	-(sp)
	cmp	r3,r0
	bhi	1f
	blo	2f
	cmp	10(r5),r1
	bhi	1f
2:
	inc	(sp)
	sub	10(r5),r1
	sbc	r0
	sub	r3,r0
1:
	mov	$16,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	bhi	3f
	blo	2f
	cmp	10(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	10(r5),r1
	sbc	r0
	sub	r3,r0
	inc	r2
	sob	r4,1b
1:
	mov	r2,r1
	mov	(sp)+,r0
	tst	(sp)+
	beq	1f
	neg	r0
	neg	r1
	sbc	r0
1:
        jmp     cret

/ Long remainder

___modsi3:
        jsr     r5, csv
	mov	10(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
	bmi	hardlrem
1:
	cmp	r4,8(r5)
	bne	hardlrem
	mov	6(r5),r2
	mov	4(r5),r1
	mov	r1,r4
	bge	1f
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	div	r3,r0
	mov	r1,r0
	mov	r2,r1
	mov	r0,(sp)
	div	r3,r0
	bvc	1f
	mov	r2,r1		/ restore clobbered dividend
	mov	(sp),r0
	sub	r3,r0
	div	r3,r0
	tst	r1
	beq	9f
	add	r3,r1
1:
	tst	r4
	bpl	9f
	neg	r1
9:
	sxt	r0
        jmp     cret

/ The divisor is known to be >= 2^15.  Only 17 cycles are
/ needed to get a remainder.
hardlrem:
	mov	6(r5),r2
	mov	4(r5),r1
	bpl	1f
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	mov	8(r5),r3
	bge	1f
	neg	r3
	neg	10(r5)
	sbc	r3
1:
	cmp	r3,r0
	blo	1f
	bhi	2f
	cmp	10(r5),r1
	bhi	2f
1:
	sub	r3,r0
	sub	10(r5),r1
	sbc	r0
2:
	mov	$16,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	blo	2f
	bhi	3f
	cmp	10(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	10(r5),r1
	sbc	r0
	sub	r3,r0
	sob	r4,1b
1:
	tst	4(r5)
	bge	1f
	neg	r0
	neg	r1
	sbc	r0
1:
        jmp     cret

csv:
	mov	r5,r0
	mov	sp,r5
	mov	r4,-(sp)
	mov	r3,-(sp)
	mov	r2,-(sp)
	tst	-(sp)
	jmp	(r0)

cret:
    // TODO Use r2!
// This version corrupts r0 which is used for the hi word of the division result
//	mov	r5,r0
//	mov	-(r0),r4
//	mov	-(r0),r3
//	mov	-(r0),r2
//	mov	r5,sp
//	mov	(sp)+,r5
//	rts	pc
	mov	(sp)+,r2
	mov	(sp)+,r3
	mov	(sp)+,r4
	tst (sp)+
	mov	(sp)+,r5
	rts	pc
