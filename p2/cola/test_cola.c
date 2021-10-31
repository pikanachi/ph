#include "cola/cola.h"
#include <stdio.h>

int main(){
    Evento e1 = {1, 0, 0};
    Evento e2 = {2, 0, 0};
    Evento e3 = {3, 0, 0};
    Evento e4 = {4, 0, 0};
    Evento e5 = {5, 0, 0};
    Evento e6 = {6, 0, 0};
    cola_crear_vacia();
    cola_guardar_evento(e1);
    cola_guardar_evento(e2);
    cola_guardar_evento(e3);
    cola_guardar_evento(e4);
    cola_guardar_evento(e5);
    cola_guardar_evento(e6);


    imp_cola();
    Evento des = cola_desencola_mas_antiguo();
    printf("Desencolado: %d\n",des.ID_evento);

    printf("Encolado 1\n");
    cola_guardar_evento(e1);
    imp_cola();

    Evento ult = cola_ultimo();
    printf("Ultimo: %d\n",ult.ID_evento);

    des = cola_desencola_mas_antiguo();
    printf("Desencolado: %d\n",des.ID_evento);

    printf("Encolado 2\n");
    cola_guardar_evento(e2);
    imp_cola();

    ult = cola_ultimo();
    printf("Ultimo: %d\n",ult.ID_evento);
    return 0;
}