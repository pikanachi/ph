#include <LPC210x.H>          // LPC21xx definitions
#include <string.h>
#include "Serial_port.h"

#define CR     		0x0D
#define MAX_SEND_BUFFER 	1500
#define MAX_REC_BUFFER 	10

static unsigned int estado = ESPERA_CMD;
static unsigned int csum = 0;
static unsigned int fila = 0;
static unsigned int columna = 0;

char frase[10];

// send_buffer
static char send_buffer[MAX_SEND_BUFFER];
static uint8_t index_send_buffer = 0;
static int size_send_buffer = 0;
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
			} else if(c == 'i' || c == 'I'){
				fila = (fila - 1)%8;
			} 
			else if(c == 'j' || c == 'J'){
				columna = (columna - 1)%8;
			} 
			else if(c == 'k' || c == 'K'){
				fila = (fila + 1)%8;
			} 
			else if(c == 'l' || c == 'L'){
				columna = (columna + 1)%8;
			} 
			else {
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

void actualizar_uart(void){
	int resta = 1;
	char msg[MAX_SEND_BUFFER];
	int i, j;
	strcpy(msg,"");
	for(i = 0; i < 19; i++){
		if(i%2 == 0){//Delimitadores
			/*
			enviar_string("+ - - +");
			for(j = 0; j < 8; j++){
				if(j == 7){
					enviar_string(" - - +\n");
				} else{
					enviar_string(" - - +");
				}
			}
			*/strcat(msg,"+ - - + - - + - - + - - + - - + - - + - - + - - + - - +\n");
		} else{
			int f;
			uint8_t valor;
			char string[MAX_SEND_BUFFER];
			char straux[2];
			char aux;
			f = i - resta;
			resta++;
			for(j = 0; j < 9; j++){
				if(j == 0){
					strcat(string,"|");
				}
				
				valor = celda_leer_valor(cuadricula_C_C[f][j]);
				aux = '0' + valor;
				straux[0] = ' ';
				straux[1] = aux;
				strcat(string, straux);
				if (esPista(cuadricula_C_C[f][j])){
					strcat(string, " P |");
				} else if(esError(cuadricula_C_C[f][j])){
					strcat(string, " E |");
				} else{
					strcat(string, "   |");
				}
				//enviar_string("+ V PE +");
				if(j == 8){
					strcat(msg, strcat(string, "\n"));
				} else{
					strcat(msg,string);
				}
				strcpy(string,"");
			}
		}
	}
	enviar_string(msg);
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
	size_send_buffer = 0;
	index_send_buffer = 0;
	terminado = 0;
	
	//Copiar el string a mi 'send_buffer' y controlar que no nos salgamos del limite
	while (string[size_send_buffer] != '\0' && size_send_buffer < MAX_SEND_BUFFER) {
		send_buffer[size_send_buffer] = string[size_send_buffer];
		size_send_buffer++;
	}
	U0THR = send_buffer[index_send_buffer]; 							//manda el primer caracter a la UART0
	index_send_buffer++;
}
