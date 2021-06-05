#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "super.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


// ---------------------- 
// |      EXTERN        |
// ----------------------
int pendientes;
int comienzo;
int empates;
sem_t comienzoSem;
// ---------------------- 
// |      STRUCT        |
// ----------------------

// Estructura con la info necesaria para imprimir 1 fila del resultado de estados.
typedef struct RowData{
    char* candidato;     // Nombre del candidato
    char* metodo;        // Siglas del metodo con que se calculo el resultado.
    char* estado;        // Nombre del estado
    int   colegioLoc;    // Cantidad de votos de colegio electoral que obtuvo en el estado
    float porcentajeLoc; // Porcentaje de votos normales que obtuvo
} RowData;


// ---------------------- 
// |      FUNCTIONS     |
// ----------------------


/**
 * @brief Imprime la cabecera de la tabla de resultados por estado.
 * 
 */
void printHeader(){
    printf("Nombre Candidato |    Nombre  Estado    | Metodo Usado | Votos Colegio Electoral | Porcentaje de Votos Bruto\n");
}

/**
 * @brief Dado los resultados de dos candidatos para un estado, imprime dos filas de resultados
 * 
 * @param candidato1 informacion del primer  candidato
 * @param candidato2 informacion del segundo candidato
 */
void printRow(RowData *candidato1, RowData *candidato2){
    printf("%-16s | %-20s | %-12s | %23d | %25.3f \n",candidato1->candidato, candidato1->estado, candidato1->metodo, candidato1->colegioLoc, candidato1->porcentajeLoc);
    printf("%-16s | %-20s | %-12s | %23d | %25.3f \n",candidato2->candidato, candidato2->estado, candidato2->metodo, candidato2->colegioLoc, candidato2->porcentajeLoc);
}

/**
 * @brief Dada la data de un estado, calcula e imprime el resultado usnado el metodo NPIVC para ese estado, 
 * y modifica la cantidad de votos electorales globales.
 * 
 * @param data Datos de un estado en especifico.
 */
void npivc(Data* data){

    sem_wait(data->mutexVotosColegio);
    pendientes--;
    sem_post(data->mutexVotosColegio);
    while(1){
        sem_wait(&comienzoSem);
        if (comienzo){
            sem_post(&comienzoSem);
            break;
        }
        sem_post(&comienzoSem);
    }

    
    // saca el % del ganador y perdedor de una por q q  fastidio.
    Reporte* pais   = (Reporte*) malloc(sizeof(Reporte));
    pais = data->rPais;
    Reporte* estado = (Reporte*) malloc(sizeof(Reporte));
    estado = data->estadal;
    float porcentajeGanador  = (float) MAX(pais->cand1, pais->cand2) / (float) (pais->cand1 + pais->cand2) * 100;
    
    float porcentajePerdedor = 100 - porcentajeGanador;
    int colegioLoc1     = 0;
    int colegioLoc2     = 0;
    float porcentajeLoc1;
    float porcentajeLoc2;
    
    
    // decide quien gana.
    if (pais->cand1 > pais->cand2){
        colegioLoc1 += 2 + estado->subCounts;
        porcentajeLoc1 = porcentajeGanador;
        porcentajeLoc2 = porcentajePerdedor;
        
    } else if (pais->cand2 > pais->cand1)
    {
        colegioLoc2   += 2 + estado->subCounts;
        porcentajeLoc2 = porcentajeGanador;
        porcentajeLoc1 = porcentajePerdedor;
    } else {
        sem_wait(&comienzoSem);
        empates += 2 + estado->subCounts;
        sem_post(&comienzoSem);
    }
    
    
    RowData* candidato1Str = (RowData*) malloc(sizeof(RowData));
    candidato1Str->candidato     = data->candidato1;
    candidato1Str->colegioLoc    = colegioLoc1;
    candidato1Str->estado        = estado->nombre;
    candidato1Str->metodo        = "NIPVC";
    candidato1Str->porcentajeLoc = porcentajeLoc1;
    
    RowData* candidato2Str = (RowData*) malloc(sizeof(RowData));
    candidato2Str->candidato     = data->candidato2;
    candidato2Str->colegioLoc    = colegioLoc2;
    candidato2Str->estado        = estado->nombre;
    candidato2Str->metodo        = "NIPVC";
    candidato2Str->porcentajeLoc = porcentajeLoc2;

    printRow(candidato1Str, candidato2Str);

    // Y pedir el mutex para actualizar las cosas compartidas
    sem_wait(data->mutexVotosColegio);
    *(data->colegioGlob1) += colegioLoc1;
    *(data->colegioGlob2) += colegioLoc2;
    pendientes++;
    sem_post(data->mutexVotosColegio);
}

/**
 * @brief Dada la data de un estado, calcula e imprime el resultado usnado el metodo split para ese estado, 
 * y modifica la cantidad de votos electorales globales.
 * 
 * @param data Datos de un estado en especifico.
 */
void split(Data* data){
    Reporte* estado = data->estadal;
    float porcentajeLoc1;
    float porcentajeLoc2;
    long poblacionTotal = estado->cand1 + estado->cand2;
    int colegioLoc1     = 0;
    int colegioLoc2     = 0;
    // Como esperamos un estado, el subDir es un distrito.
    Queue* distritos = estado->subDirs;

    // Iterando sobre los distritos
    while(distritos){
        // Por cada distrito
        Reporte* distrito = (Reporte*) head(distritos);
        // Si el candidato 1 tiene mas votos que el 2, incrementa el voto de colegioLocal1
        if (distrito->cand1 > distrito->cand2){
            colegioLoc1++;
        // Si el candidato 2 tiene mas votos que el 1, lo mismo pero pal candidato 2.
        } else if (distrito->cand1 < distrito->cand2)
        {
            colegioLoc2++;
        // Si ambos candidatos tienen la MISMA cantidad de votos (lo cual es probabilisticamente imposible)
        // manda un error.
        } else {
            sem_wait(&comienzoSem);
            empates++;
            sem_post(&comienzoSem);
        }
        distritos = tail(distritos);
    }

    // En este punto ya tenemos la cantidad de votos electorales simples que tiene un candidato
    // falta por determinal los senatoriales.

    // Si el candidato 1 tiene mas votos que el 2, los 2 votos senatoriales van pal 1
    if (estado->cand1 >  estado->cand2){
        colegioLoc1 += 2;
        porcentajeLoc1 = (float) estado->cand1 / (float) poblacionTotal * 100;
        porcentajeLoc2 = 100 - porcentajeLoc1;
    // Si el candidato 2 tiene mas votos que el 1, los 2 votos senatoriales van pal 2
    } else if (estado->cand2 >  estado->cand1){
        colegioLoc2 += 2;
        porcentajeLoc2 = (float) estado->cand2 / (float) poblacionTotal * 100;
        porcentajeLoc1 = 100 - porcentajeLoc2;
    // Si ambos tienen la misma cantidad, uno pa cada uno
    } else {
        colegioLoc1++;
        colegioLoc2++;
        porcentajeLoc1 = 50;
        porcentajeLoc2 = 50;
    }

   // Llamadas a impresion.

    RowData* candidato1Str = (RowData*) malloc(sizeof(RowData));
    candidato1Str->candidato     = data->candidato1;
    candidato1Str->colegioLoc    = colegioLoc1;
    candidato1Str->estado        = estado->nombre;
    candidato1Str->metodo        = "split";
    candidato1Str->porcentajeLoc = porcentajeLoc1;
    
    RowData* candidato2Str = (RowData*) malloc(sizeof(RowData));
    candidato2Str->candidato     = data->candidato2;
    candidato2Str->colegioLoc    = colegioLoc2;
    candidato2Str->estado        = estado->nombre;
    candidato2Str->metodo        = "split";
    candidato2Str->porcentajeLoc = porcentajeLoc2;


    printRow(candidato1Str, candidato2Str);

    // Y pedir el mutex para actualizar las cosas compartidas
    sem_wait(data->mutexVotosColegio);
    *(data->colegioGlob1) += colegioLoc1;
    *(data->colegioGlob2) += colegioLoc2;
    sem_post(data->mutexVotosColegio);


}

/**
 * @brief Dada la data de un estado, calcula e imprime el resultado usnado el metodo WTA para ese estado, 
 * y modifica la cantidad de votos electorales globales.
 * 
 * @param data Datos de un estado en especifico.
 */
void wta(Data* data){
    Reporte* estado = data->estadal;
    float porcentajeLoc1;
    float porcentajeLoc2;
    long poblacionTotal = estado->cand1 + estado->cand2;
    int colegioLoc1     = 0;
    int colegioLoc2     = 0;
    // Como esperamos un estado, el subDir es un distrito.
    Queue* distritos = estado->subDirs;


    // En este punto ya tenemos la cantidad de votos electorales simples que tiene un candidato
    // falta por determinal los senatoriales.

    // Si el candidato 1 tiene mas votos que el 2, los 2 votos senatoriales van pal 1
    if (estado->cand1 >  estado->cand2){
        colegioLoc1   += 2 + estado->subCounts;
        porcentajeLoc1 = estado->cand1 / poblacionTotal * 100;
        porcentajeLoc2 = 100 - porcentajeLoc1;
    // Si el candidato 2 tiene mas votos que el 1, los 2 votos senatoriales van pal 2
    } else if (estado->cand2 >  estado->cand1){
        colegioLoc2   += 2 + estado->subCounts;
        porcentajeLoc2 = estado->cand2 / poblacionTotal * 100;
        porcentajeLoc1 = 100 - porcentajeLoc2;
    // Si ambos tienen la misma cantidad, uno pa cada uno
    } else {
        sem_wait(&comienzoSem);
        empates += 2 + estado->subCounts;
        sem_post(&comienzoSem);
    }

   // Llamadas a impresion.

    RowData* candidato1Str = (RowData*) malloc(sizeof(RowData));
    candidato1Str->candidato     = data->candidato1;
    candidato1Str->colegioLoc    = colegioLoc1;
    candidato1Str->estado        = estado->nombre;
    candidato1Str->metodo        = "wta";
    candidato1Str->porcentajeLoc = porcentajeLoc1;
    
    RowData* candidato2Str = (RowData*) malloc(sizeof(RowData));
    candidato2Str->candidato     = data->candidato2;
    candidato2Str->colegioLoc    = colegioLoc2;
    candidato2Str->estado        = estado->nombre;
    candidato2Str->metodo        = "wta";
    candidato2Str->porcentajeLoc = porcentajeLoc2;


    printRow(candidato1Str, candidato2Str);

    // Y pedir el mutex para actualizar las cosas compartidas
    sem_wait(data->mutexVotosColegio);
    *(data->colegioGlob1) += colegioLoc1;
    *(data->colegioGlob2) += colegioLoc2;
    sem_post(data->mutexVotosColegio);


}


// probando la tablita
/*
int main(int argc, char const *argv[])
{

    RowData* candidato1Str = (RowData*) malloc(sizeof(RowData));
    candidato1Str->candidato     = "Trump";
    candidato1Str->colegioLoc    = 10;
    candidato1Str->estado        = "Alabama";
    candidato1Str->metodo        = "split";
    candidato1Str->porcentajeLoc = 90.32468;
    
    RowData* candidato2Str = (RowData*) malloc(sizeof(RowData));
    candidato2Str->candidato     = "Biden";
    candidato2Str->colegioLoc    = 5;
    candidato2Str->estado        = "Alabama";
    candidato2Str->metodo        = "split";
    candidato2Str->porcentajeLoc = 100-90.32468;

    printHeader();
    printRow(candidato1Str, candidato2Str);

    return 0;
}
*/