#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

//el buffer es de tamaño 4096 por elegir un numero grande, pero podria ser
//mas grande o mas chico en base a mas informacion que se piense recibir.
//lo mismo para el nombre del archivo.
#define BUFFER_SIZE 4096
#define FILE_NAME 20

bool valid_int(char *string);

int main(void)
{
    char buffer[10];
    int cant_enteros = 0;
    char v[BUFFER_SIZE];
    memset(v, 0, BUFFER_SIZE);
    char filename[FILE_NAME];
    memset(filename, 0, FILE_NAME);

    // pido al usuario por enteros hasta que me de un numero valido o un 0
    // de darme enteros invalidos, vuelvo a pedir
    do
    {
        printf("Enter integer: ");

        //creo un buffer para guardar la entero (en string) que recibo por input
        fgets(buffer, 10, stdin);

        // Si el input es invalido, devuelve:
        if (!valid_int(buffer))
            printf("Must be an integer value!\n");
        if(buffer[0] != '0') {
            int i = 0;
            while(isdigit(buffer[i]) || i == sizeof(buffer) - 1){
                if(isdigit(buffer[i])){
                    v[(cant_enteros * 10) + i] = buffer[i];
                    i++;
                }else{
                    v[(cant_enteros * 10) + i] = '0';
                    i++;
                }
            }
            cant_enteros++;
        }
    } while (buffer[0] != '0');

    printf("Por favor, ingrese un nombre para el archivo a guardar: ");
    fgets(filename, 20, stdin);

    strcat(filename, ".txt");
    FILE *fp;
    fp  = fopen (filename, "w");
    for(int i = 0; i < cant_enteros * 10; i++) {
        char c = v[i];
        if (i % 10 == 9){
            fprintf(fp,v[i]);
            fprintf(fp,"\n" );
        }else {
            fputc(v[i], fp);
        }
    }

    fclose(fp);
    return 0;
}

bool valid_int(char *string){

    int i = 0;
    // si es un 0 termino la entrada de numeros
    //if(string[i] == 0) return true;

    int length = strlen(string);

    // si no se ingreso ningun valor, termino la lectura
    if (length == i) return false;

    while (i < length - 1) {
        // Si me encuentro con un valor erroneo, termino la lectura
        if (!isdigit(string[i])) {
            return false;
        }
        i++;
    }
    return true;
}