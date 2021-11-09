#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H


/* Pre:  true
 * Post: Inicializa los bits del GPIO para el juego
 */
void GPIO_iniciar();

//Mejor seria un uint16
int GPIO_leer(int bit_inicial, int num_bits);


/* Pre:  true
 * Post: Si el valor se puede representar con num_bits
 *       devuelve 1 indicando que no se ha producido
 *       nada extraño, en caso contrario devuelve 0
 */
void GPIO_escribir(int bit_inicial, int num_bits, int valor);

void GPIO_marcar_entrada(int bit_inicial, int num_bits);

void GPIO_marcar_salida(int bit_inicial, int num_bits);

#endif