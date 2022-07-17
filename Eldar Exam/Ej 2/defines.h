#ifndef EJ2_DEFINES_H
#define EJ2_DEFINES_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


#define PURCHASE_DIGITS 12
#define PURCHASE_TEXT "Ingrese el monto del producto: "
#define CARD_SIZE "Ingrese la cantidad de digitos en la tarjeta: "
#define CARD_ID "Ingrese el numero de tarjeta: "
#define PURCHASE_TEXT "Ingrese el monto del producto: "
#define SAFE_CODE "Ingrese el codigo de seguridad: "

#define MESSAGE_TYPE_MESS "Tipo de mensaje: "
#define CARD_NUM_MESS "Número de tarjeta: "
#define PURCHASE_AMOUNT_MESS "Monto: "
#define SAFE_CODE_MESS "Código de seguridad: "

#define ip '100.100.100'
#define port 2
// Hice un define de ip y port para tener un test del codigo
// Como no trabaje con ip hasta ahora (si bien lo busque en google
// para tener una idea de como obtenerlo) no lo quise agregar al
// codigo para que muestro mis conocimientos actuales.


typedef struct {
    char rangeLow[8 + 1];
    char rangeHigh[8 + 1];
    unsigned char len;
    int id;
} range_t;

typedef struct {
    char label[12 + 1];
    int id;
} card_t;


bool valid_int(char *string);

void retrieve_message(char *save, int text);

bool card_validation(card_t *card, char *card_number, int size);

void retrieve_card_label(card_t *tarjeta);

void amount_frame(char *monto);


//--------------------------------------------------------------------
/**
* Crea un socket
* @return handle del socket o -1 en caso de error
*/
int socketCreate();
/**
* Conectar el socket a un host
* @param handle handle del socket a utilizar
* @param ip direccion ip del host a conectarse
* @param port puerto del host a conectarse
* @return 0 en caso de exito o -1 en caso de error
*/
int socketConnect(int handle, const char *ip, unsigned short port);
/**
* Leer datos del socket. La función bloquea hasta leer datos o transcurrir maxTimeout
milisegundos
* @param handle handle del socket a utilizar
* @param data puntero donde se guardarán los datos leidos
* @param maxTimeout tiempo máximo de espera en milisegundos
* @return cantidad de bytes leidos o -1 en caso de error
*/
int socketRead(int handle, unsigned char * data, int maxTimeout);
/**
* Escribir datos en un socket
* @param handle handle del socket a utilizar
* @param data puntero desde donde se leerán los datos a escribir
* @return cantidad de bytes escritos o -1 en caso de error
*/
int socketWrite(int handle, const unsigned char * data);
/**
* Cierra el socket y libera recursos
* @param handle handle del socket a utilizar
* @return 0 en caso de exito o -1 en caso de error
*/
int socketClose( int handle ) ;

#endif //EJ2_DEFINES_H
