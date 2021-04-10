#ifndef CALCULO_H_
#define CALCULO_H_

#include "Modelos.h"
#include <unistd.h>
#include <time.h>

struct tm *addDay(struct tm* date);
void Etapa3(Mundo *mundo, Queue *listas[], double tasaContagio, double mortalidadNoTratarla, struct tm *date);
pid_t Etapa5(Mundo *mundo, char *fileName, int days);

#endif // CALCULO_H_

// Colocar en el main
// Queue* pacienteCero = NULL;
// Queue* primerMuerto = NULL;
// Queue* primerMuertoImpreso = NULL;
// Queue* libraEnfermedad = NULL;
// Queue* enCuarentena = NULL;
// Queue* primerCierreAereopuertos = NULL;
// Queue* aereopuertosCerrados = NULL;
// Queue* primerCierreNegocios = NULL;
// Queue* negociosCerrados = NULL;
// Queue* primerClausuraMercados = NULL;
// Queue* mercadosCerrados = NULL;
// Queue* primerTransporteDetenido = NULL;
// Queue* transportesDetenidos = NULL;

// Queue *listas[13];

// lista[0]    = pacienteCero;
// lista[1]    = primerMuerto;
// lista[2]    = primerMuertoImpreso;
// lista[3]    = libraEnfermedad;
// lista[4]    = enCuarentena;
// lista[5]    = primerCierreAereopuertos;
// lista[6]    = aereopuertosCerrados;
// lista[7]    = primerCierreNegocios;
// lista[8]    = negociosCerrados;
// lista[9]    = primerClausuraMercados;
// lista[10]   = mercadosCerrados;
// lista[11]   = primerTransporteDetenido;
// lista[12]   = transportesDetenidos;


