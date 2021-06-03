#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <pthread.h>
//#include <semaphore.h>



// esto lo tengo que borrar cuando kha integre el tipo
// pero por ahora mejor tenerlo para que el coso este me detecte errores de tipos
typedef struct Reporte{
    char* nombre;   // El nombre del pais/estado/distrito/condado
    int nivel;      // Niveles: [0] Pais [1] Estado [2] Distrito Electoral [3] Condado
    long cand1;     // Cantidad de votos para el candidato 1
    long cand2;     // Cantidad de votos para el candidato 2
    int subCounts;  // Cantidad de estados (pais), distritos (estados) o condados (distritos) que posee el reporte 
    Queue* subDirs; // Una cola a los reportes de estados (pais), distritos (estados) o condados (distritos).

}Reporte;

// Posible dummy structure, encapsula todas las cosas que necesito para hacer los calculos
typedef struct splitData{
    Reporte* estadal;               // Para el modo split, basta con el reporte estadal
    char*  candidato1;              // Nombre del candidato 1.
    char*  candidato2;              // Nombre del candidato 2.
    //sem_t* mutexVotosColegio;     // el mutex para poder modificar cuantos votos de COLEGIO ELECTORAL tienen los candidatos (globalmente)
    int*   colegioGlob1;            // apuntador a la cantidad de votos del colegio electoral (global) que tiene el candidato 1.
    int*   colegioGlob2;            // apuntador a la cantidad de votos del colegio electoral (global) que tiene el candidato 2.
    int*   estadosFinalizados;      // apuntador a un entero que senaliza cuantos estados faltan por finalizar

}splitData;

// Posible dummy structure, encapsula todas las cosas que necesito para hacer los calculos
typedef struct npvicData{
    Reporte* pais;                  // Para el modo National Popular Vote Interstate Compact, basta con el reporte pais
    char*  candidato1;              // Nombre del candidato 1.
    char*  candidato2;              // Nombre del candidato 2.
    char*  estado;                  // nombre del estado.
    //sem_t* mutexVotosColegio;     // el mutex para poder modificar cuantos votos de COLEGIO ELECTORAL tienen los candidatos (globalmente)
    int*   colegioGlob1;            // apuntador a la cantidad de votos del colegio electoral (global) que tiene el candidato 1.
    int*   colegioGlob2;            // apuntador a la cantidad de votos del colegio electoral (global) que tiene el candidato 2.
    int*   estadosFinalizados;      // apuntador a un entero que senaliza cuantos estados faltan por finalizar

}npvicData;


typedef struct RowData{
    char* candidato;     // Nombre del candidato
    char* metodo;        // Siglas del metodo con que se calculo el resultado.
    char* estado;        // Nombre del estado
    int   colegioLoc;    // Cantidad de votos de colegio electoral que obtuvo en el estado
    float porcentajeLoc; // Porcentaje de votos normales que obtuvo
} RowData;

void printHeader();
void printRow(RowData*, RowData*);
void split(splitData*);
void npivc(npvicData*);
void wta(splitData*);
void printHeader(){
    printf("Nombre Candidato | Nombre Estado | Metodo Usado | Votos Colegio Electoral | Porcentaje de Votos Bruto\n");
}

void printRow(RowData *candidato1, RowData *candidato2){
    printf("%-16s | %-13s | %-12s | %23d | %25.3f \n",candidato1->candidato, candidato1->estado, candidato1->metodo, candidato1->colegioLoc, candidato1->porcentajeLoc);
    printf("%-16s | %-13s | %-12s | %23d | %25.3f \n",candidato2->candidato, candidato2->estado, candidato2->metodo, candidato2->colegioLoc, candidato2->porcentajeLoc);
}


void npivc(npvicData* data){
    // saca el % del ganador y perdedor de una por q q  fastidio.
    float porcentajeGanador  = MAX(data->pais->cand1, data->pais->cand2) / (data->pais->cand1 + data->pais->cand2) * 100;
    float porcentajePerdedor = 100 - porcentajeGanador;
    int colegioLoc1     = 0;
    int colegioLoc2     = 0;
    float porcentajeLoc1;
    float porcentajeLoc2;
    
    // decide quien gana.
    if (data->pais->cand1 > data->pais->cand2){
        colegioLoc1 += 3;
        porcentajeLoc1 = porcentajeGanador;
        porcentajeLoc2 = porcentajePerdedor;
        
    } else if (data->pais->cand2 > data->pais->cand1)
    {
        colegioLoc2 += 3;
        porcentajeLoc2 = porcentajeGanador;
        porcentajeLoc1 = porcentajePerdedor;
    } else {
        perror("EMPATE A NIVEL NACIONAL, INSOLITO!");
    }
    

    RowData* candidato1Str = (RowData*) malloc(sizeof(RowData));
    candidato1Str->candidato     = data->candidato1;
    candidato1Str->colegioLoc    = colegioLoc1;
    candidato1Str->estado        = data->estado;
    candidato1Str->metodo        = "NIPVC";
    candidato1Str->porcentajeLoc = porcentajeLoc1;
    
    RowData* candidato2Str = (RowData*) malloc(sizeof(RowData));
    candidato2Str->candidato     = data->candidato2;
    candidato2Str->colegioLoc    = colegioLoc2;
    candidato2Str->estado        = data->estado;
    candidato2Str->metodo        = "NIPVC";
    candidato2Str->porcentajeLoc = porcentajeLoc2;

    printRow(candidato1Str, candidato2Str);

    // Y pedir el mutex para actualizar las cosas compartidas
    /*
    sem_wait(data->mutexVotosColegio);
    *(data->colegioGlob1) += colegioLoc1;
    *(data->colegioGlob2) += colegioLoc2;
    (*(data->estadosFinalizados))--;
    sem_post(data->mutexVotosColegio);
    */
}

void split(splitData* data){
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
            perror("Lo imposible sucedio, dos distritos tienen la MISMA cantidad de votos");
        }
        distritos = tail(distritos);
    }

    // En este punto ya tenemos la cantidad de votos electorales simples que tiene un candidato
    // falta por determinal los senatoriales.

    // Si el candidato 1 tiene mas votos que el 2, los 2 votos senatoriales van pal 1
    if (estado->cand1 >  estado->cand2){
        colegioLoc1 += 2;
        porcentajeLoc1 = estado->cand1 / poblacionTotal * 100;
        porcentajeLoc2 = 100 - porcentajeLoc1;
    // Si el candidato 2 tiene mas votos que el 1, los 2 votos senatoriales van pal 2
    } else if (estado->cand2 >  estado->cand1){
        colegioLoc2 += 2;
        porcentajeLoc2 = estado->cand2 / poblacionTotal * 100;
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
    /*
    sem_wait(data->mutexVotosColegio);
    *(data->colegioGlob1) += colegioLoc1;
    *(data->colegioGlob2) += colegioLoc2;
    (*(data->estadosFinalizados))--;
    sem_post(data->mutexVotosColegio);
    */


}


void wta(splitData* data){
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
        perror("EMPATE WTA, QUE HAGO?");
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
    /*
    sem_wait(data->mutexVotosColegio);
    *(data->colegioGlob1) += colegioLoc1;
    *(data->colegioGlob2) += colegioLoc2;
    (*(data->estadosFinalizados))--;
    sem_post(data->mutexVotosColegio);
    */


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