#ifndef MODELOS_H_
#define MODELOS_H_
#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/Closures/Closure.h"

typedef struct Pais {
    char nombre[25];
    char* region;
    long long poblacionTotal;
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
    int sucesos[3];
    double infectadosClase[3];
    long infectadosSalen[3];
}Pais;


typedef struct Region {
    Queue* paises;
    char siglas[10];
    Queue* aliadas;
    Queue* buzonSalida;
    Queue* buzonEntrada;
    Queue* buzonPendientes;
    int paisesListos;
    int tareasPendientes;
} Region;

typedef struct Mundo {
    Queue* regiones;
    Queue* buzonMundo; // nuevo
    Queue* eventos; // nuevo
}Mundo;

char *nombre(Pais *p);
long long poblacionTotal(Pais *p);
int contagiadosCierreAeropuertos(Pais *p);
int contagiadosCuarentena(Pais *p);
int contagiadosCierreNegocios(Pais *p);
int contagiadosClausuraMercados(Pais *p);
int contagiadosDetenerTransporte(Pais *p);
double claseAlta(Pais *p);
double claseMedia(Pais *p);
double claseBaja(Pais *p);
double limitacionesMedia(Pais *p);
double limitacionesBaja(Pais *p);
long long altaInfectados(Pais *p);
long long mediaInfectados(Pais *p);
long long bajaInfectados(Pais *p);
void actualizarClaseAlta(Pais *p, double data);
void actualizarClaseMedia(Pais *p, double data);
void actualizarClaseBaja(Pais *p, double data);
void actualizarAltaInfectados(Pais *p, double data);
void actualizarMediaInfectados(Pais *p, double data);
void actualizarBajaInfectados(Pais *p, double data);
void actualizarPoblacionTotal(Pais *p, long long data);
Pais *lookupByName(Mundo *mundo, char *nombre);

#endif // MODELOS_H_