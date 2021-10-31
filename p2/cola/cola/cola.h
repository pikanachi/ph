#ifndef COLA_H
#define COLA_H

#include "../evento/evento.h"
#include <stdint.h> // Para los uint
#include <stddef.h> // Para NULL

#define MAX_EVENTOS 32

typedef struct Cola Cola;

struct Cola {
    Evento  eventos[MAX_EVENTOS];
    uint8_t indPrimEv;      // Índice del evento más antiguo de la cola
    uint8_t indProxEv;      // Índice del siguiente evento a encolar
    uint8_t numEventos;
};

/*
 * Crea la cola circular vacía, es decir, sin Evento.
 */
void cola_crear_vacia(void);


/*
 * Si numEventos < maxEventos devuelve la cola circular resultante de añadir a c
 * el Evento e. Si no, devuelve la cola resultante de añadir a c el Evento e, eliminando
 * el Evento más antiguo de la cola, activando el flag V (Overflow).
 */
void cola_guardar_evento(Evento e);


/* 
 * Si numEventos > 0, devuelve el Evento más antiguo de la cola (el primero) y devuelve la
 * cola circular resultante de eliminar de la cola dicho Evento. En caso contrario, devuelve
 * un Evento vacío.
 */
Evento cola_desencola_mas_antiguo(void);


/*
 * Devuelve 1 (verdad) sí y sólo si la cola c no tiene ningún Evento.
 * devuelve 0 (falso) en caso contrario.
 */
uint8_t cola_es_vacia(void);


/*
 * Si numEventos > 0, devuelve el Evento más nuevo encolado en la cola (el último). En caso contrario, devuelve
 * un evento vacío.
 */
Evento cola_ultimo(void);


/*
 * Activa el Flag V que indica que ha ocurrido un desbordamiento.
 */
extern void actualizarFV();


//void imp_cola();
//void actualizarFV();

#endif
