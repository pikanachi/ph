#include "relojes.h"

void RTC_init(void){
	//Tratar con el CCR o registro de control del RTC(5 bits)
	CCR = 2;	//Resetear la cuenta(Poner a 1 el bit 1 del CCR)
	CCR = 1;	//Enablear el reloj(Poner el bit 0 a 1) y desactivar el reset(poner bit 1 a 0)
	
	//Modificar PREINT para la frecuencia
	PREINT = 1829; //(CCLK(MHz)/32768 - 1) CCLK = 60MHz
	PREFRAC = 34560; // Lo que se trunca al hacer la division
	PCONP = PCONP | 0x100;
}