#ifndef RELOJES_H
#define RELOJES_H

#include <stdint.h> // Para los uint
#include "LPC210x.H"

//RTC

/*
 * Inicializa el RTC
 */
void RTC_init(void);

/*
 * Devuelve los minutos que lleva contados el RTC
 */
uint32_t RTC_leer_minutos(void);

/*
 * Devuelve los segundos que lleva contados el RTC
 */
uint32_t RTC_leer_segundos(void);

//WatchDog

/*
 * Inicializa el WatchDog y se prepara para alimentarlo con una alarma periodica cada 5 seg
 */
void WD_init(int sec);

/*
 * Alimenta el WatchDog
 */
void WD_feed(void);

#endif
