#include <LPC210X.H>  
#include "gpioManager.h"

void GPIO_iniciar(){ 
	//bit 31: output | bit 30: output | bit 27-24: input | bit 23-20: input | bit 19-16: input | bit 15-14: input | bit 13: output | bit 12-4 : output | bit 3-0: output
	//2_11000000000000000011111111111111
	//0xC0003FFF
	IODIR = 0xC0003FFF;
	IOCLR = 0xC0003FFF; //Inicializo los outputs a 0
}

int GPIO_leer(int bit_inicial, int num_bits){
	int pin = IOPIN;
	int mask = 1;
	int i;
	int v;
	for(i = bit_inicial; i > 0; i = i - 1){
		mask = mask * 2;
	}
	v = 2 * mask;
	for(i = 1; i < num_bits; i = i +1){
		mask = v + mask;
		v = v * 2;
	}
	pin = pin & mask;
	return pin >> bit_inicial;
}

void GPIO_escribir(int bit_inicial, int num_bits, int valor){
	int pin = IOPIN;
	int mask = 1;
	int i;
	int v;
	int limpiar;
	v = 2 * mask;
	for(i = 1; i < num_bits; i = i + 1){
		mask = v + mask;
		v = v * 2;
	}
	valor = valor & mask;
	limpiar = (~valor & mask) <<(bit_inicial);
	valor = valor <<(bit_inicial);
	IOSET = valor;
	IOCLR = limpiar ;
}

void GPIO_marcar_entrada(int bit_inicial, int num_bits){
	int pin = IODIR;
	int mask = 1;
	int i;
	int v;
	for(i = bit_inicial; i > 0; i = i - 1){
		mask = mask * 2;
	}
	v = 2 * mask;
	for(i = 1; i < num_bits; i = i +1){
		mask = v + mask;
		v = v * 2;
	}
	pin = pin & ~mask;
	IODIR = pin;
}


void GPIO_marcar_salida(int bit_inicial, int num_bits){
	int pin = IODIR;
	int mask = 1;
	int i;
	int v;
	for(i = bit_inicial; i > 0; i = i - 1){
		mask = mask * 2;
	}
	v = 2 * mask;
	for(i = 1; i < num_bits; i = i +1){
		mask = v + mask;
		v = v * 2;
	}
	pin = pin | mask;
	IODIR = pin;
}
