							AREA datos, DATA

NUM_FILAS					EQU 9
PADDING						EQU 7
NUM_COLUMNAS				EQU NUM_FILAS + PADDING
init_region					DCB 0, 0, 0, 3, 3, 3, 6, 6, 6

	
							PRESERVE8							;Para hacer llamadas a codigo c
							AREA cod, CODE
							EXPORT candidatos_actualizar_arm_c
							EXPORT candidatos_propagar_arm
							EXPORT	candidatos_actualizar_arm
							EXTERN candidatos_propagar_c
							ENTRY
		
		
		
candidatos_actualizar_arm_c
		PUSH	{r1-r6,r14}
		mov 	r3,#0						;r3 = celdas_vacias
		LDR		r1,=NUM_FILAS				;r1 = i (AMBAS, numero de iteraciones del primer for y para direccionar la fila de la celda)
		subs	r1,r1,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
		mov		r6,r0						;Como la direccion de la celda necesita estar en r0 para pasarlo como parametro a esVacia y eliminaCandidatos
		
b11		LDR		r2,=NUM_FILAS				;r2 = j	(AMBAS, numero de iteraciones del segundo for y para direccionar la columna de la celda)
		subs	r2,r2,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
b21		mov		r0,r6						;Recupero el valor de la primera celda para calcular la que toca

		add		r0,r0,r2,LSL#1
		add		r0,r0,r1,LSL#5				;Calculo la direccion de la celda
						
;-----------esVacia
		ldrh 	r5,[r0]					;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		ands	r4,r5,#0x000F			;r0 = valor de la celda
;-----------

;-----------elminaCandidatos si es vacia
		ldrheq	r5,[r0]					;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		andeq	r5,r5,#0x007F
		strheq	r5,[r0]
;-----------

		subs	r2,r2,#1					;Una iteracion del segundo for
		bpl 	b21							;Acaba cuando j < 0 para calcular la columna 0
		
		subs	r1,r1,#1					;Una iteracion del primer for
		bpl		b11							;Acaba cuando i < 0 para calcular la fila 0
		
;-----------SEGUNDO PAR DE BUCLES ANIDADOS
		LDR		r1,=NUM_FILAS				;r1 = i (AMBAS, numero de iteraciones del primer for y para direccionar la fila de la celda)
		subs	r1,r1,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
		mov		r6,r0						;Como la direccion de la celda necesita estar en r0 para pasarlo como parametro a esVacia y eliminaCandidatos
		
b12		LDR		r2,=NUM_FILAS				;r2 = j	(AMBAS, numero de iteraciones del segundo for y para direccionar la columna de la celda)
		subs	r2,r2,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
b22		mov		r0,r6						;Recupero el valor de la primera celda para calcular la que toca

		add		r0,r0,r2,LSL#1
		add		r0,r0,r1,LSL#5				;Calculo la direccion de la celda
		
;-----------esVacia
		ldrh 	r5,[r0]					;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		ands	r4,r5,#0x000F			;r0 = valor de la celda
;-----------			
		

		;cmp		r0,#0						;Comparo el resultado para acutalizar los flags
		;POP		{r0}						;Recupero, ya habiendo comparado r0, el valor de r0 para que lo use elimina candidatos
		addeq	r3,r3,#1					;Si estaba vacia sumo 1 a celdas vacias
		PUSH	{r1-r3}						;Para guardar r1 y r2 (ya que necesito pasarselo como parametro, pero me los destroza)
		movne	r0,r6
		blne	candidatos_propagar_c		;Si la celda estaba vacia, propago sus candidatos(r0 = direcion celda, r1 = i, r2 = j)
		POP		{r1-r3}	
		
		subs	r2,r2,#1					;Una iteracion del segundo for
		bpl 	b22							;Acaba cuando j < 0 para calcular la columna 0
		
		subs	r1,r1,#1					;Una iteracion del primer for
		bpl		b12							;Acaba cuando i < 0 para calcular la fila 0

		mov r0,r3							;se pasan las celdas vacías a r0 para devolverlas
		POP	{r1-r6,pc}




							; PARÁMTEROS:
							; r0 = @ini_cuadricula
							; r1 = fila
							; r2 = columna
							
candidatos_propagar_arm		;push{lr}
							;mov fp,sp
							;push{r12,lr}					; registros a usar
							push{r3-r10,r12,lr}					; registros a usar
							
							; obtener la cuadricula[fila][columna]
							; ------------------------------------------------------------------------------------------------------------
							;mov r3,#NUM_COLUMNAS
													
							add		r6,r0,r1,LSL#5			; r6 = @ini_fila
							add		r5,r6,r2,LSL#1			; r5 = cuadricula[fila][columna]

							ldrh r12,[r5]					; r12 = cuadricula[fila][columna] = mem[@ini_fila + col*2] (r6+r5 = @cuadricula[fila][columna])
															
							; valor = celda_leer_valor(cuadricula[fila][columna])
;-------------------------------celda_leer_valor
							and r12,#0x00F					;r12 = valor					
;-------------------------------						
							; for (j=NUM_FILAS;j>0;j--)
							; ------------------------------------------------------------------------------------------------------------
							mov r7,#NUM_FILAS				; r7 = j = 9		
for1						

							; celda_eliminar_candidato(&cuadricula[r6],r12)
;-------------------------------celda_eliminar_candidato
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r12				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r6]					;r4 = mem[r5]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r6]					;mem[r5] = r4
;-------------------------------

							add r6,r6,#2					; avanza a la columna siguiente
							subs r7,r7,#1					; r7 = j-- (concición de fin de bucle j = 0)
							bne for1			





							; for (i=NUM_FILAS;i>0;i--)
							; ------------------------------------------------------------------------------------------------------------
							mov r7,#NUM_FILAS				; r7 = i = 9
							sub r5,r5,r1,LSL#5				; r5 = @ini_col
							
							
							
for2						; celda_eliminar_candidato(&cuadricula[r6],r12)
;-------------------------------celda_eliminar_candidato
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r12				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r5]					;r4 = mem[r5]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r5]					;mem[r5] = r4
;-------------------------------					; libera los parámetros de pila

							add r5,r5,#32					; r5 = cuadricula[fila + 1][columna] 
							subs r7,r7,#1					; r7 = i-- (concición de fin de bucle i = 0)
							bne for2

							; Encontrar el inicio de la región
							; ------------------------------------------------------------------------------------------------------------
							; init_i = init_region[fila];
							; init_j = init_region[columna];
							; end_i = init_i + 3; //En verdad el fin es init_i + 3 -1 pero el for es < no <=
							; end_j = init_j + 3;
							
							LDR r6,=init_region
							
							add r7,r6,r1
							add r8,r6,r2
							
							;mov r3,r1,LSL #1
							ldrb r7,[r7] 					; r7  = init_i (fila)
							ldrb r8,[r8] 					; r8  = init_j (col)
							add r9,r7,#3					; r9  = end_i
							add r10,r8,#3					; r10 = end_j

for3						; for (i=init_i; i<end_i; i++) 	; avanzar fila a fila
							; ------------------------------------------------------------------------------------------------------------
							mov r11,r8						; se restaura en r9 el valor de la columna para repetir for4

							; Calcular la @cuadricula[init_i][init_j]
							add		r5,r0,r7,LSL#5			
							add		r5,r5,r8,LSL#1
							
							

for4						; for(j=init_j; j<end_j; j++)   	; avanzar columna a columna
							; ------------------------------------------------------------------------------------------------------------
							; celda_eliminar_candidato(&cuadricula[r6],r12)
							; celda_eliminar_candidato(&cuadricula[r6],r12)
;-------------------------------celda_eliminar_candidato
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r12				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r5]					;r4 = mem[r5]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r5]					;mem[r5] = r4
;-------------------------------
							
							add r5,r5,#2					; avanza r6 en la región
							add r11,r11,#1					; avanza en la columna
							cmp r11,r10						; se compara r3 (init_j que va avanzando por las columnas) con end_j
							bne for4
							
							add r7,r7,#1					; avanzar la fila en para for3
							cmp r7,r9						; se compara r7 (init_i que va avanzando por las filas) con end_i
							bne for3
							
							pop {r3-r10,r12,pc}				; restaurar los registros usados y vuelta del pc a sudoku_2021.c
							;pop {r12,pc}					; restaurar los registros usados y vuelta del pc a sudoku_2021.c
							
			
			
candidatos_actualizar_arm			
		PUSH	{r1-r12,r14}
		mov 	r3,#0						;r3 = celdas_vacias
		LDR		r1,=NUM_FILAS				;r1 = i (AMBAS, numero de iteraciones del primer for y para direccionar la fila de la celda)
		subs	r1,r1,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
		mov		r6,r0						;Como la direccion de la celda necesita estar en r0 para pasarlo como parametro a esVacia y eliminaCandidatos
		
arm_armb11		LDR		r2,=NUM_FILAS				;r2 = j	(AMBAS, numero de iteraciones del segundo for y para direccionar la columna de la celda)
		subs	r2,r2,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
arm_armb21		mov		r0,r6						;Recupero el valor de la primera celda para calcular la que toca

		add		r0,r0,r2,LSL#1
		add		r0,r0,r1,LSL#5				;Calculo la direccion de la celda
						
;-----------esVacia
		ldrh 	r5,[r0]					;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		ands	r4,r5,#0x000F			;r0 = valor de la celda
;-----------

;-----------elminaCandidatos si es vacia
		ldrheq	r5,[r0]					;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		andeq	r5,r5,#0x007F
		strheq	r5,[r0]
;-----------

		subs	r2,r2,#1					;Una iteracion del segundo for
		bpl 	arm_armb21							;Acaba cuando j < 0 para calcular la columna 0
		
		subs	r1,r1,#1					;Una iteracion del primer for
		bpl		arm_armb11							;Acaba cuando i < 0 para calcular la fila 0
		
;-----------SEGUNDO PAR DE BUCLES ANIDADOS
		LDR		r1,=NUM_FILAS				;r1 = i (AMBAS, numero de iteraciones del primer for y para direccionar la fila de la celda)
		subs	r1,r1,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
		mov		r6,r0						;Como la direccion de la celda necesita estar en r0 para pasarlo como parametro a esVacia y eliminaCandidatos
		
arm_armb12		LDR		r2,=NUM_FILAS				;r2 = j	(AMBAS, numero de iteraciones del segundo for y para direccionar la columna de la celda)
		subs	r2,r2,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
arm_armb22		mov		r0,r6						;Recupero el valor de la primera celda para calcular la que toca

		add		r0,r0,r2,LSL#1
		add		r0,r0,r1,LSL#5				;Calculo la direccion de la celda
		
;-----------esVacia
		ldrh 	r5,[r0]					;r5 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		ands	r4,r5,#0x000F			;r0 = valor de la celda
;-----------			
		

		;cmp		r0,#0						;Comparo el resultado para acutalizar los flags
		;POP		{r0}						;Recupero, ya habiendo comparado r0, el valor de r0 para que lo use elimina candidatos
		addeq	r3,r3,#1					;Si estaba vacia sumo 1 a celdas vacias
		movne	r0,r6
		beq		noPropagar
		
		;r0 = cuadricula 
		;r1 = fila
		;r2 = columna
		;r3 = celdas_vacias
		;r6 = cuadricula backup
		
		
		
		push{r3,r6}
;-----------candidatos_propagar_arm
							
							; obtener la cuadricula[fila][columna]
							; ------------------------------------------------------------------------------------------------------------
							;mov r3,#NUM_COLUMNAS
													
							add		r6,r0,r1,LSL#5			; r6 = @ini_fila
							add		r5,r6,r2,LSL#1			; r5 = cuadricula[fila][columna]

							ldrh r12,[r5]					; r12 = cuadricula[fila][columna] = mem[@ini_fila + col*2] (r6+r5 = @cuadricula[fila][columna])
															
							; valor = celda_leer_valor(cuadricula[fila][columna])
;-------------------------------celda_leer_valor
							and r12,#0x00F					;r12 = valor					
;-------------------------------						
							; for (j=NUM_FILAS;j>0;j--)
							; ------------------------------------------------------------------------------------------------------------
							mov r7,#NUM_FILAS				; r7 = j = 9		
arm_armfor1						

							; celda_eliminar_candidato(&cuadricula[r6],r12)
;-------------------------------celda_eliminar_candidato
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r12				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r6]					;r4 = mem[r5]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r6]					;mem[r5] = r4
;-------------------------------

							add r6,r6,#2					; avanza a la columna siguiente
							subs r7,r7,#1					; r7 = j-- (concición de fin de bucle j = 0)
							bne arm_armfor1			





							; for (i=NUM_FILAS;i>0;i--)
							; ------------------------------------------------------------------------------------------------------------
							mov r7,#NUM_FILAS				; r7 = i = 9
							sub r5,r5,r1,LSL#5				; r5 = @ini_col
							
							
							
arm_armfor2						; celda_eliminar_candidato(&cuadricula[r6],r12)
;-------------------------------celda_eliminar_candidato
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r12				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r5]					;r4 = mem[r5]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r5]					;mem[r5] = r4
;-------------------------------					; libera los parámetros de pila

							add r5,r5,#32					; r5 = cuadricula[fila + 1][columna] 
							subs r7,r7,#1					; r7 = i-- (concición de fin de bucle i = 0)
							bne arm_armfor2

							; Encontrar el inicio de la región
							; ------------------------------------------------------------------------------------------------------------
							; init_i = init_region[fila];
							; init_j = init_region[columna];
							; end_i = init_i + 3; //En verdad el fin es init_i + 3 -1 pero el for es < no <=
							; end_j = init_j + 3;
							
							LDR r6,=init_region
							
							add r7,r6,r1
							add r8,r6,r2
							
							;mov r3,r1,LSL #1
							ldrb r7,[r7] 					; r7  = init_i (fila)
							ldrb r8,[r8] 					; r8  = init_j (col)
							add r9,r7,#3					; r9  = end_i
							add r10,r8,#3					; r10 = end_j

arm_armfor3						; for (i=init_i; i<end_i; i++) 	; avanzar fila a fila
							; ------------------------------------------------------------------------------------------------------------
							mov r11,r8						; se restaura en r9 el valor de la columna para repetir for4

							; Calcular la @cuadricula[init_i][init_j]
							add		r5,r0,r7,LSL#5			
							add		r5,r5,r8,LSL#1
							
							

arm_armfor4						; for(j=init_j; j<end_j; j++)   	; avanzar columna a columna
							; ------------------------------------------------------------------------------------------------------------
							; celda_eliminar_candidato(&cuadricula[r6],r12)
							; celda_eliminar_candidato(&cuadricula[r6],r12)
;-------------------------------celda_eliminar_candidato
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r12				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r5]					;r4 = mem[r5]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r5]					;mem[r5] = r4
;-------------------------------
							
							add r5,r5,#2					; avanza r6 en la región
							add r11,r11,#1					; avanza en la columna
							cmp r11,r10						; se compara r3 (init_j que va avanzando por las columnas) con end_j
							bne arm_armfor4
							
							add r7,r7,#1					; avanzar la fila en para for3
							cmp r7,r9						; se compara r7 (init_i que va avanzando por las filas) con end_i
							bne arm_armfor3
								
;-------------------------------------------------------------------------------------
			pop{r3,r6}


noPropagar
		
		subs	r2,r2,#1					;Una iteracion del segundo for
		bpl 	arm_armb22							;Acaba cuando j < 0 para calcular la columna 0
		
		subs	r1,r1,#1					;Una iteracion del primer for
		bpl		arm_armb12							;Acaba cuando i < 0 para calcular la fila 0

		mov r0,r3							;se pasan las celdas vacías a r0 para devolverlas
		POP	{r1-r12,pc}		

							END