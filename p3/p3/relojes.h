#ifndef RELOJES_H
#define RELOJES_H

#include <stdint.h> // Para los uint
#include "LPC210x.H"

void RTC_init(void);

uint32_t RTC_leer_minutos(void);

uint32_t RTC_leer_segundos(void);

#endif