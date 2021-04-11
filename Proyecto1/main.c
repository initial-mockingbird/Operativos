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
#include <unistd.h>
#include <sys/wait.h>

Queue* pacienteCero = NULL;
Queue* primerMuerto = NULL;
Queue* primerMuertoImpreso = NULL;
Queue* libraEnfermedad = NULL;
Queue* enCuarentena = NULL;
Queue* primerCierreAereopuertos = NULL;
Queue* aereopuertosCerrados = NULL;
Queue* primerCierreNegocios = NULL;
Queue* negociosCerrados = NULL;
Queue* primerClausuraMercados = NULL;
Queue* mercadosCerrados = NULL;
Queue* primerTransporteDetenido = NULL;
Queue* transportesDetenidos = NULL;

int main(int args, char *argv[]){

    Mundo* mundo = (Mundo*) malloc(sizeof(Mundo));
    struct tm* date = malloc(sizeof(struct tm));
    infoEnfermedad* infoE = (infoEnfermedad*) malloc(sizeof(struct infoEnfermedad));
    Queue* listas[13];
    pid_t child;
    int NUM_THREADS;

    //Inicializar listas en mundo
    mundo->buzonMundo = emptyQ();
    mundo->regiones   = emptyQ();

    //Iniciar arreglo con los pointers a las listas
    listas[0]    = pacienteCero;
    listas[1]    = primerMuerto;
    listas[2]    = primerMuertoImpreso;
    listas[3]    = libraEnfermedad;
    listas[4]    = enCuarentena;
    listas[5]    = primerCierreAereopuertos;
    listas[6]    = aereopuertosCerrados;
    listas[7]    = primerCierreNegocios;
    listas[8]    = negociosCerrados;
    listas[9]    = primerClausuraMercados;
    listas[10]   = mercadosCerrados;
    listas[11]   = primerTransporteDetenido;
    listas[12]   = transportesDetenidos;

    readData(mundo, argv[1], date, infoE);
    
    //printf("PAIS: %s\n",(Pais*) head(((Region*) head(mundo->regiones))->paises));
    NUM_THREADS = length(mundo->regiones);
    //for (int i=0; i < infoE->duracionDias; i++){
    for (int i=0; i < 1; i++){
       etapa1(mundo);
       //etapa2(mundo,NUM_THREADS);
       //etapa3(mundo, listas, infoE->tasaContagio, infoE->mortalidad, date);
       //child = etapa5(mundo, argv[2], i);
    }
    
    //if(child) waitpid(child, NULL, 0);

    return EXIT_SUCCESS;
}