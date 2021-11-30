ENMASK			EQU		0x18030
VICIntEnable	EQU		0xFFFFF010
VICIntEnClr		EQU		0xFFFFF014
                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  FIQ_Handler
				IMPORT 	timer0_ISR
FIQ_Handler 	
				LDR  	r1,=timer0_ISR
				MOV     LR, PC
				BX 		r1
				LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
				END