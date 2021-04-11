#ifndef LECTURA_H_
#define LECTURA_H_
#include "Modelos.h"

typedef struct infoEnfermedad{
    int duracionDias;
    double tasaContagio;
    double mortalidad;
}infoEnfermedad;

void readData(Mundo* m, char* file_name, struct tm* fechaContagio, infoEnfermedad* infoE);

#endif // LECTURA_H_