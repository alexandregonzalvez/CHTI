	thumb
	AREA code_m2k, code, READONLY	
	export m2k	
	extern TabCos
	extern TabSin
	
m2k proc	
	push {r12}

; dft partie reelle + im
	ldr r2, =TabCos
	ldr r11, =TabSin
	
	mov r3, #0
	; r4 = ik2pi/N
	; r3 = i
db	mul r4, r0, r3
	and r4, r4, #0x003f
	; r5 = cos[ik2pi/n]
	; r6 = signal[i]
	; r10 = sin[ik2pi/n]
	ldrsh r5, [r2, r4, lsl #1]
	ldrsh r10,[r11, r4, lsl #1]
	ldrsh r6, [r1, r3, lsl #1]
	; r7 += r5 * r6
	; r12 += r10 * r6
	mla r7, r5, r6, r7
	mla r12, r10, r6, r12
	; incremente r3
	add r3, #1
	cmp r3, #64
	blt db

	smlal r8, r9, r7, r7
	smlal r8, r9, r12, r12

	mov r0, r9
	
	pop {r12}
	
	bx lr
	
	endp
	end
	