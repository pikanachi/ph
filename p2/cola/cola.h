#ifndef COLA_H
#define COLA_H

#include "evento.h"
#include <stdint.h> // Para los uint
#include <stddef.h> // Para NULL

/*
 * Los valores del TAD Cola representan colas circulares de Eventos en las que el primer Evento de 
 * la cola apunta al último y el último a la cola, encolados como una cola es una cola FIFO, es decir, 
 * el primer Evento en ser encolado es también el primer Evento en ser desencolado. 
 * La cola circular tiene un número máximo de typedef struct Celda Celda;, de tal manera que si se llega al número
 * máximo de Eventos encolados, se elimina el Evento más antiguo (el primero) y se encola 
 * al final el nuevo Evento.
 */

typedef struct Celda Celda;
typedef struct Cola Cola;

struct Cola {
    struct Celda {
        Evento  e;
        Celda  *sig;
        Celda  *ant;
    };
    Celda       *prim;
    Celda       *ult;
    uint8_t     numEventos;
    uint8_t     maxEventos;
};

/*
 * Crea la cola circular vacía, es decir, sin Evento.
 */
void cola_crear_vacia(uint8_t max, Cola *c);

/*
 * Si numEventos < maxEventos devuelve la cola circular resultante de añadir a c
 * el Evento e. Si no, devuelve la cola resultante de añadir a c el Evento e, eliminando
 * el Evento más antiguo de la cola, activando el flag V (Overflow).
 */
void cola_guardar_eventos(Evento *e, Cola *c);

/* 
 * Si numEventos > 0, devuelve el Evento más antiguo de la cola (el primero) y devuelve la
 * cola circular resultante de eliminar de la cola dicho Evento. En caso contrario, devuelve
 * un Evento vacío.
 */
Evento cola_desencola_mas_antiguo(Cola *c);

/*
 * Devuelve 1 (verdad) sí y sólo si la cola c no tiene ningún Evento.
 * devuelve 0 (falso) en caso contrario.
 */
uint8_t cola_es_vacia(Cola *c);

/*
 * Si numEventos > 0, devuelve el Evento más nuevo encolado en la cola (el último). En caso contrario, devuelve
 * un evento vacío.
 */
Evento cola_ultimo(Cola *c);

#endif