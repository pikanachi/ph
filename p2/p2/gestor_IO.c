#include "gestor_IO.h"

/*
 * Inicializa la GPIO
 */
 
#define TIME_IO 200
 
static uint8_t old_fila = 10;
static uint8_t old_columna = 10;



 
void gIO_inicializar(void) {
	int retardo;
	Evento eAlarma;
  GPIO_iniciar();
	candidatos_actualizar_c(cuadricula_C_C);
	retardo = TIME_IO & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
	eAlarma.ID_evento = Set_Alarma;
	eAlarma.auxData = Check_Entrada;  							// ID evento a generar
	eAlarma.auxData = eAlarma.auxData << 1;
	eAlarma.auxData = eAlarma.auxData | 1;          	// Es periódica
	eAlarma.auxData = eAlarma.auxData << 23;
	eAlarma.auxData = eAlarma.auxData | retardo;
	eAlarma.timestamp = temporizador_leer() / 1000;
	cola_guardar_evento(eAlarma); 
}



void gIO_mostrar_candidatos(void){
	uint16_t candidatos;
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	candidatos = celda_leer_candidatos(cuadricula_C_C[fila][columna]);
	GPIO_escribir(4,9,candidatos);
}

void gIO_mostrar_valor(void){
	uint8_t valor;
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	valor = celda_leer_valor(cuadricula_C_C[fila][columna]);
	GPIO_escribir(0,4,valor);
}

void gIO_encender_overflow(void){
	GPIO_escribir(30,1,1);
}

int gIO_leer_overflow(void){
	return GPIO_leer(30,1);
}

void gIO_check_entrada(void){
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	if(fila > 9 || columna > 9){
		gIO_encender_validacion();
	}
	else{
		if(!(old_fila == fila && old_columna == columna)){
			gIO_apagar_validacion();
			if(esPista(cuadricula_C_C[fila][columna])){
				gIO_encender_validacion();
			}
			old_fila = fila;
			old_columna = columna;
			gIO_mostrar_valor();
			gIO_mostrar_candidatos();
		}
	}
}

void gIO_escribir_entrada(void){
	uint8_t valor;
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	if(fila > 9 || columna > 9){
		gIO_encender_validacion();
	}
	else{
		valor = GPIO_leer(24,4);
		if(valor >= 1 && valor  <= 9){
			if(!celda_noEsCandidato(cuadricula_C_C[fila][columna],valor)){
				int retardo;
				Evento eAlarma;
				gIO_encender_validacion();
				retardo = 1000 & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
				eAlarma.ID_evento = Set_Alarma;
				eAlarma.auxData = Apagar_Validacion;  							// ID evento a generar
				eAlarma.auxData = eAlarma.auxData << 1;
				eAlarma.auxData = eAlarma.auxData & 0;          	// Es periódica
				eAlarma.auxData = eAlarma.auxData << 23;
				eAlarma.auxData = eAlarma.auxData | retardo;
				eAlarma.timestamp = temporizador_leer() / 1000;
				cola_guardar_evento(eAlarma); 
				celda_poner_valor(&cuadricula_C_C[fila][columna], valor);
				candidatos_actualizar_c(cuadricula_C_C);
				gIO_mostrar_candidatos();
				gIO_mostrar_valor();
			}
		}
	}
}

void gIO_apagar_validacion(void){
	GPIO_escribir(13,1,0);
}

void gIO_encender_validacion(void){
	GPIO_escribir(13,1,1);
}
