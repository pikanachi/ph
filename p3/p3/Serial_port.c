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
static unsigned int valor = 0;
static uint8_t jugada = 0;

char frase[10];

// send_buffer
static char send_buffer[MAX_SEND_BUFFER];
static int index_send_buffer = 0;
static int size_send_buffer = 0;
static uint8_t terminado = 0;

static char rec_buffer[MAX_REC_BUFFER];
static uint8_t index_rec_buffer = 0;
static char msg[MAX_SEND_BUFFER];

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
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
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
			} else if(c >= '1' && c <= '9'){
				csum = csum + c - '0';
				estado = ESPERA_COL;
				fila = c - '1';
				U0THR = c;
				index_rec_buffer++;
			} else{
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
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
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
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
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
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
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
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
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_COL:
			if(c >= '1' && c <= '9'){
				csum = csum + c - '0';
				estado = ESPERA_VAL_C;
				columna = c - '1';
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_VAL_C:
			if(c >= '0' && c <= '9'){
				csum = csum + c - '0';
				estado = ESPERA_CHECK;
				U0THR = c;
				valor = c - '0';
				index_rec_buffer++;
			} else if(c == 'C'){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
			} else {
				index_rec_buffer = 0;
				estado = ESPERA_CMD;
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
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
				enviar_string("\nbadCHECKSUM\n");
			}
			break;
		case ESPERA_EXC:
			if(c == '!'){
				U0THR = c;
				//enviar_string("\ngudCMD\n");
			} else{
				enviar_string("\nbadCMD\nIntroduce tu comando-->\0");
			}
			estado = ESPERA_CMD;
			csum = 0;
			break;
	}
}

void cat(char *dest, char *catenado){
	int i, a;
	for(i = 0; dest[i]!= '\0';i++);
	// dest[i] = '\0'
	for(a = 0; catenado[a] != '\0'; a++){
		dest[i] = catenado[a];
		i++;
	}
	dest[i] = '\0';
}

void actualizar_uart(char *msgFinal){
	int resta = 1;
	//char string[MAX_REC_BUFFER];
	int f;
	uint8_t valor;
	char straux[3];
	char aux;
	int i, j;
	msg[0] = '\0';
	msg[1] = 'L';
	msg[2] = 'R';
	msg[2] = '\0';
	cat(msg,"\n");
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
			*/cat(msg,"+ - - + - - + - - + - - + - - + - - + - - + - - + - - +\n");
		} else{
			f = i - resta;
			resta++;
			for(j = 0; j < 9; j++){
				if(j == 0){
					strcat(msg,"|");
				}
				
				valor = celda_leer_valor(cuadricula_C_C[f][j]);
				if(valor == 0){
					aux = ' ';
				} else{
					aux = '0' + valor;
				}
				straux[0] = ' ';
				straux[1] = aux;
				straux[2] = '\0';
				cat(msg, straux);
				if (esPista(cuadricula_C_C[f][j])){
					strcat(msg, " P |");
				} else if(esError(cuadricula_C_C[f][j])){
					strcat(msg, " E |");
				} else{
					strcat(msg, "   |");
				}
				//enviar_string("+ V PE +");
				if(j == 8){
					strcat(msg, "\n");
				}
			}
		}
	}
	strcat(msg, msgFinal);
	strcat(msg, "\0TERMINO AQUI");
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

void enviar_candidatos(void){
	uint16_t candidatos;
	char msg[50];
	char aux[2];
	int i;
	int mask;
	int cand;
	if(!esPista(cuadricula_C_C[fila][columna])){
		strcpy(msg,"\nCandidatos en fila ");
		aux[0] = '0' + fila;
		aux[1] = '\0';
		strcat(msg, aux);
		strcat(msg, " y columna ");
		aux[0] = '0' + columna;
		aux[1] = '\0';
		strcat(msg, aux);
		strcat(msg, ": ");
		candidatos = celda_leer_candidatos(cuadricula_C_C[fila][columna]);
		mask = 1;
		for(i = 1; i <= 9; i++){
			cand = candidatos & mask;
			if(cand  == 0){
				aux[0] = '0' + i;
				aux[1] = '\0';
				strcat(msg, aux);
				strcat(msg, " ");
			}
			mask = mask * 2;
		}
		strcat(msg, "\n");
	} else{
		strcpy(msg, "\nLa celda seleccionada es una pista\n");
	}
	strcat(msg,"Introduce tu comando-->\0");
	enviar_string(msg);
}

void introducir_jugada(void){
	//int tiempo;
	char msg2[100];
	if(esPista(cuadricula_C_C[fila][columna])){
		strcpy(msg2, "\nLa celda en la que quieres introducir valor es una pista\n");
			enviar_string(msg2);
	} else{
		celda_poner_valor(&cuadricula_C_C[fila][columna], valor);
				checkError(&cuadricula_C_C[fila][columna]);
		candidatos_actualizar_c(cuadricula_C_C);
		jugada = 1;
		actualizar_uart("¿Confirmar jugada?\n\0");
		set_Alarma(No_Confir_Jugada,3000,0);
		set_Alarma(Latido_Validacion,100,1);
	}
}

void acaba_jugada(void){
	jugada = 0;
	set_Alarma(Latido_Validacion,0,0);
}

void cancelar_jugada(void){
	celda_poner_valor(&cuadricula_C_C[fila][columna], 0);
	checkError(&cuadricula_C_C[fila][columna]);
	candidatos_actualizar_c(cuadricula_C_C);
	actualizar_uart("Jugada cancelada\nIntroduce tu comando-->\0");
	jugada = 0;
	set_Alarma(Latido_Validacion,0,0);
}

/*
 * RSI de la UART0 
 */
void serial_ISR (void) __irq {
	int mask;
	int retardo;
	VICVectAddr = 0;
	
	mask = 0xE; 																						// Nos quedamos con los bits 3:1 que nos dicen que tipo de interr ha sido
	mask &= U0IIR; 																			 		// Interrupt ID register (if read clears the interrupt)
	
	// Recibo datos de UART0 (3:1 = 010) (pg 88)
	if (mask == 0x4) {
		if(ha_terminado() && jugada == 0){
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
					} else if (rec_buffer[index_rec_buffer - 1] >= '0' && rec_buffer[index_rec_buffer - 1] <= '9' && rec_buffer[index_rec_buffer - 2] >= '0' && rec_buffer[index_rec_buffer - 2] <= '9'  && rec_buffer[index_rec_buffer - 3] >= '0' && rec_buffer[index_rec_buffer - 3] <= '9' && rec_buffer[index_rec_buffer - 4] >= '0' && rec_buffer[index_rec_buffer - 4] <= '9') {
						introducir_jugada();
					}	else if (rec_buffer[index_rec_buffer - 1] == 'C' && rec_buffer[index_rec_buffer - 2] >= '0' && rec_buffer[index_rec_buffer - 2] <= '9'  && rec_buffer[index_rec_buffer - 3] >= '0' && rec_buffer[index_rec_buffer - 3] <= '9') {
						enviar_candidatos();
					}	
					rec_buffer[index_rec_buffer] = '.';
					index_rec_buffer = 0;				// Reiniciar el buffer para leer otro mensaje
				} 
			} else {	
				index_rec_buffer = 0;																// Hemos llenado el buffer, resetear
			}
		} else{
			char aux;
			aux = U0RBR;								// Receiver send_buffer RBR Register (nos da el caracter introducido en la UART)
			continuar_msj();
		}
	} 
	// Escribo datos en UART0 (3:1 = 010) (pg 88)
	else if (mask == 0x2) { 	
		continuar_msj();
		if (terminado) {
			// Mandar al planificador evento para que continue el planificador
		}
	}
	retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
	set_Alarma(Power_Down, retardo, 1);
}

/*
 * Inicializa las interrupciones de la UART0
 */
void init_serial_byInterrupt (void)  {        // Initialize Serial Interface	
  PINSEL0 = PINSEL0 | 0x5;             									// Enable RxD1 and TxD1
  U0LCR = 0x83;                          			// 8 bits, no Parity, 1 Stop bit
  U0DLL = 97;                            			// Divisor Latch LSB 9600 Baud Rate @ 15MHz VPB Clock
  U0LCR = 0x03;                          			// Line Control Register DLAB = 0
	
	//habilitar interrupciones de uart1
	VICIntEnable = VICIntEnable | 0x00000040; 
	VICVectAddr5 = (unsigned long)serial_ISR;
	VICVectCntl5 = 0x20 | 6;
	U0IER = U0IER | 0x3;												// Interrupt Enable Register, habilitar las RBR (Receiver send_buffer Register) y las THR (Transmit Holding Register)
	candidatos_actualizar_c(cuadricula_C_C);
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
