#include "gestor_energia.h"

/*
 * Pone al procesador en modo IDE
 */
void ge_modo_IDE(void) {
    //Se va a modo idle
    idle();
}

/*
 * Pone al procesador en modo powerdown
 */
void ge_modo_pwdwn(void) {
    pwdwn();
}


//FALTA COMO PADO DE MODO IDLE A POWERDOWN OSEA COMO SE DECIDE ESO SI YA ESTOY EN IDLE?
