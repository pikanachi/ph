#include "relojes.h"
#include "softI.h"


//RTC (EL RTC SIGUE FUNCIONANDO CUANDO SALTA EL WD)

/*
 * Inicializa el RTC
 */
void RTC_init(void){
	//Tratar con el CCR o registro de control del RTC(5 bits)
	CCR = 2;	//Resetear la cuenta(Poner a 1 el bit 1 del CCR)
	CCR = 1;	//Enablear el reloj(Poner el bit 0 a 1) y desactivar el reset(poner bit 1 a 0)
	
	//Modificar PREINT para la frecuencia
	PREINT = 1829; //(CCLK(MHz)/32768 - 1) CCLK = 60MHz
	PREFRAC = 34560; // Lo que se trunca al hacer la division
	PCONP = PCONP | 0x100;
}

/*
 * Devuelve los minutos que lleva contados el RTC
 */
uint32_t RTC_leer_minutos(void) {
	uint32_t res;
	res = MIN;
	return res;
}

/*
 * Devuelve los segundos que lleva contados el RTC
 */
uint32_t RTC_leer_segundos(void) {
	uint32_t res;
	res = SEC;
	return res;
}


//WatchDog

/*
 * Inicializa el WatchDog y se prepara para alimentarlo con una alarma periodica cada 5 seg
 */
void WD_init(int sec){	
	//Poner el WDEN(bit 0 del WDMOD) y el WDRESET(bit 1 del WDMOD) a 1
	//ASignar <sec> como tiempo para resetear para el WD
	WDTC = sec * 256 * 61476;
	WDMOD = 3;
	disable_isr_fiq();
	set_Alarma(Feed, 5000, 1);
	enable_isr_fiq();
}

/*
 * Alimenta el WatchDog
 */
void WD_feed(void){
	disable_isr_fiq();
	WDFEED = 0xAA;
	WDFEED = 0x55;
	enable_isr_fiq();
}
