							AREA datos, DATA



NUM_FILAS					EQU 0x10000000


							AREA codigo, CODE
							ENTRY
			

							mrs r1,cpsr
							orr r1,r1,#NUM_FILAS
							MSR cpsr_cxsf,r1
							
fin							b fin
							
							END
							
							END