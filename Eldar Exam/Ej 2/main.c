#include "defines.h"

int main(void)
{
    char monto[PURCHASE_DIGITS];
    char tam_tarjeta[2];
    char card_sc[3];
    card_t tarjeta;

    memset(monto, '-', PURCHASE_DIGITS);

    retrieve_message(monto, 0);

    amount_frame(monto);

    int t_tarj = atoi(tam_tarjeta);

    while (t_tarj < 12){
        printf("El tamaño de la tarjeta es muy chico.\n Intente denuevo: \n");
        retrieve_message(tam_tarjeta, 1);
        t_tarj = atoi(tam_tarjeta);
    }
    char card_number[t_tarj];

    retrieve_message(card_number, 2);

    if(!card_validation(&tarjeta, &card_number, t_tarj)){
        printf("TARJETA NO SOPORTADA");
        return 0;
    }

    retrieve_card_label(&tarjeta);

    retrieve_message(card_sc, 3);

    char req_message[] = MESSAGE_TYPE_MESS;
    strcat(req_message, '0200 \n');

    strcat(req_message, CARD_NUM_MESS);
    strcat(tam_tarjeta, card_number);
    strcat(tam_tarjeta, '\n');

    strcat(req_message, tam_tarjeta);
    strcat(req_message, '0200 \n');

    strcat(req_message, PURCHASE_AMOUNT_MESS);
    strcat(req_message, monto);
    strcat(req_message, '\n');

    strcat(req_message, SAFE_CODE_MESS);
    strcat(req_message, card_sc);

    int handle = socketCreate();

    while(handle == -1){
        handle = socketCreate();
    }


    int connection = socketConnect(handle, ip, port);

    while (connection == -1){
        time_t t0 = time(0);
        connection = socketConnect(handle, ip, port);
        time_t t1 = time(0);
        double datetime_diff = difftime(t1, t0);
        if(datetime_diff > 5){
            printf('CONNECTION ERROR');
            return -1;
        }
    }

    int send_mess = socketWrite(handle, req_message);
    while (send_mess == -1){
        time_t t0 = time(0);
        send_mess = socketWrite(handle, req_message);
        time_t t1 = time(0);
        double datetime_diff = difftime(t1, t0);
        if(datetime_diff > 5){
            printf('ERROR DE COMUNICACION');
            return -1;
        }
    }

    int response = socketRead(handle, req_message, 5);

    if(response == 00){
        printf("APROBADA");
    } else{
        printf("RECHAZADA");
    }

    return 0;
}

// Arme esta funcion para que sea mas prolijo el hecho de mandar y
// y recibir input/output del usuario y en caso de que no sea valido
// lo vuelve a pedir.
void retrieve_message(char *save, int text){
    switch (text) {
        case 0:
            printf(PURCHASE_TEXT);
            break;
        case 1:
            printf(CARD_SIZE);
            break;
        case 2:
            printf(CARD_ID);
            break;
        case 3:
            printf(SAFE_CODE);
            break;
    }
    do
    {
        fgets(save, sizeof(save), stdin);
        // Si el input es invalido, devuelve:
        if (!valid_int(save))
            printf("El numero ingresado no es valido.\n Intente denuevo: \n");
    } while (!valid_int(save));
}


// Esta funcion se encarga de validar la tarjeta por medio de los rangos
// que se encuentran en el archivo ranges.dat
bool card_validation(card_t *card, char *card_num, int size){
    char digitos_c[8];
    int digitos_tarj;
    for(int i = 0; i < 8; i++){
        digitos_c[i] = card_num[i];
    }
    digitos_tarj = atoi(digitos_c);
    FILE *fp;
    fp  = fopen ("ranges.dat", "r");
    range_t rango;
    if(fp == NULL) return false;
    while(fread(&rango, sizeof(rango), 1, fp)){
        int rango_alto = atoi(rango.rangeHigh);
        int rango_bajo = atoi(rango.rangeLow);
        if(digitos_tarj >= rango_bajo || digitos_tarj <= rango_alto && size == rango.len){
            card->id = rango.id;
            return true;
        }
    }
    fclose(fp);
    return false;
}


// Esta funcion busca en el archivo cards.dat el label de la tarjeta
void retrieve_card_label(card_t *tarjeta){
    FILE *fp;
    fp  = fopen ("cards.dat", "r");
    card_t labels;
    while(fread(&labels, sizeof(labels), 1, fp)){
        if(labels.id == tarjeta->id){
            for (int i = 0; i < sizeof(labels.label); ++i) {
                tarjeta->label[i] = labels.label[i];
            }
            break;
        }
    }
    fclose(fp);
}


// Esta funcion esta solamente para recibir el string del monto
// y dejarlo en el formato pedido 125,24546 => 0000012524
void amount_frame(char *monto){
    bool is_float = false;
    int amount_size = 0;
    int f_numbers = 0;
    char monto_format[PURCHASE_DIGITS];
    memset(monto_format, 0, PURCHASE_DIGITS);

    for(int i = 0; i < PURCHASE_DIGITS; i++) {
        if (!isdigit(monto[i])) amount_size++;
        if (isdigit(monto[i]) && is_float) f_numbers++;
        if (monto[i] == ',') is_float = true;
    }
    int index = 0;
    int i = 0;
    while(index < PURCHASE_DIGITS){
        if (index > amount_size){
            if(is_float){
                if(f_numbers > 2){
                    index++;
                    f_numbers--;
                }else if (isdigit(monto[i])){
                        monto_format[index] = monto[i];
                        i++;
                }else{
                    i++;
                }
            }
        }
        index++;
    }
    for (int j = 0; j < PURCHASE_DIGITS; ++j) {
        monto[j] = monto_format[j];
    }
}