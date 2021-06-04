#ifndef SUPER_H_
#define SUPER_H_
#include <pthread.h>
#include <semaphore.h>
#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"

#define MAX_LEN 256



// ---------------------- 
// |      STRUCT        |
// ----------------------
typedef struct Reporte{
    char   nombre[MAX_LEN];     //Nombre del dir archivo
    int    nivel;               //Niveles: [0] Pais [1] Estado [2] Distrito Electoral [3] Condado
    long   cand1;               //Votos por candidato 1 en ese nivel
    long   cand2;               //Votos por candidato 2 en ese nivel
    int    subCounts;           //Cantidad de subdir o subarchivos en ese nivel
    Queue* subDirs;             //Lista de reportes de los subdirectorios inmediatos
}Reporte;

// Estructura par, que alberga: (nombre estado,modo)
// en donde modo es 0 (WAT) 1 (SPLIT) 2 (POP)
typedef struct Pair{
    char fst[MAX_LEN]; // Nombre del estado.
    int snd;           // Metodo, 0 para WAT, 1 para SPLIT y 2 para POP
} Pair;



// Posible dummy structure, encapsula todas las cosas que necesito para hacer los calculos
typedef struct Data{
    Reporte* rPais;                 // Para el modo National Popular Vote Interstate Compact, basta con el reporte pais
    Reporte* estadal;               // Reporte del estado
    char*  candidato1;              // Nombre del candidato 1.
    char*  candidato2;              // Nombre del candidato 2.
    sem_t* mutexVotosColegio;       // el mutex para poder modificar cuantos votos de COLEGIO ELECTORAL tienen los candidatos (globalmente)
    int*   colegioGlob1;            // apuntador a la cantidad de votos del colegio electoral (global) que tiene el candidato 1.
    int*   colegioGlob2;            // apuntador a la cantidad de votos del colegio electoral (global) que tiene el candidato 2.
    int*   comienzo;                // apuntador a un entero que senaliza cuantos estados faltan por finalizar

}Data;



// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

void split(Data* data);
void npivc(Data* data);
void wta(Data* data);
void printHeader();

// ---------------------- 
// |      VARIABLES     |
// ----------------------

extern int  colegioGlob1;
extern int  colegioGlob2;
extern int  comienzo;
extern sem_t mutexVotosColegio;
extern sem_t comienzoSem;
extern Queue* modos;
extern Reporte* pais;
extern int pendientes;

#endif // SUPER_H_