#include "cola.h"
#include "gestor_IO.h"
#include "softI.h"
#include <string.h>


static Cola c;  //static para que no la lie al compilar con otras globales (otra var con el mismo nombre en otro c)

/*
 * Crea la cola circular vac�a, es decir, sin Evento.
 */
void cola_crear_vacia(void) {
	c.numEventos = 0;
	c.indProxEv = 0;
	c.indPrimEv = 0;
}


/*
 * Si numEventos < maxEventos devuelve la cola circular resultante de a�adir a c
 * el Evento e. Si no, devuelve la cola resultante de a�adir a c el Evento e, eliminando
 * el Evento m�s antiguo de la cola, activando el flag V (Overflow).
 */
void cola_guardar_evento(Evento e) {
	//disable_isr_fiq();
	c.eventos[c.indProxEv] = e;

	if (!cola_es_vacia() && c.indPrimEv == c.indProxEv) {   //Overflow
		c.indPrimEv = (c.indPrimEv + 1) % MAX_EVENTOS;
		//OVERFLOW
		gIO_encender_overflow();
	}else{
		c.numEventos++;
	}
	c.indProxEv = (c.indProxEv + 1) % MAX_EVENTOS;
	//enable_isr_fiq();
}


/* 
 * Si numEventos > 0, devuelve el Evento m�s antiguo de la cola (el primero) y devuelve la
 * cola circular resultante de eliminar de la cola dicho Evento. En caso contrario, devuelve
 * un Evento inv�lido.
 */
Evento cola_desencola_mas_antiguo(void) {
	Evento e;
	disable_isr_fiq();
	e = el_invalido();
	if(!cola_es_vacia()) {
		e = c.eventos[c.indPrimEv];
		c.indPrimEv = (c.indPrimEv + 1) % MAX_EVENTOS;
		c.numEventos--;
	}
	enable_isr_fiq();
	return e;
}


/*
 * Devuelve 1 (verdad) s� y s�lo si la cola c no tiene ning�n Evento.
 * devuelve 0 (falso) en caso contrario.
 */
uint8_t cola_es_vacia(void) {
	return c.numEventos == 0;
}


/*
 * Si numEventos > 0, devuelve el Evento m�s nuevo encolado en la cola (el �ltimo). En caso contrario, devuelve
 * un evento inv�lido.
 */
Evento cola_ultimo(void) {
	Evento e = eventoInvalido;
	if (!cola_es_vacia()) {
		if (c.indProxEv-1 < 0) {  //Es el �ltimo
			e = c.eventos[MAX_EVENTOS-1];
		} else {
			e = c.eventos[c.indProxEv-1];
		}
	}
	return e;
}


/*
 * Llama a crearVacia
 */
void vaciar_cola(void){
	memset(&c.eventos[0], 0, sizeof(c));
	cola_crear_vacia();
}
