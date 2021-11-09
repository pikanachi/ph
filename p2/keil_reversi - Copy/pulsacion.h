#ifndef PULSACION_H
#define PULSACION_H

enum {
	PULSADO = 0,
	NO_PULSADO = 1
};

unsigned int nueva_pulsacion(void);
void clear_nueva_pulsacion(void);
void actualizar_estado(void);
unsigned int leer_estado(void);
unsigned int leer_entrada(void);

#endif
