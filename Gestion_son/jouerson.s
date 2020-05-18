    	thumb
    	area	moncode, code, readonly
	
E_POS	equ	0
E_TAI	equ	4
E_SON	equ	8
E_RES	equ	12
E_PER	equ	16
TIM3_CCR3	equ	0x4000043C	; adresse registre PWM
		
		extern etat
		export timer_callback

timer_callback proc
	push {lr}
	push {r4, r5, r6, r7, r8, r9, r10}
	ldr r8, =etat
	ldr r9, =E_SON
	ldr r6, [r8, r9]
	ldr r9, =E_POS
	ldr r4, [r8, r9]
	ldr r9, =E_TAI
	ldr r5, [r8, r9]
	cmp r4, r5
	bhs fin
	
	;r7 = echantillon a traiter et jouer
	ldrsh r7, [r6, r4]
	;operation sur les echantillons
	;addition
	adds r7, #0x8000
	;mise a l'echelle	
	ldr r10, =E_RES
	ldr r9, [r8, r10]
	mul r7, r7, r9
	mov r9, #0xffff
	udiv r7, r7, r9
	
	;emission du son
	ldr r9, =TIM3_CCR3
	str r7, [r9]
	; incrementation de la position
	add r4, #2
	ldr r10, =E_POS
	mov r9, r8
	add r9, r10
	str r4, [r9]
	
fin	pop {r4, r5, r6, r7, r8, r9, r10}
	pop {pc}
	endp
	end