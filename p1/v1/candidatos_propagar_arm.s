							AREA datos, DATA

NUM							EQU 0x40000000

NUM_FILAS					EQU 9
PADDING						EQU 7
NUM_COLUMNAS				EQU NUM_FILAS + PADDING
init_region					DCW 0, 0, 0, 3, 3, 3, 6, 6, 6

							; PARÁMTEROS:
							; r0 = @ini_cuadricula
							; r1 = fila
							; r2 = columna

							AREA codigo, CODE, READONLY
							EXPORT candidatos_propagar_arm
							ENTRY
							
candidatos_propagar_arm		push{fp,lr}
							mov fp,sp
							push{r12}						; registros a usar
							push{r3-r10}					; registros a usar
							
							; obtener la cuadricula[fila][columna]
							; ------------------------------------------------------------------------------------------------------------
							mov r3,#NUM_COLUMNAS

							mov r4,r3,LSL #1				; r4 = NUM_COLUMNAS*2
							mla r6,r4,r1,r0					; r6 =((NUM_COLUMNAS*2)*fila)+@ini = @ini_fila	
							mov r5,r2,LSL #1				; r5 = col*2
							ldrh r12,[r6,r5]				; r12 = cuadricula[fila][columna] = mem[@ini_fila + col*2] (r6+r5 = @cuadricula[fila][columna])
															
							; valor = celda_leer_valor(cuadricula[fila][columna])
							; ------------------------------------------------------------------------------------------------------------
							sub sp,sp,#4					; se hace hueco en la pila para el resultado devuelto
							push {r12}						; r12 = parametro1 = cuadricula[fila][columna]
							bl celda_leer_valor
							add sp,sp,#4					; libera el parámetro de pila
							pop{r12}						; r12 = valor (resultado devuelto por celda_leer_valor)

							; for (j=NUM_FILAS;j>0;j--)
							; ------------------------------------------------------------------------------------------------------------
							mov r7,#NUM_FILAS				; r7 = j = 9		
for1						; celda_eliminar_candidato(&cuadricula[r6],r12)
							; ------------------------------------------------------------------------------------------------------------
							push {r6}						; r6  = @cuadricula
							push {r12}						; r12 = valor
							bl celda_eliminar_candidato
							add sp,sp,#8					; libera los parámetros de pila
							
							add r6,r6,#2					; avanza a la columna siguiente
							subs r7,r7,#1					; r7 = j-- (concición de fin de bucle j = 0)
							bne for1			

							; for (i=NUM_FILAS;i>0;i--)
							; ------------------------------------------------------------------------------------------------------------
							mov r7,#NUM_FILAS				; r7 = i = 9	
							add r6,r0,r5					; r6 = @ini_col
for2						; celda_eliminar_candidato(&cuadricula[r6],r12)
							; ------------------------------------------------------------------------------------------------------------
							push {r6}						; r6 = @cuadricula
							push {r12}						; r12 = valor
							bl celda_eliminar_candidato
							add sp,sp,#8					; libera los parámetros de pila

							add r6,r6,r4					; r6 = r6 + (NUM_COLUMNAS * 2)
							subs r7,r7,#1					; r7 = i-- (concición de fin de bucle i = 0)
							bne for2

							; Encontrar el inicio de la región
							; ------------------------------------------------------------------------------------------------------------
							; init_i = init_region[fila];
							; init_j = init_region[columna];
							; end_i = init_i + 3; //En verdad el fin es init_i + 3 -1 pero el for es < no <=
							; end_j = init_j + 3;
							
							LDR r6,=init_region
							mov r3,r1,LSL #1
							ldrh r7,[r6,r3] 				; r7  = init_i (r3 = fil*2)
							ldrh r8,[r6,r5] 				; r8  = init_j (r5 = col*2)
							add r9,r7,#3					; r9  = end_i
							add r10,r8,#3					; r10 = end_j

for3						; for (i=init_i; i<end_i; i++) 	; avanzar fila a fila
							; ------------------------------------------------------------------------------------------------------------
							mov r3,r8						; se restaura en r9 el valor de la columna para repetir for4

							; Calcular la @cuadricula[init_i][init_j]
							mla r6,r4,r7,r0					; r6  = ((NUM_COLUMNAS*2)*init_i)+@ini =	@ini_fila
							mov r5,r8,LSL #1				; r5  = init_j*2
							add r6,r6,r5					; r6  = @cuadricula[init_i][init_j]

for4						; for(j=init_j; j<end_j; j++)   	; avanzar columna a columna
							; ------------------------------------------------------------------------------------------------------------
							; celda_eliminar_candidato(&cuadricula[r6],r12)
							; ------------------------------------------------------------------------------------------------------------
							push {r6}						; r6 = @cuadricula[init_i][init_j]
							push {r12}						; r12 = valor
							bl celda_eliminar_candidato
							add sp,sp,#8					; libera los parámetros de pila
							
							add r6,r6,#2					; avanza r6 en la región
							add r3,r3,#1					; avanza en la columna
							cmp r3,r10						; se compara r3 (init_j que va avanzando por las columnas) con end_j
							bne for4
							
							add r7,r7,#1					; avanzar la fila en para for3
							cmp r7,r9						; se compara r7 (init_i que va avanzando por las filas) con end_i
							bne for3
							
							pop {r3-r10}
							pop {r12,fp,pc}					; restaurar los registros usados y vuelta del pc a sudoku_2021.c
							
			
celda_leer_valor			push{fp,lr}
							mov fp,sp
							push {r0}
							ldr r0,[fp,#8]					;r0 = celda
							
							and r0,#0x00F					;r12 = valor
							str r0,[fp,#12]					;guardo el resultado que voy a devolver en el hueco
							
							pop{r0,fp,pc}					;restaurar registros y vuelta


celda_eliminar_candidato	push{fp,lr}
							mov fp,sp
							push{r0-r4}
							
							ldr r0,[fp,#12]					;r0 = @ini_fila
							ldr r1,[fp,#8]					;r1 = valor
							
							mov r3,#0x40					;0000 0000 0100 0000
							mov r3,r3,LSL r1				;muevo el 1 tantas posiciones como valor y así obtengo la máscara para eliminar el candidato
							
							ldrh r4,[r0]					;r14 = mem[r0]
							orr r4,r4,r3					;r4 = or bit a bit para poner a 1 el bit de r4 que está a 1 en r3 y no tocar el resto
							strh r4,[r0]					;mem[r0] = r4
							
							pop{r0-r4,fp,pc}				;restaurar registros y vuelta
							
							END