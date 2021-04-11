#include "../Utils/UsefulMacros/macros.h"
#include "Modelos.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Devuelve el nombre de un pais.
 * @param p
 * @return long long
 */
char *nombre(Pais *p){
    return p->nombre;
}

/**
 * @brief Devuelve la poblacion total de un pais.
 * @param p
 * @return long long
 */
long long poblacionTotal(Pais *p){
    return p->poblacionTotal;
}

/**
 * @brief Devuelve la cantidad de contagiado con que un pais cierra sus areopuertos.
 * @param p
 * @return int
 */
int contagiadosCierreAeropuertos(Pais *p){
    return p->contagiadosCierreAeropuertos;
}

/**
 * @brief Devuelve la cantidad de contagiado con que un pais entra en cuarentena.
 * @param p
 * @return int
 */
int contagiadosCuarentena(Pais *p){
    return p->contagiadosCuarentena;
}

/**
 * @brief Devuelve la cantidad de contagiado con que un pais cierra sus negocios.
 * @param p
 * @return int
 */
int contagiadosCierreNegocios(Pais *p){
    return p->contagiadosCierreNegocios;
}

/**
 * @brief Devuelve la cantidad de contagiado con que un pais clausura sus mercados.
 * @param p
 * @return int
 */
int contagiadosClausuraMercados(Pais *p){
    return p->contagiadosClausuraMercados;
}

/**
 * @brief Devuelve la cantidad de contagiado con que un pais detiene sus transportes.
 * @param p
 * @return int
 */
int contagiadosDetenerTransporte(Pais *p){
    return p->contagiadosDetenerTransporte;
}

/**
 * @brief Devuelve el porcentaje de poblacion que pertenece a la clase alta.
 * @param p
 * @return double
 */
double claseAlta(Pais *p){
    return p->claseAlta;
}

/**
 * @brief Devuelve el porcentaje de poblacion que pertenece a la clase media.
 * @param p
 * @return double
 */
double claseMedia(Pais *p){
    return p->claseMedia;
}

/**
 * @brief Devuelve el porcentaje de poblacion que pertenece a la clase baja.
 * @param p
 * @return double
 */
double claseBaja(Pais *p){
    return p->claseBaja;
}

/**
 * @brief Devuelve el porcentaje de clase media que tiene limitaciones.
 * @param p
 * @return double
 */
double limitacionesMedia(Pais *p){
    return p->limitacionesMedia;
}

/**
 * @brief Devuelve el porcentaje de clase baja que tiene limitaciones.
 * @param p
 * @return double
 */
double limitacionesBaja(Pais *p){
    return p->limitacionesBaja;
}

/**
 * @brief Devuelve la poblacion perteneciente a la clase alta que estan infectados.
 * @param p
 * @return long long
 */
long long altaInfectados(Pais *p){
    return (long long) ceil(p->infectadosClase[0]);
}

/**
 * @brief Devuelve la poblacion perteneciente a la clase media que estan infectados.
 * @param p
 * @return long long
 */
long long mediaInfectados(Pais *p){
    
    return (long long) ceil(p->infectadosClase[1]);
}

/**
 * @brief Devuelve la poblacion perteneciente a la clase baja que estan infectados.
 * @param p
 * @return long long
 */
long long bajaInfectados(Pais *p){
    return (long long) ceil(p->infectadosClase[2]);
}

/**
 * @brief Actualiza el porcentaje poblacion perteneciente a la clase alta.
 * @param p
 * @param data
 */
void actualizarClaseAlta(Pais *p, double data){
    p->claseAlta += data;
}

/**
 * @brief Actualiza el porcentaje poblacion perteneciente a la clase media.
 * @param p
 * @param data
 */
void actualizarClaseMedia(Pais *p, double data){
    p->claseMedia += data;
}

/**
 * @brief Actualiza el porcentaje poblacion perteneciente a la clase baja.
 * @param p
 * @param data
 */
void actualizarClaseBaja(Pais *p, double data){
    p->claseBaja += data;
}

/**
 * @brief Actualiza el porcentaje poblacion perteneciente a la clase alta que estan infectados.
 * @param p
 * @param data
 */
void actualizarAltaInfectados(Pais *p, double data){
    p->infectadosClase[0] += data;
}

/**
 * @brief Actualiza el porcentaje poblacion perteneciente a la clase media que estan infectados.
 * @param p
 * @param data
 */
void actualizarMediaInfectados(Pais *p, double data){
    p->infectadosClase[1] = +data;
}

/**
 * @brief Actualiza el porcentaje poblacion perteneciente a la clase baja que estan infectados.
 * @param p
 * @param data
 */
void actualizarBajaInfectados(Pais *p, double data){
    p->infectadosClase[2] += data;
}

/**
 * @brief Actualiza la poblacion total de un pais.
 * @param p
 * @param data
 */
void actualizarPoblacionTotal(Pais *p, long long data){
    p->poblacionTotal += data;
}

/**
 * @brief Busca un pais por su nombre.
 * @param mundo
 * @param nombre
 * @return Pais*
 */
Pais *lookupByName(Mundo *mundo, char *nombre){
    Queue* r = mundo->regiones;
    Queue* pp;
    char* name;

    while(r){
        
        pp = ((Region*)r)->paises;
        while(pp){
            
            name = ((Pais*) head(pp))->nombre;
            if(name && COMPSTR(nombre, ((Pais*) head(pp))->nombre)){
                return (Pais*) head(pp);
            }
            pp = tail(pp);
        }
        r = tail(r);
    }
    return NULL;
}
