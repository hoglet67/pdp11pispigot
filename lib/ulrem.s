/ Unsigned long remainder

.globl	ulrem
.globl	csv, cret
ulrem:
	jsr	r5,csv
	tst	010(r5)
	bne	ulrem1
	mov	012(r5),r3
	bmi	ulrem1
	mov	06(r5),r2
	mov	04(r5),r1
	bmi	ulrem2
	clr	r0
	div	r3,r0
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	sub	r3,r0		/ this is the clever part
	div	r3,r0
	tst	r1
	beq	1f
	add	r3,r1
1:
	clr	r0
	jmp	cret

/ The divisor is known to be >= 2^15 so only 16 cycles are needed.
ulrem1:
	mov	06(r5),r2
	mov	04(r5),r1
	clr	r0
	mov	010(r5),r3
	mov	$020,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	bhi	3f
	blo	2f
	cmp	012(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	012(r5),r1
	sbc	r0
	sub	r3,r0
	sob	r4,1b
1:
	jmp	cret

/ The dividend is known to be >= 2^31 so 32 cycles are needed.
ulrem2:
	mov	06(r5),r3
	mov	04(r5),r2
	clr	r1
	clr	r0
	mov	$040,r4
1:
	clc
	rol	r3
	rol	r2
	rol	r1
	rol	r0
	cmp	010(r5),r0
	bhi	3f
	blo	2f
	cmp	012(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	012(r5),r1
	sbc	r0
	sub	010(r5),r0
	sob	r4,1b
1:
	jmp	cret
