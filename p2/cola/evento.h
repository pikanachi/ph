#ifndef EVENTO_H
#define EVENTO_H

#include <stdint.h> // Para los uint
#include <stddef.h> // Para NULL

/*
 * Los valores del TAD Evento representan diferentes eventos que pueden surgir
 * en un sistema. Cada Evento es caracterizado por su ID (único), además cada 
 * evento tiene una serie de datos asociados y una estampilla temporal que 
 * indica cuando el evento fue generado.
 */

typedef struct Evento Evento;

struct Evento {
    uint8_t     ID_evento;
    uint32_t    auxData;
    uint32_t    timestamp;
};

/*
 * Devuelve el evento resultante de asignar a evento su ID, auxData,
 * y timestamp.
 */
void evento_crear(uint8_t ID, uint32_t aux, uint32_t ts, Evento *e);

#endif