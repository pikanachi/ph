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
    Pulsacion_EINT1 = 3,
    Power_Down = 4,
    Apagar_Validacion = 5,
    Terminar = 6,
		Latido = 7,
		No_Confir_Jugada = 8,
		Latido_Validacion = 9,
		Candidatos = 10,
		Jugada = 11,
		Feed = 12,
		Start = 13,
		Check_Terminado_PWDOWN = 14
};

/*
 * Devuelve 1 si el evento tiene id 255 InvalidID.
 * Devuelve 0 en caso contrario.
 */
char es_valido(Evento *e);

Evento el_invalido(void);

#endif
