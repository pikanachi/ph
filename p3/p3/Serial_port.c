#include <LPC210x.H>          // LPC21xx definitions
#include <string.h>
#include "Serial_port.h"

#define CR 0x0D
#define MAX_SEND_BUFFER 1500
#define MAX_REC_BUFFER 10

static unsigned int estado = ESPERA_CMD;
static unsigned int fila = 0;
static unsigned int columna = 0;
static unsigned int valor = 0;
static uint8_t jugada = 0;
static uint8_t old_valor = 0;
static uint8_t empezado = 0;

char frase[10];

static char send_buffer[MAX_SEND_BUFFER];
static int index_send_buffer = 0;
static int size_send_buffer = 0;
static uint8_t terminado = 0;

static char rec_buffer[MAX_REC_BUFFER];
static uint8_t index_rec_buffer = 0;
static char msg[MAX_SEND_BUFFER];

/*
 * Actualiza el estado segun el caracter enviado por el usuario a la UART.
 * Si el caracter no pertenece a ningun comenado muestra un mensaje de error.
 */
void actualizar_estado(char c){
	static int csum = 0;
	int era_exec = 0;
	switch(estado){
		case ESPERA_CMD:
			if(c == '#'){
				csum = 0;
				estado = ESPERA_NRNUM;
				U0THR = c;
				index_rec_buffer++;
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_NRNUM:
			if(c == 'N') {
				U0THR = c;
				index_rec_buffer++;
				estado = ESPERA_E;
			} else if((c == 'R' || (c >= '1' && c <= '9')) && empezado){
				U0THR = c;
				index_rec_buffer++;
				if(c == 'R') {
					estado = ESPERA_S;
				} else {
					csum += c - '0';
					estado = ESPERA_COL;
					fila = c - '1';
				}
			} else {
				estado = ESPERA_CMD;
				if (!empezado) {
					UART_enviar_string("\nPrimero debes empezar una partida con #NEW!\nIntroduce tu comando-->\0");
				} else {
					UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
				}
			}
			break;
		case ESPERA_S:
			if(c == 'S'){
				estado = ESPERA_T;
				U0THR = c;
				index_rec_buffer++;
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_T:
			if(c == 'T'){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_E:
			if(c == 'E'){
				estado = ESPERA_W;
				U0THR = c;
				index_rec_buffer++;
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_W:
			if(c == 'W'){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_COL:
			if(c >= '1' && c <= '9'){
				csum += c - '0';
				estado = ESPERA_VAL_C;
				columna = c - '1';
				U0THR = c;
				index_rec_buffer++;
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_VAL_C:
			if ((c >= '0' && c <= '9') || (c == 'C')) {
				U0THR = c;
				index_rec_buffer++;
				if(c == 'C'){
					estado = ESPERA_EXC;
				} else {
					csum += c - '0';
					estado = ESPERA_CHECK;
					valor = c - '0';
				}
			} else {
				estado = ESPERA_CMD;
				UART_enviar_string("\nInstruccion no reconocida\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_CHECK:
			if((csum%8) == (c - '0')){
				estado = ESPERA_EXC;
				U0THR = c;
				index_rec_buffer++;
			} else{
				estado = ESPERA_CMD;
				UART_enviar_string("\nChecksum incorrecta\nIntroduce tu comando-->\0");
			}
			break;
		case ESPERA_EXC:
			if(c == '!'){
				U0THR = c;
			}
			estado = ESPERA_CMD;
			era_exec = 1;
			break;
	}
	if (estado == ESPERA_CMD && !era_exec) {
		memset(&rec_buffer[0], 0, sizeof(rec_buffer)); 		//Clear buffer
		era_exec = 0;
		index_rec_buffer = 0;
	}
}

/*
 * Actualiza el sudoku mostrado en la UART
 */
void UART_actualizar(char *msgFinal){
	int resta = 1;
	int f;
	uint8_t valor;
	char straux[3];
	char aux;
	int i, j;
	msg[0] = '\0';
	msg[1] = 'L';
	msg[2] = 'R';
	msg[2] = '\0';
	strcat(msg,"\n");
	for(i = 0; i < 19; i++){
		if(i%2 == 0){ //Delimitadores
			strcat(msg,"+ - - + - - + - - + - - + - - + - - + - - + - - + - - +\n");
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
				} else {
					aux = '0' + valor;
				}
				straux[0] = ' ';
				straux[1] = aux;
				straux[2] = '\0';
				strcat(msg, straux);
				if (esPista(cuadricula_C_C[f][j])) {
					strcat(msg, " P |");
				} else if(esError(cuadricula_C_C[f][j])) {
					strcat(msg, " E |");
				} else {
					strcat(msg, "   |");
				}
				if(j == 8) {
					strcat(msg, "\n");
				}
			}
		}
	}
	strcat(msg, msgFinal);
	UART_enviar_string(msg);
}

/*
 * Continua enviando el siguiente caracter del string enviado en UART_enviar_string a la UART
 * Una vez que acaba pone terminado a 1
 */
void continuar_msj(void){
	if (index_send_buffer < size_send_buffer){	// Mirar si hemos llagado al final del mensaje
		U0THR = send_buffer[index_send_buffer];
		index_send_buffer++;
	} else {
		terminado = 1;
	}
}

/*
 * Devuelve 1 si se ha terminado de enviar el string a la UART. 0 en caso contrario
 */
uint8_t UART_ha_terminado(void){
	return terminado;
}

/*
 * Escribe por la UART los candidatos de la celda correspondiente a la fila fil y de la columna col.
 * Si la celda seleccionada era una pista devuelve el mensaje "La celda seleccionada 
 * es una pista"
 */
void UART_enviar_candidatos(int fil, int col){
	uint16_t candidatos;
	char msg[50];
	char aux[2];
	int i;
	int mask;
	int cand;
	if(!esPista(cuadricula_C_C[fil][col])){
		strcpy(msg,"\nCandidatos en fila ");
		aux[0] = '0' + fila + 1;
		aux[1] = '\0';
		strcat(msg, aux);
		strcat(msg, " y columna ");
		aux[0] = '0' + columna + 1;
		aux[1] = '\0';
		strcat(msg, aux);
		strcat(msg, ": ");
		candidatos = celda_leer_candidatos(cuadricula_C_C[fil][col]);
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
	strcat(msg,"Introduce tu comando-->");
	UART_enviar_string(msg);
}

/* 
 * Procesa la jugada que el usuario quiere introducir en la celda correspondiente a la fila fil y
 * la columna col. Si la celda seleccionada era una pista devuelve el mensaje "La celda en la que 
 * quieres introducir valor es una pista"
 * Si la juagada es valida, envia el mensaje "¿Confirmar jugada?" la realiza, pone una alarma a 3
 * seg para que el usuario valide la juagada y enciende con otra alarma el led de latido.
 */
void UART_introducir_jugada(int fil, int col){
	char msg2[100];
	if(esPista(cuadricula_C_C[fil][col])){
		strcpy(msg2, "\nLa celda en la que quieres introducir valor es una pista\n");
			UART_enviar_string(msg2);
	} else{
		old_valor = celda_leer_valor(cuadricula_C_C[fil][col]);
		celda_poner_valor(&cuadricula_C_C[fil][col], valor);
		checkError(&cuadricula_C_C[fil][col], old_valor);
		
		//Si ERA candidato, le borro el bit de error
		if (!celda_noEsCandidato(cuadricula_C_C[fil][col], valor)) {
			celda_borrarError(&cuadricula_C_C[fil][col]);
		}
		
		candidatos_actualizar_c(cuadricula_C_C);
		jugada = 1;
		UART_actualizar("¿Confirmar jugada?\n\0");
		
		disable_isr_fiq();
		set_Alarma(No_Confir_Jugada,3000,0);
		set_Alarma(Latido_Validacion,100,1);
		enable_isr_fiq();
	}
}

/*
 * Apaga el led de latido de validación con una alarma con su tiempo a 0
 */
void UART_acaba_jugada(void){
	jugada = 0;
	disable_isr_fiq();
	set_Alarma(Latido_Validacion,0,0);
	enable_isr_fiq();
	if(check_resuelto(cuadricula_C_C)){
		Evento ganar;
		ganar.ID_evento = Terminar;
		ganar.auxData = 1;
		disable_isr_fiq();
		cola_guardar_evento(ganar);
		enable_isr_fiq();
		jugada = 1;
	}
}

/*
 * Si el usuario no valida la jugada, se cancela y muestra el mensaje
 * "Jugada cancelada"
 */
void UART_cancelar_jugada(void){
	celda_poner_valor(&cuadricula_C_C[fila][columna], old_valor);
	checkError(&cuadricula_C_C[fila][columna], old_valor);
	
	// Se vuelve a poner el bit de error si no era candidato
	if (celda_noEsCandidato(cuadricula_C_C[fila][columna], old_valor)) {
		celda_ponerError(&cuadricula_C_C[fila][columna]);
	}
	
	candidatos_actualizar_c(cuadricula_C_C);
	UART_actualizar("Jugada cancelada\nIntroduce tu comando-->\0");
	jugada = 0;
	disable_isr_fiq();
	set_Alarma(Latido_Validacion,0,0);
	enable_isr_fiq();
}

/*
 * RSI de la UART0 
 */
void serial_ISR (void) __irq {
	int mask;
	int retardo;
	
	disable_isr_fiq();
	VICVectAddr = 0;
	mask = 0xE; 																							// Nos quedamos con los bits 3:1 que nos dicen que tipo de interr ha sido
	mask &= U0IIR; 																			 			// Interrupt ID register (if read clears the interrupt)
	
	// Recibo datos de UART0 (3:1 = 010) (man pg 88)
	if (mask == 0x4) {
		if(UART_ha_terminado() && jugada == 0){
			retardo = TIME_PWDN & 0x007FFFFF;     								// Asegurarnos que el retardo es de 23bits
			set_Alarma(Power_Down, retardo, 1);
			if (index_rec_buffer < MAX_REC_BUFFER) {							// Appendear mensaje al buffer si cabe para no irnos de vacas por la memoria
				rec_buffer[index_rec_buffer] = U0RBR;								// Receiver send_buffer RBR Register (nos da el caracter introducido en la UART)
				actualizar_estado(rec_buffer[index_rec_buffer]);	
				if (rec_buffer[index_rec_buffer] == '!') {					// Fin del mensaje
					//hacer lo que sea que diga la accion
					if (strcmp(rec_buffer, "#RST!") == 0) {
						// Terminar partida
						Evento terminar;
						empezado = 0;
						terminar.ID_evento = Terminar;
						cola_guardar_evento(terminar);
					} else if (strcmp(rec_buffer, "#NEW!") == 0) {
						// Nueva partida
						Evento jugada;
						empezado = 1;
						jugada.ID_evento = Start;
						cola_guardar_evento(jugada);	
					} else if (rec_buffer[index_rec_buffer - 1] >= '0' && rec_buffer[index_rec_buffer - 1] <= '9' && 
										 rec_buffer[index_rec_buffer - 2] >= '0' && rec_buffer[index_rec_buffer - 2] <= '9' && 
										 rec_buffer[index_rec_buffer - 3] >= '0' && rec_buffer[index_rec_buffer - 3] <= '9' && 
										 rec_buffer[index_rec_buffer - 4] >= '0' && rec_buffer[index_rec_buffer - 4] <= '9') {
						// Jugada
						Evento jugada;
						jugada.ID_evento = Jugada;
						jugada.auxData = rec_buffer[index_rec_buffer - 4] - '0'; 			//Fila
						jugada.auxData <<= 8;
					  jugada.auxData |= rec_buffer[index_rec_buffer - 3] - '0'; 		//Columna
						cola_guardar_evento(jugada);					
					
					}	else if (rec_buffer[index_rec_buffer - 1] == 'C' && rec_buffer[index_rec_buffer - 2] >= '0' && 
										 rec_buffer[index_rec_buffer - 2] <= '9' && rec_buffer[index_rec_buffer - 3] >= '0' && 
										 rec_buffer[index_rec_buffer - 3] <= '9') {
						// Mostrar candidatos				 
						Evento candidatos;
						candidatos.ID_evento = Candidatos;
						candidatos.auxData = rec_buffer[index_rec_buffer - 3] - '0'; 	//Fila
						candidatos.auxData <<= 8;
					  candidatos.auxData |= rec_buffer[index_rec_buffer - 2] - '0'; //Columna
						cola_guardar_evento(candidatos);
					}	
					memset(&rec_buffer[0], 0, sizeof(rec_buffer)); 									// Clear buffer
					index_rec_buffer = 0;																						// Reiniciar el buffer para leer otro mensaje
				} 
			} else {
				memset(&rec_buffer[0], 0, sizeof(rec_buffer)); 										// Clear buffer
				index_rec_buffer = 0;																							// Hemos llenado el buffer, resetear
			}
		} else{																											
			U0FCR = U0FCR | 0x2;																								//Ignorar el byte escrito para que no vuelva a interrumpir
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
	enable_isr_fiq();
}

/*
 * Inicializa las interrupciones de la UART0
 */
void UART_init_serial(void)  {        							// Initialize Serial Interface	
  PINSEL0 = PINSEL0 | 0x5;             							// Enable RxD1 and TxD1
  U0LCR = 0x83;                          						// 8 bits, no Parity, 1 Stop bit
  U0DLL = 97;                            						// Divisor Latch LSB 9600 Baud Rate @ 15MHz VPB Clock
  U0LCR = 0x03;                          						// Line Control Register DLAB = 0
	
	//habilitar interrupciones de uart1
	VICIntEnable = VICIntEnable | 0x00000040; 
	VICVectAddr5 = (unsigned long)serial_ISR;
	VICVectCntl5 = 0x20 | 6;
	U0IER = U0IER | 0x3;															// Interrupt Enable Register, habilitar las RBR (Receiver send_buffer Register) y las THR (Transmit Holding Register)
	candidatos_actualizar_c(cuadricula_C_C);					// Inicializar la cuadricula
}

/*
 * Manda el string a la UART. Copia el string en un buffer send_buffer y manda el primer caracter
 * del buffer a la UART
 */
void UART_enviar_string(char *string) {
	memset(&send_buffer[0], 0, sizeof(send_buffer)); 	//Wipe previous buffer
	size_send_buffer = 0;
	index_send_buffer = 0;
	terminado = 0;
	
	//Copiar el string a mi 'send_buffer' y controlar que no nos salgamos del limite
	while (string[size_send_buffer] != '\0' && size_send_buffer < MAX_SEND_BUFFER) {
		send_buffer[size_send_buffer] = string[size_send_buffer];
		size_send_buffer++;
	}
	U0THR = send_buffer[index_send_buffer]; 					//manda el primer caracter a la UART0
	index_send_buffer++;
}

/*
 * Borra el tablero y recalcula los candidatos
 */
void UART_borrar_tablero(void){
	borrar_tablero(cuadricula_C_C);
	candidatos_actualizar_c(cuadricula_C_C);
}

void UART_reset(void){
	jugada = 0;
}

int en_jugada(void){
	return jugada;
}
