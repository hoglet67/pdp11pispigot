.globl	csv
.globl	cret

csv:
	mov	r5,r0
	mov	sp,r5
	mov	r4,-(sp)
	mov	r3,-(sp)
	mov	r2,-(sp)
	tst	-(sp)
	jmp	(r0)

cret:
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
