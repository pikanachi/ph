		AREA data, DATA

NUM_FILAS 	EQU 9
NUM_COLUMNAS	EQU 16
BIT_CANDIDATOS	EQU 7
		PRESERVE8							;Para hacer llamadas a codigo c
		AREA cod, CODE
		EXPORT candidatos_actualizar_arm_c
		EXTERN candidatos_propagar_c
		ENTRY
		
		
		
candidatos_actualizar_arm_c
		PUSH	{r1-r7,r14}
		mov 	r3,#0						;r3 = celdas_vacias
		LDR		r1,=NUM_FILAS				;r1 = i (AMBAS, numero de iteraciones del primer for y para direccionar la fila de la celda)
		subs	r1,r1,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
		mov		r6,r0						;Como la direccion de la celda necesita estar en r0 para pasarlo como parametro a esVacia y eliminaCandidatos
		
b11		LDR		r2,=NUM_FILAS				;r2 = j	(AMBAS, numero de iteraciones del segundo for y para direccionar la columna de la celda)
		subs	r2,r2,#1					;Como el valor de NUM_FILAS es 9, le resto 1 para poder direccionar bien las celdas
		
b21		mov		r0,r6						;Recupero el valor de la primera celda para calcular la que toca

		add		r0,r0,r2,LSL#1
		add		r0,r0,r1,LSL#5				;Calculo la direccion de la celda
		
		PUSH	{r0}						;Como esVacia devuelve su resultado en r0 lo guardo en la pila
		bl		esVacia						
		
		cmp		r0,#0						;Comparo el resultado para acutalizar los flags
		POP		{r0}						;Recupero, ya habiendo comparado r0, el valor de r0 para que lo use elimina candidatos
		blne	elminaCandidatos			;Si la celda estaba vacia, elimino sus candidatos
		
		
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
		
		PUSH	{r0}						;Como esVacia devuelve su resultado en r0 lo guardo en la pila
		bl		esVacia						
		
		cmp		r0,#0						;Comparo el resultado para acutalizar los flags
		POP		{r0}						;Recupero, ya habiendo comparado r0, el valor de r0 para que lo use elimina candidatos
		addne	r3,r3,#1					;Si estaba vacia sumo 1 a celdas vacias
		PUSH	{r1-r2}						;Para guardar r1 y r2
		moveq	r0,r6
		bleq	candidatos_propagar_c		;Si la celda estaba vacia, propago sus candidatos(r0 = direcion celda, r1 = i, r2 = j)
		POP		{r1-r2}	
		
		subs	r2,r2,#1					;Una iteracion del segundo for
		bpl 	b22							;Acaba cuando j < 0 para calcular la columna 0
		
		subs	r1,r1,#1					;Una iteracion del primer for
		bpl		b12							;Acaba cuando i < 0 para calcular la fila 0


		POP	{r1-r7,r14}
		bx	r14


esVacia
		PUSH	{r1,r14}
		ldrh 	r1,[r0]						;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		and		r0,r1,#0x000F				;r0 = valor de la celda
		cmp		r0,#0				
		moveq	r0,#1						;Si es 0, el resultado es 1
		movne	r0,#0
		POP		{r1,r14}
		bx		r14
		
		
		
		
		
elminaCandidatos
		PUSH	{r1,r14}
		ldrh	r1,[r0]						;r1 = 31-16(DONT CARE) 15-0(INFORMACION DE LA CELDA)
		and 	r1,r1,#0x007F
		strh	r1,[r0]
		POP		{r1,r14}
		bx		r14
		END