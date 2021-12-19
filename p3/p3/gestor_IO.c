#include "gestor_IO.h"
#include "idle_pwdwn.h"

#define TIME_CHECK_IO 200
#define LATIDO 125
#define TIME_VALI_LED 1000

 
static uint8_t old_fila = 10;
static uint8_t old_columna = 10;

/*
 * Inicializa el gestor_IO iniciclizando la GPIO y pone una alarma periodica
 * que Check_Entrada para mostrar en el GPIO el valor de la celda seleccionada
 * (valor y candidatos)
 */
void gIO_inicializar(void) {
	int retardo;
  GPIO_iniciar();
	retardo = TIME_CHECK_IO & 0x007FFFFF;     				// Asegurarnos que el retardo es de 23bits
	disable_isr_fiq();
	set_Alarma(Check_Entrada, retardo, 1);
	enable_isr_fiq();
	retardo = LATIDO & 0x007FFFFF;     				// Asegurarnos que el retardo es de 23bits
	disable_isr_fiq();
	set_Alarma(Latido, retardo, 1);
	enable_isr_fiq();

}

void gIO_reset() {
	int retardo;
	retardo = TIME_CHECK_IO & 0x007FFFFF;     				// Asegurarnos que el retardo es de 23bits
	disable_isr_fiq();
	set_Alarma(Check_Entrada, retardo, 1);
	enable_isr_fiq();
	retardo = LATIDO & 0x007FFFFF;     				// Asegurarnos que el retardo es de 23bits
	disable_isr_fiq();
	set_Alarma(Latido, retardo, 1);
	enable_isr_fiq();	
}

/*
 * Muestra el valor de los candidatos de la celda seleccionada (fila y columna) 
 * en el GPIO
 */
void gIO_mostrar_candidatos(void){
	uint16_t candidatos;
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	candidatos = celda_leer_candidatos(cuadricula_C_C[fila][columna]);
	GPIO_escribir(4,9,candidatos);
}

/*
 * Muestra el valor de la celda seleccionada (fila y columna) en el GPIO
 */
void gIO_mostrar_valor(void){
	uint8_t valor;
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	valor = celda_leer_valor(cuadricula_C_C[fila][columna]);
	GPIO_escribir(0,4,valor);
}

/*
 * Escribe el bit de overfow del GPIO para saber si ha desbordado la
 * cola de eventos
 */
void gIO_encender_overflow(void){
	GPIO_escribir(30,1,1);
}

/*
 * Lee el estado del bit de overfow del GPIO para saber si ha desbordado la
 * cola de eventos
 */
int gIO_leer_overflow(void){
	return GPIO_leer(30,1);
}

/*
 * Si la fila y la columna introducidas por el usuario es invalida (>9)
 * se enciende el led de validación (entrada invaida) sino si no tiene valor
 * la celda se enciende el led de validacion. Finalmente se muestra el
 * valor de la casilla seleccionada y sus candidatos en el GPIO
 */
void gIO_check_entrada(void){
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	if(fila > 9 || columna > 9){
		gIO_encender_validacion();
	}
	else{
		if(!(old_fila == fila && old_columna == columna)){
			int retardo;
			gIO_apagar_validacion();
			if(!esPista(cuadricula_C_C[fila][columna])){
				gIO_apagar_validacion();
			}
			old_fila = fila;
			old_columna = columna;
			gIO_mostrar_valor();
			gIO_mostrar_candidatos();
			//---------------------------------------------
			//Actualizar contador para PWDOWN
			retardo = TIME_PWDN & 0x007FFFFF;     						// Asegurarnos que el retardo es de 23bits
			enable_isr_fiq();
			set_Alarma(Power_Down, retardo, 1);
			disable_isr_fiq();
		}
	}
}

/*
 * Si la fila y la columna introducidas por el usuario es invalida (>9)
 * se enciende el led de validación (entrada invaida) sino, si el valir introcucido
 * por el usuario es válido (>= 1 y <= 9) y es uno de los posibles candidatos de la 
 * celda,se enciende el led de validación durante 1000ms, se pone el valor introducido
 * en la celda seleccionada, se actualizan los candidatos y se muestran en el GPIO los
 * candidatos y el valor introducidos
 */
void gIO_escribir_entrada(void){
	int tiempo;
	uint8_t valor;
	int fila, columna;
	tiempo = temporizador_leer();
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	if(fila > 9 || columna > 9){
		gIO_encender_validacion();
	}
	else{
		valor = GPIO_leer(24,4);
		if(fila == 0 && columna == 0 && valor == 0){
			Evento eTerminar;
			gIO_encender_validacion();
			eTerminar.ID_evento = Terminar;
			eTerminar.timestamp = temporizador_leer() / 1000;
			disable_isr_fiq();
			cola_guardar_evento(eTerminar);
			enable_isr_fiq();
		}
		else{
			if(valor >= 1 && valor  <= 9){
				if(esPista(cuadricula_C_C[fila][columna])){
					gIO_encender_validacion();
				}
				else{
					if(!celda_noEsCandidato(cuadricula_C_C[fila][columna],valor)){
						int retardo;
						gIO_encender_validacion();
						retardo = TIME_VALI_LED & 0x007FFFFF;     							// Asegurarnos que el retardo es de 23bits
						disable_isr_fiq();
						set_Alarma(Apagar_Validacion, retardo, 0);
						enable_isr_fiq();
						celda_poner_valor(&cuadricula_C_C[fila][columna], valor);
						candidatos_actualizar_c(cuadricula_C_C);
						gIO_mostrar_candidatos();
						gIO_mostrar_valor();
						tiempo = temporizador_leer() - tiempo;
					}
				}
			}
		}
	}
}

void gIO_eliminar_valor(void){
	int fila, columna;
	fila = GPIO_leer(16,4);
	columna = GPIO_leer(20,4);
	if(fila > 9 || columna > 9){
		gIO_encender_validacion();
	}
	else{
		if(!esPista(cuadricula_C_C[fila][columna])){
			int retardo;
			gIO_encender_validacion();
			retardo = TIME_VALI_LED & 0x007FFFFF;     										// Asegurarnos que el retardo es de 23bits
			disable_isr_fiq();
			set_Alarma(Apagar_Validacion, retardo, 0); 
			enable_isr_fiq();
			celda_poner_valor(&cuadricula_C_C[fila][columna], 0);
			candidatos_actualizar_c(cuadricula_C_C);
			gIO_mostrar_candidatos();
			gIO_mostrar_valor();
		}
	}
}

void gIO_borrar_tablero(void){
	borrar_tablero(cuadricula_C_C);
	candidatos_actualizar_c(cuadricula_C_C);
}

/*
 * Apaga el led de validación (bit 13 GPIO)
 */
void gIO_apagar_validacion(void){
	GPIO_escribir(13,1,0);
}

/*
 * Enciende el led de validación (bit 13 GPIO)
 */
void gIO_encender_validacion(void){
	GPIO_escribir(13,1,1);
}

/*
 * Enciende el bit 31 del GPIO
 */
void gIO_encender_latido(void){
	GPIO_escribir(31,1,1);
}

/*
 * Apaga el bit 31 del GPIO
 */
void gIO_apagar_latido(void){
	GPIO_escribir(31,1,0);
}


/*
 * Alterna el latido del GPIO encendiendo el bit 31 
 * si estaba apagado y apagandolo si estaba encendido
 */
void gIO_alternar_latido(void){
	int l31 = GPIO_leer(31,1);
	GPIO_escribir(31,1,(l31+1)%2);
}

void gIO_alternar_validacion(void){
	int l31 = GPIO_leer(13,1);
	GPIO_escribir(13,1,(l31+1)%2);
}
