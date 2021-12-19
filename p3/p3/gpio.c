#include "gpio.h"

void GPIO_iniciar(){ 
	GPIO_marcar_entrada(14,16);
	GPIO_marcar_salida(30,2);
	GPIO_marcar_salida(0,14);
}

uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits) {
	uint32_t pin = IOPIN;
	uint32_t mask = 1;
	uint8_t i;
	uint32_t v;
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

void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor) {
	uint32_t pin = IOPIN;
	uint32_t mask = 1;
	uint8_t i;
	uint32_t v;
	uint32_t limpiar;
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

void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits) {
	uint32_t pin = IODIR;
	uint32_t mask = 1;
	uint8_t i;
	uint32_t v;
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


void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits) {
	uint32_t pin = IODIR;
	uint32_t mask = 1;
	uint8_t i;
	uint32_t v;
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
