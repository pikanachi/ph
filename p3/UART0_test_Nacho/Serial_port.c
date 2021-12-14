#include <LPC210x.H>          // LPC21xx definitions
#include <string.h>
#include "Serial_port.h"

#define CR     		0x0D
#define MAX_SEND_BUFFER 	1000
#define MAX_REC_BUFFER 	10

static unsigned int estado = ESPERA_CMD;
static unsigned int csum = 0;

char frase[10];

// send_buffer
static char send_buffer[MAX_SEND_BUFFER];
static uint8_t index_send_buffer = 0;
static uint8_t size_send_buffer = 0;
static uint8_t terminado = 0;

static char rec_buffer[MAX_REC_BUFFER];
static uint8_t index_rec_buffer = 0;

void actualizar_estado(char c){
	switch(estado){
		case ESPERA_CMD:
			if(c == '#'){
				estado = ESPERA_NRNUM;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_NRNUM:
			if(c == 'N'){
				estado = ESPERA_E;
				U0THR = c;
				index_rec_buffer++;
			} else if(c == 'R') {
				estado = ESPERA_S;
				U0THR = c;
				index_rec_buffer++;
			} else if(c >= '0' && c <= '9'){
				csum = csum + c - '0';
				estado = ESPERA_COL;
				U0THR = c;
				index_rec_buffer++;
			} else{
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_S:
			if(c == 'S'){
				estado = ESPERA_T;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_T:
			if(c == 'T'){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_E:
			if(c == 'E'){
				estado = ESPERA_W;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_W:
			if(c == 'W'){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_COL:
			if(c >= '0' && c <= '9'){
				csum = csum + c - '0';
				estado = ESPERA_VAL;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_VAL:
			if(c >= '0' && c <= '9'){
				csum = csum + c - '0';
				estado = ESPERA_CHECK;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_CHECK:
			if((csum%8) == (c - '0')){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
				csum = 0;
			} else{
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\n");
			}
			break;
		case ESPERA_EXC:
			if(c == '!'){
				U0THR = c;
				enviar_string("\ngudCMD\n");
			} else{
				enviar_string("\nbadCMD\n");
			}
			estado = ESPERA_CMD;
			break;
	}
}

void continuar_msj(void){
	if (index_send_buffer < size_send_buffer){						// Mirar si hemos llagado al final del mensaje
		U0THR = send_buffer[index_send_buffer];
		index_send_buffer++;
	}
	//if (index_send_buffer >= size_send_buffer) {
	else {
		terminado = 1;
	}
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
		if (index_rec_buffer < MAX_REC_BUFFER) {							// Appendear mensaje al buffer si cabe para no irnos de vacas por la memoria
			rec_buffer[index_rec_buffer] = U0RBR;								// Receiver send_buffer RBR Register (nos da el caracter introducido en la UART)
			actualizar_estado(rec_buffer[index_rec_buffer]);	
			if (rec_buffer[index_rec_buffer] == '!') {					// Fin del mensaje
				//hacer lo que sea que diga la accion
				if (strcmp(rec_buffer, "#RST!") == 0) {
					// Terminar partida
					strcpy(frase, "terminar");
				} else if (strcmp(rec_buffer, "#NEW!") == 0) {
					// Nueva partida
					strcpy(frase, "nueva");
				} else if (strcmp(rec_buffer, "#FCVS!") == 0) {
					// Jugada
					strcpy(frase, "jugada");
				}	
			index_rec_buffer = 0;				// Reiniciar el buffer para leer otro mensaje
			} 
		} else {	
			index_rec_buffer = 0;																// Hemos llenado el buffer, resetear
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
	VICVectAddr5 = (unsigned long)serial_ISR;
	VICVectCntl5 = 0x20 | 6;
	U0IER = U0IER | 0x3;												// Interrupt Enable Register, habilitar las RBR (Receiver send_buffer Register) y las THR (Transmit Holding Register)
}

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


