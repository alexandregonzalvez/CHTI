	thumb
	AREA code_dft_partie_imaginaire, code, READONLY
	extern TabSin
	export somme_partie_imaginaire
		
somme_partie_imaginaire proc
	
	ldr r2, =TabSin

	mov r7, #0
	mov r3, #0
db	mul r4, r0, r3
	and r4, r4, #0x003f
	ldrsh r5, [r2, r4, lsl #1]
	ldrsh r6, [r1, r3, lsl #1]	
	mla r7, r5, r6, r7
	add r3, #1
	cmp r3, #64
	blt db
	
	mov r0, r7
	
	bx lr
	endp
	end