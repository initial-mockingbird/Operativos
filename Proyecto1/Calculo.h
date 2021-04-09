#ifndef CALCULO_H_
#define CALCULO_H_

#include "Modelos.h"

int days;
double tasaContagio;
double mortalidadNoTratarla;

Queue* pacienteCero;
Queue* primerMuerto;
Queue* primerMuertoImpreso;
Queue* libraEnfermedad;
Queue* enCuarentena;
Queue* primerCierreAereopuertos;
Queue* aereopuertosCerrados;
Queue* primerCierreNegocios;
Queue* negociosCerrados;
Queue* primerClausuraMercados;
Queue* mercadosCerrados;
Queue* primerTransporteDetenido;
Queue* transportesDetenidos;

int print(Mundo *mundo, char *fileName);
void calculoContagio(Mundo *mundo);

#endif // CALCULO_H_