				AREA datos,DATA				
						
VICIntEnable	EQU 0xFFFFF010 					; registro base enable periféricos en VIC		
IRQ_Index_Tec	EQU 7							; índice del teclado en VIC
chanel2irq		DCB 	0,1,2,3,4,5,6,7,8,9,10,12,13,14,15,16
I_Bit			EQU 0x80
				
				
				PRESERVE8							;Para hacer llamadas a codigo c
				AREA cod, CODE
				EXPORT enable_irq
				
enable_irq		PUSH {r4-r10,lr}					;regs a usar y lr

				LDR 	r0,=0xFFFFF200 
				LDR 	r1,=chanel2irq 
				eor 	r2,r2,r2 
VIC0			ldrb 	r3,[r1],#1 
				orr 	r3,r3,#2_100000 
				str 	r3,[r0,r2,LSL #2] 
				add 	r2,r2,#1 
				cmp 	r2,#16 
				bne 	VIC0 
				
				;Habilitar interrpción en VIC -> RSI_teclado
				;----------------------------------------------------------------------------------------------------------
				LDR r1,=RSI_teclado					; r1 = @rutina servicio interrupción teclado
				LDR r2,=IRQ_Index_Tec				; r2 = índiceTeclado
				
				str r1,[r0,r2,LSL#2]				; guardo @rsi_key en (@baseVIC+(índiceTeclado*4))

				; Configurar el VIC para que intEnable pase de 0 a 1 para que escuche las interrupciones de la posición 4
				;(Timer) y de la 7(UART1)
				;----------------------------------------------------------------------------------------------------------
				LDR r0,=VICIntEnable				; r0 = @base Enable VICIntEnable
				ldr r1,[r0]							; r1 = contenido r0 (estrictamente no es necesaria)
				orr r1,r1,#144						; ponemos el VicIntEnable a 1 con esta máscara a 0000 0000 1001 0000 
													; para el Timer 0 y UART1
				str r1,[r0]


				POP	{r4-r10,pc}	

RSI_teclado 	;##########################################################################################################
				;#											RSI_TECLADO													  #
				;##########################################################################################################
				
				; Rutina de servicio a la interrupcion IRQ7 (teclado) al pulsar tecla en UART	
				;==========================================================================================================
				
				;INICIO RSI
				;==========================================================================================================

				; Silenciamos execepciones para no perder estado y guardo @ retorno y palabra de estado
				;----------------------------------------------------------------------------------------------------------
				sub lr,lr,#4				; aumentamos lr para que apunte a la siguiente instrucción
				push {lr}					; guardamos el @ de retorno
				mrs r14,spsr				; guardamos r14 pues es el único que podemos machacar, el cpsr (estado flags) del modo anterior que es spsr actual
				push {r14}					; guardamos el spsr para no perder estado
				push {r0,r1}				; guardamos los registros a usar
				
				; Activar IRQ. A partir de "Activo ->" ya nos pueden interrumpir
				;----------------------------------------------------------------------------------------------------------			
				mrs r1,cpsr					; habilita IRQ de la palabra de estado del modo activo y la mete en cprs
				bic r1,r1,#I_Bit			; pone a 0 el 1º bit de las IRQ 0x80 -> bit 7 (si nos pueden interrumpir o no)
;Activo ->
				msr	cpsr_c,r1				; _c indica que copiamos el byte menos significativo


				END