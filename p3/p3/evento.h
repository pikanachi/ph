#ifndef EVENTO_H
#define EVENTO_H

#include <stdint.h>     // Para los uint

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
static Evento eventoInvalido = {INVALID_ID, 0, 0};

/*
 * Tipo para definir el id de cada tipo de evento
 */
enum {
    Temp_perio = 0,
    Set_Alarma = 1,
    Check_Pulsacion_EINT1 = 2,
    Check_Pulsacion_EINT2 = 3,
    Pulsacion_EINT1 = 4,
    Pulsacion_EINT2 = 5,
    Power_Down = 6,
    Check_Entrada = 7,
    Apagar_Validacion = 8,
    Terminar = 9,
    Check_Pulsacion_Terminar = 10,
		Latido = 11,
		No_Confir_Jugada = 12,
		Latido_Validacion = 13,
		Candidatos = 14,
		Jugada = 15
};

/*
 * Devuelve 1 si el evento tiene id 255 InvalidID.
 * Devuelve 0 en caso contrario.
 */
char es_valido(Evento *e);

Evento el_invalido(void);

#endif
