#ifndef CALCULO_H_
#define CALCULO_H_

#include "Modelos.h"
#include <unistd.h>
#include <time.h>

struct tm *addDay(struct tm* date);
void etapa3(Mundo *mundo, Queue *listas[], double tasaContagio, double mortalidadNoTratarla, struct tm *date);
void etapa5(Mundo *mundo, char *fileName, int days, struct tm *date);

#endif // CALCULO_H_






