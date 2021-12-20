#include "gpio.h"

/*
 * Inicializa el GPIO
 */
void GPIO_iniciar(){ 
	GPIO_marcar_entrada(14,2);
	GPIO_marcar_salida(16,17);
	GPIO_marcar_salida(0,14);
}

/*
 * bit_inicial indica el primer bit a leer. num_bits indica cuántos bits queremos leer. 
 * La función devuelve un entero con el valor de los bits indicados.
 */
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

/*
 * similar al anterior, pero en lugar de leer escribe en los bits indicados el valor 
 * (si valor no puede representarse en los bits indicados se escribirá los num_bits 
 * menos significativos a partir del inicial)
 */
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

/*
 * los bits indicados se utilizarán como pines de entrada desde bit_inicial hasta
 * bit_inicial + num_bits.
 */
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

/*
 * los bits indicados se utilizarán como pines de salida desde bit_inicial hasta
 * bit_inicial + num_bits.
 */
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
