#include "Mensaje.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Crea e inicializa un Mensaje.
 * @param regionOrigen
 * @param paisOrigen
 * @return Mensaje*
 */
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

/**
 * @brief Devuelve el porcentaje de nuevos infectados pertenecientes a la clase alta.
 * @param MR
 * @return double
 */
double altaNuevosInfectados(MR *MR){
    return (MR->altaNuevosInfectados);
}

/**
 * @brief Devuelve el porcentaje de nuevos infectados pertenecientes a la clase media.
 * @param MR
 * @return double
 */
double mediaNuevosInfectados(MR *MR){
    return (MR->mediaNuevosInfectados);
}

/**
 * @brief Devuelve el porcentaje de nuevos infectados pertenecientes a la clase baja.
 * @param MR
 * @return double
 */
double bajaNuevosInfectados(MR *MR){
    return (MR->bajaNuevosInfectados);
}

/**
 * @brief Devuelve el porcentaje de nuevos muertos pertenecientes a la clase alta.
 * @param MR
 * @return double
 */
double altaNuevosMuertos(MR *MR){
    return (MR->altaNuevosMuertos);
}

/**
 * @brief Devuelve el porcentaje de nuevos muertos pertenecientes a la clase media.
 * @param MR
 * @return double
 */
double mediaNuevosMuertos(MR *MR){
    return (MR->mediaNuevosMuertos);
}

/**
 * @brief Devuelve el porcentaje de nuevos muertos pertenecientes a la clase baja.
 * @param MR
 * @return double
 */
double bajaNuevosMuertos(MR *MR){
    return (MR->bajaNuevosMuertos);
}