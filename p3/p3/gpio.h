#ifndef GPIO_H
#define GPIO_H

#include <LPC210x.H>
#include <stdint.h> // Para los uint

/*
 * Inicializa el GPIO
 */
void GPIO_iniciar(void);

/*
 * bit_inicial indica el primer bit a leer. num_bits indica cuántos bits queremos leer. 
 * La función devuelve un entero con el valor de los bits indicados.
 */
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits);

/*
 * similar al anterior, pero en lugar de leer escribe en los bits indicados el valor 
 * (si valor no puede representarse en los bits indicados se escribirá los num_bits 
 * menos significativos a partir del inicial)
 */
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor);

/*
 * los bits indicados se utilizarán como pines de entrada desde bit_inicial hasta
 * bit_inicial + num_bits.
 */
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits);

/*
 * los bits indicados se utilizarán como pines de salida desde bit_inicial hasta
 * bit_inicial + num_bits.
 */
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits);

#endif
