#include "gestor_IO.h"
#include "idle_pwdwn.h"

#define LATIDO 125

/*
 * Inicializa el gestor_IO iniciclizando la GPIO y pone una alarma periodica
 * que Check_Entrada para mostrar en el GPIO el valor de la celda seleccionada
 * (valor y candidatos)
 */
void gIO_inicializar(void) {
	int retardo;
  GPIO_iniciar();
	retardo = LATIDO & 0x007FFFFF;     									// Asegurarnos que el retardo es de 23bits
	disable_isr_fiq();
	set_Alarma(Latido, retardo, 1);
	enable_isr_fiq();
}

/*
 * Vuelve a poner una alarma periodica para el latido tras resetear una partida
 */
void gIO_reset() {
	int retardo;
	retardo = LATIDO & 0x007FFFFF;     									// Asegurarnos que el retardo es de 23bits
	disable_isr_fiq();
	set_Alarma(Latido, retardo, 1);
	enable_isr_fiq();	
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

/*
 * Alterna la validacion del GPIO encendiendo el bit 13 
 * si estaba apagado y apagandolo si estaba encendido
 */
void gIO_alternar_validacion(void){
	int l31 = GPIO_leer(13,1);
	GPIO_escribir(13,1,(l31+1)%2);
}
