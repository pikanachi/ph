#include <LPC210x.H>          // LPC21xx definitions
#include <stdint.h> 					// Para los uint
#include "Serial_port.h"

#define CR     		0x0D
#define MAX_buffer 	10

char car;

// Buffer
static char buffer[MAX_buffer];
uint8_t index_buffer = 0;
static uint8_t size_buffer = 0;

// Numero de interrs
static uint8_t num_interrs;

/*
 * Al generarse una interrupcion y seguir habiendo caracteres
 * en el buffer continua enviando el mensaje a la UART0
 */
void continuar_msj(void) {
	U0THR = buffer[index_buffer];
	index_buffer++;
	if (num_interrs == size_buffer) { 										// Hemos terminado resetear para vaciar la buffer
		size_buffer = 0;
		index_buffer = 0;
	}
}

/*
 * RSI de la UART0 
 */
void serial_ISR (void) __irq {
	int mask;
	VICVectAddr = 0;
	
	mask = 0xE; 																						// Nos quedamos con los bits 3:1 que nos dicen que tipo de interr ha sido
	mask &= U0IIR; 																			 		// Interrupt ID register (if read clears the interrupt)
	
	// Recibo datos de UART0 (3:1 = 010) (pg 88)
	if (mask == 0x4) { 																	 		
		  car = U0RBR;																		 		// Receiver Buffer Register (nos da el caracter introducido en la UART)
	} 
	// Escribo datos en UART0 (3:1 = 010) (pg 88)
	else if (mask == 0x2 && num_interrs < size_buffer) { 	
		num_interrs++;	
		continuar_msj();
		// Mandar al planificador evento para que continue el planificador
	}
}

/*
 * Inicializa las interrupciones de la UART0
 */
void init_serial_byInterrupt (void)  {        // Initialize Serial Interface	
  PINSEL0 = 0x5;             									// Enable RxD1 and TxD1
  U0LCR = 0x83;                          			// 8 bits, no Parity, 1 Stop bit
  U0DLL = 97;                            			// Divisor Latch LSB 9600 Baud Rate @ 15MHz VPB Clock
  U0LCR = 0x03;                          			// Line Control Register DLAB = 0
	
	//habilitar interrupciones de uart1
	VICIntEnable = VICIntEnable | 0x00000040; 
	VICVectAddr0 = (unsigned long)serial_ISR;
	VICVectCntl0 = 0x20 | 6;
	U0IER = U0IER | 0x3;												// Interrupt Enable Register, habilitar las RBR (Receiver Buffer Register) y las THR (Transmit Holding Register)
}

/*
 * Realiza una copia del string de entrada y manda el
 * primer caracter a la UART0
 */
void enviar_string(char *string) {
	int i = 0;
	num_interrs = 0;
	
	//Copiar el string a mi 'buffer'
	while (string[i] != '\0') {
		buffer[size_buffer] = string[i];
		size_buffer++;
		i++;
	}
	buffer[size_buffer] = '\0';
	size_buffer++;	
	
	U0THR = buffer[index_buffer]; 							//manda el primer caracter a la UART0
	index_buffer++;
}
