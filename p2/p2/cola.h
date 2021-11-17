#ifndef COLA_H
#define COLA_H

#include <stdint.h> // Para los uint
#include "evento.h"

#define MAX_EVENTOS 32

typedef struct Cola Cola;

struct Cola {
    Evento  eventos[MAX_EVENTOS];
    uint8_t indPrimEv;      // �ndice del evento m�s antiguo de la cola
    uint8_t indProxEv;      // �ndice del siguiente evento a encolar
    uint8_t numEventos;
};

/*
 * Crea la cola circular vac�a, es decir, sin Evento.
 */
void cola_crear_vacia(void);


/*
 * Si numEventos < maxEventos devuelve la cola circular resultante de a�adir a c
 * el Evento e. Si no, devuelve la cola resultante de a�adir a c el Evento e, eliminando
 * el Evento m�s antiguo de la cola, activando el flag V (Overflow).
 */
void cola_guardar_evento(Evento e);


/* 
 * Si numEventos > 0, devuelve el Evento m�s antiguo de la cola (el primero) y devuelve la
 * cola circular resultante de eliminar de la cola dicho Evento. En caso contrario, devuelve
 * un Evento vac�o.
 */
Evento cola_desencola_mas_antiguo(void);


/*
 * Devuelve 1 (verdad) s� y s�lo si la cola c no tiene ning�n Evento.
 * devuelve 0 (falso) en caso contrario.
 */
uint8_t cola_es_vacia(void);


/*
 * Si numEventos > 0, devuelve el Evento m�s nuevo encolado en la cola (el �ltimo). En caso contrario, devuelve
 * un evento vac�o.
 */
Evento cola_ultimo(void);


/*
 * Activa el Flag V que indica que ha ocurrido un desbordamiento.
 */
extern void actualizarFV(void);

void vaciar_cola(void);


//void imp_cola();
//void actualizarFV();

#endif
