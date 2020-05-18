	thumb
	AREA code_m2k, code, READONLY	
	export m2k
	extern somme_partie_reelle
	extern somme_partie_imaginaire
		
m2k proc
	push {lr}
	push {r4, r5, r6, r7, r8, r9, r11, r12}
	
	mov r8, #0
	mov r9, #0
	mov r10, r0
	bl somme_partie_reelle
	smlal r8, r9, r0, r0
	mov r0, r10
	bl somme_partie_imaginaire
	smlal r8, r9, r0, r0 
	mov r0, r9
	
	pop {r4, r5, r6, r7, r8, r9, r11, r12}
	pop {pc}
	
	endp
	end
	