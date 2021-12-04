#include <LPC210x.H>          // LPC21xx definitions
#include <string.h>
#include "Serial_port.h"

#define CR     		0x0D
#define MAX_SEND_BUFFER 	10
#define MAX_REC_BUFFER 	10

char car;

// send_buffer
static char send_buffer[MAX_SEND_BUFFER];
static uint8_t index_send_buffer = 0;
static uint8_t size_send_buffer = 0;
static uint8_t terminado = 1;

static char rec_buffer[MAX_REC_BUFFER];
static uint8_t index_rec_buffer = 0;

/*
 * Al generarse una interrupcion y seguir habiendo caracteres
 * en el send_buffer continua enviando el mensaje a la UART0
 */
/*void continuar_msj(void) {
	if (size_send_buffer > 0){
		U0THR = send_buffer[index_send_buffer_read];
		index_send_buffer_read = (index_send_buffer_read + 1) % MAX_SEND_BUFFER;
		size_send_buffer--;
	}else
		terminado = 1;
}*/

void continuar_msj(void){
	if (index_send_buffer < size_send_buffer){
		U0THR = send_buffer[index_send_buffer];
		index_send_buffer++;
	}else
		terminado = 1;
}

uint8_t ha_terminado(void){
	return terminado;
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
		// Appendear mensaje al buffer  
		//car = U0RBR;																		 		// Receiver send_buffer Register (nos da el caracter introducido en la UART)
		rec_buffer[index_rec_buffer] = U0RBR;
		if (rec_buffer[index_rec_buffer] == '!') {
			//hacer lo que sea que diga la accion
			
			//reiniciar el buffer para leer otro mensaje
			index_rec_buffer = 0;
		} else {
			index_rec_buffer++;
		}
	} 
	// Escribo datos en UART0 (3:1 = 010) (pg 88)
	else if (mask == 0x2) { 	
		continuar_msj();
		if (terminado) {
			// Mandar al planificador evento para que continue el planificador
		}
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
	U0IER = U0IER | 0x3;												// Interrupt Enable Register, habilitar las RBR (Receiver send_buffer Register) y las THR (Transmit Holding Register)
}

/*
 * Realiza una copia del string de entrada y manda el
 * primer caracter a la UART0
 */
/*
void enviar_string(char *string) {
	int i = 0;
	num_interrs = 0;
	
	//Copiar el string a mi 'send_buffer'
	while (string[i] != '\0' && size_send_buffer < MAX_SEND_BUFFER) {
		send_buffer[index_send_buffer_write] = string[i];
		index_send_buffer_write = (index_send_buffer_write + 1) % MAX_SEND_BUFFER;
		size_send_buffer++;
		i++;
	}
	//send_buffer[size_send_buffer] = '\0';
	//size_send_buffer++;	
	if (terminado){
		terminado = 0;
		U0THR = send_buffer[index_send_buffer_read]; 							//manda el primer caracter a la UART0
		index_send_buffer_read = (index_send_buffer_read + 1) % MAX_SEND_BUFFER;
		size_send_buffer--;
	}
}*/

void enviar_string(char *string) {
	index_send_buffer = 0;
	size_send_buffer = 0;
	terminado = 0;
	
	//Copiar el string a mi 'send_buffer' y controlar que no nos salgamos del limite
	while (string[size_send_buffer] != '\0' && size_send_buffer < MAX_SEND_BUFFER) {
		send_buffer[size_send_buffer] = string[size_send_buffer];
		size_send_buffer++;
	}

	U0THR = send_buffer[index_send_buffer]; 							//manda el primer caracter a la UART0
	index_send_buffer++;
}

