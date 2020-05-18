	thumb
	AREA code_somme, code, READONLY
	export somme
	extern TabSin
	extern TabCos

somme proc
	ldr r2, =TabSin
	ldr r3, =TabCos
	
	ldrsh r5, [r2, r0, lsl #1]
	ldrsh r6, [r3, r0, lsl #1]
	
	mul r4, r5, r5
	mul r5, r6, r6
	
	add r0, r4, r5
	
	bx lr
	endp
	end