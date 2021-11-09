#ifndef EVENTO_H
#define EVENTO_H

#include <stdint.h>     // Para los uint
#include <stddef.h>     // Para NULL

#define INVALID_ID 255  //ID de los eventos inválidos

/*
 * Cada Evento es caracterizado por su ID (único), además cada 
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
 * Tipo de evento invalido
 */
extern Evento eventoInvalido;   

/*
 * Tipo para definir el id de cada tipo de evento
 */
enum {
    Temp_perio = 0,
    Set_Alarma = 1,
    NOMBRE_EVENTO_2 = 2,
    NOMBRE_EVENTO_3 = 3,
    NOMBRE_EVENTO_4 = 4,
    NOMBRE_EVENTO_5 = 5,
    NOMBRE_EVENTO_6 = 6,
    NOMBRE_EVENTO_7 = 7,
    NOMBRE_EVENTO_8 = 8,
    NOMBRE_EVENTO_9 = 9,
    NOMBRE_EVENTO_10 = 10
};

/*
 * Devuelve 1 si el evento tiene id 255 InvalidID.
 * Devuelve 0 en caso contrario.
 */
char es_valido(Evento *e);

#endif
