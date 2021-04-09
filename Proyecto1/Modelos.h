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

#endif // MODELOS_H_