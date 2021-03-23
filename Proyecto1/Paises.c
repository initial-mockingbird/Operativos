
#include "../LinkedList/LinkedList.h"
#include "./Region.h"
#include "./Paises.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ---------------------- 
// |      MACROS        |
// ----------------------

/** @def FROM_DYADIC
 * @brief Casts a dyadic function of type: `a -> b -> c` to `void* -> void* -> void*`.
 * 
 */
#define FROM_DYADIC(f) (void* (*)(void *,void*)) f
/** @def FROM_MONADIC
 * @brief Casts a monadic function of type: `a -> b` to `void* -> void*`.
 * 
 */
#define FROM_MONADIC(f) (void* (*)(void *)) f
/** @def FROM_MONADIC_VOID
 * @brief Casts a monadic function of type: `a -> ()` to `void* -> void`.
 * 
 */
#define FROM_MONADIC_VOID(f) (void (*)(void *)) f
/** @def COMPSTR
 * @brief Compares two strings, returns 1 if they are the same, and 0 otherwise.
 * 
 */
#define COMPSTR(fst, snd) strcmp(fst,snd) == 0

// ---------------------- 
// |      ESTADO        |
// ----------------------

typedef struct Pais {
    char* nombre;
    char* region;
    int poblacionTotal;
    int viajerosDiarios;
    int contagiadosCierreAeropuertos;
    int contagiadosCuarentena;
    int contagiadosCierreNegocios;
    int contagiadosClausuraMercados;
    int contagiadosDetenerTransporte;
    double claseAlta;
    double claseMedia; 
    double claseBaja; 
    double limitacionesMedia;
    double limitacionesBaja;
    double compradores;
    double mercado;
    int domesticos;
}Pais;

/** @var 
 * @brief Lista que contiene todos los paises hasta el momento. 
 * 
 */
extern Queue* paises = NULL;

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Retorna el nombre de un pais.
 * 
 * @param p 
 * @return char* 
 */
char* nombre(Pais* p){
    return p->nombre;
}

/**
 * @brief Retorna la region de un pais.
 * 
 * @param p 
 * @return char* 
 */
char* region(Pais* p){
    return p->region;
}

/**
 * @brief Retorna la poblacion total de un pais.
 * 
 * @param p 
 * @return int
 */
int poblacionTotal(Pais* p){
    return p->poblacionTotal;
}

/**
 * @brief Retorna el numero de viajeros diarios de un pais.
 * 
 * @param p 
 * @return int 
 */
int viajerosDiarios(Pais* p){
    return p->viajerosDiarios;
}

/**
 * @brief Retorna el numero de contagiados en el cierre de aeropuertos.
 * 
 * @param p 
 * @return int 
 */
int contagiadosCierreAeropuertos(Pais* p){
    return p->contagiadosCierreAeropuertos;
}

/**
 * @brief Retorna el numero de contagiados al declarar cuarentena.
 * 
 * @param p 
 * @return int 
 */
int contagiadosCuarentena(Pais* p){
    return p->contagiadosCuarentena;
}

/**
 * @brief Retorna el numero de contagiados en el cierre de negocios.
 * 
 * @param p 
 * @return int 
 */
int contagiadosCierreNegocios(Pais* p){
    return p->contagiadosCierreNegocios;
}

/**
 * @brief Retorna el numero de contagiados en la clausura de mercados.
 * 
 * @param p 
 * @return int 
 */
int contagiadosClausuraMercados(Pais* p){
    return p->contagiadosClausuraMercados;
}

/**
 * @brief Retorna el numero de contagiados al detener el transporte.
 * @param p 
 * @return int 
 */
int contagiadosDetenerTransporte(Pais* p){
    return p->contagiadosDetenerTransporte;
}

/**
 * @brief Retorna el porcentaje de la clase alta de un pais.
 * 
 * @param p 
 * @return double 
 */
double claseAlta(Pais* p){
    return p->claseAlta;
}

/**
 * @brief Retorna el porcentaje de la clase media de un pais.
 * 
 * @param p 
 * @return double 
 */
double claseMedia(Pais* p){
    return p->claseMedia;
}

/**
 * @brief Retorna el porcentaje de la clase baja de un pais.
 * 
 * @param p 
 * @return double 
 */
double claseBaja(Pais* p){
    return p->claseBaja;
}

/**
 * @brief Retorna el porcentaje de la poblacion que compra en tiendas de gama alta.
 * 
 * @param p 
 * @return double 
 */
double compradores(Pais* p){
    return p->compradores;
}

/**
 * @brief Retorna el porcentaje de la poblacion que compra en mercados.
 * 
 * @param p 
 * @return double 
 */
double mercado(Pais* p){
    return p->mercado;
}

/**
 * @brief Retorna el numero de trabajadores domesticos.
 * 
 * @param p 
 * @return int 
 */
int domesticos(Pais* p){
    return p->domesticos;
}

/**
 * @brief Retorna el porcentaje de la poblacion clase media que no puede costear una visita medica.
 * 
 * @param p 
 * @return double 
 */
double limitacionesMedia(Pais* p){
    return p->limitacionesMedia;
}

/**
 * @brief Retorna el porcentaje de la poblacion clase baja que no puede costear una visita medica.
 * 
 * @param p 
 * @return double 
 */
double limitacionesBaja(Pais* p){
    return p->limitacionesBaja;
}

/**
 * @brief Retorna el pais (si existe) que posea el nombre `nombre`.
 * 
 * @param nombre 
 * @return Persona* 
 */
Pais* lookupByName(char* nombre){
    Queue* p = paises;
    char* name;
    while(p){
        name = ((Pais*) head(p))->nombre;
        if(name && COMPSTR(nombre, ((Pais*) head(p))->nombre)){
            return (Pais*) head(p);
        }
        p = tail(p);
    }
    
    return NULL;
}


/**
 * @brief Actualiza la informacion de un pais, si el pais no existe, lo crea. Para campos desconocidos inicializar con `NULL` o `0`.
 *  
 */
void updatePais(char* name){
    
    Pais* pp = NULL;
    if(name){
        pp = lookupByName(name);
    }
    if (!pp){   
        pp = (Pais*) malloc(sizeof(Pais));
        pp->nombre=name;
        paises = snoc((void*) (pp), paises);
        return ;
    }
    return ;
    
}
