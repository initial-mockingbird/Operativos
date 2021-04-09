#include "Mensaje.h"
#include <stdlib.h>
#include <string.h>
Mensaje* emptyMessage(char* regionOrigen, char* paisOrigen){
    Mensaje* empty = (Mensaje*) malloc(sizeof(Mensaje));
    empty->regionOrigen   = (char*) malloc((strlen(regionOrigen) +1) * sizeof(char));
    empty->regionDestino  = (char*) malloc((strlen(regionOrigen) +1) * sizeof(char));
    empty->paisOrigen     = (char*) malloc((strlen(paisOrigen) +1) * sizeof(char));
    strcpy(empty->regionOrigen, regionOrigen);
    strcpy(empty->regionDestino, regionOrigen);
    strcpy(empty->paisOrigen, paisOrigen);
    empty->TTL            = 0;
    empty->contenido[0]   = 0;
    empty->contenido[1]   = 0;
    empty->contenido[2]   = 0;
    empty->infectados[0]  = 0;
    empty->infectados[1]  = 0;
    empty->infectados[2]  = 0;
    empty->estado         = 0;

    return empty;
}