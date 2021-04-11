#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "Calculo.h"
#include "Etapa1.h"
#include "Etapa2.h"
#include "Lectura.h"
#include "Mensaje.h"
#include "Modelos.h"
#include <stdio.h>
#include <stdlib.h>

int main(int args, char *argv[]){

    Mundo* mundo = (Mundo*) malloc(sizeof(Mundo));
    mundo->buzonMundo = emptyQ();
    mundo->regiones   = emptyQ();
    readData(mundo);
    NUM_THREADS = length(mundo->regiones);
   for (int i=0; i < 355; i++){
       etapa1(mundo);
       etapa2(mundo,NUM_THREADS);
   }
   

    return EXIT_SUCCESS;
}