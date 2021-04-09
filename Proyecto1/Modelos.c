#include "Modelos.h"
#include <stdlib.h>
#include <string.h>

long long poblacionTotal(Pais *p){
    return p->poblacionTotal;
}

int contagiadosCierreAeropuertos(Pais *p){
    return p->contagiadosCierreAeropuertos;
}

int contagiadosCuarentena(Pais *p){
    return p->contagiadosCuarentena;
}

int contagiadosCierreNegocios(Pais *p){
    return p->contagiadosCierreNegocios;
}

int contagiadosClausuraMercados(Pais *p){
    return p->contagiadosClausuraMercados;
}

int contagiadosDetenerTransporte(Pais *p){
    return p->contagiadosDetenerTransporte;
}

double claseAlta(Pais *p){
    return p->claseAlta;
}

double claseMedia(Pais *p){
    return p->claseMedia;
}

double claseBaja(Pais *p){
    return p->claseBaja;
}

double limitacionesMedia(Pais *p){
    return p->limitacionesMedia;
}

double limitacionesBaja(Pais *p){
    return p->limitacionesBaja;
}

long long altaInfectados(Pais *p){
    return (long long) ceil((p->infectadosClase[0] * claseAlta(p)) * poblacionTotal(p));
}

long long mediaInfectados(Pais *p){
    return (long long) ceil((p->infectadosClase[1] * claseMedia(p)) * poblacionTotal(p));
}

long long bajaInfectados(Pais *p){
    return (long long) ceil((p->infectadosClase[2] * claseBaja(p)) * poblacionTotal(p));
}

void actualizarClaseAlta(Pais *p, double data){
    p->claseAlta += data;
}

void actualizarClaseMedia(Pais *p, double data){
    p->claseMedia += data;
}

void actualizarClaseBaja(Pais *p, double data){
    p->claseBaja += data;
}

void actualizarAltosInfectados(Pais *p, double data){
    p->infectadosClase[0] += data;
}

void actualizarMediaInfectados(Pais *p, double data){
    p->infectadosClase[1] += data;
}

void actualizarBajaInfectados(Pais *p, double data){
    p->infectadosClase[2] += data;
}

void actualizarPoblacionTotal(Pais *p, double data){
    p->poblacionTotal += data;
}

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
