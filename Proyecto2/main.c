#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "super.h"
#include "Readdir.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define helpMessage "Modo de empleo:  <dir> [-method|-f <file>]\n"\
                    "Evalua los resultados de una elección estadounidense.\n"\
                    "dir es el directorio conteniendo los votos (ver más adelante)\n"\
                    "[-method] es uno de los siguientes flags\n"\
                    "\t-FPTP, -WTA indicando que en todos los Estados deben asignarse los votos electorales al candidato con mayoría simple en el Estado\n"\
                    "\t-split, -dist indicando que el voto correspondiente a cada Distrito Electoral debe asignarse al candidato que obtenga mayoría simple\n"\
                    "\t--NPIVC, -pop indicando que los votos del Colegio Electoral deben asignarse al candidato que obtenga mayoría simple a nivel nacional\n"\
                    "-h, --help muestra esta ayuda y finaliza\n"

// ---------------------- 
// |      EXTERN        |
// ----------------------
int colegioGlob1 = 0;
int colegioGlob2 = 0;
int comienzo     = 0;
int pendientes   = 0;
int empates      = 0;
Queue* modos = NULL;
sem_t mutexVotosColegio;
sem_t comienzoSem;
Reporte* pais;

int main(int argc, char *argv[]){
    /*------------------------------------- Verificar argumentos -------------------------------------*/

    char dir[25], method[10], file[100]; 

    if(argc == 2) {
    
        if (COMPSTR(argv[1], "-h") == 1 || COMPSTR(argv[1], "--help") == 1) {
            printf(helpMessage);
            exit(EXIT_SUCCESS);
        }

        strcpy(dir, argv[1]);

    }  else {

        if(argc < 2 || (argc == 3 && COMPSTR(argv[2], "-f") == 1)) {
            printf("Faltan argumentos. \nPrueba --help o -h para más información.\n");
            exit(EXIT_FAILURE);
        } else if ((COMPSTR(argv[2], "-f") != 1 && argc > 3) || argc > 4) {
            printf("Demasiados argumentos. \nPrueba --help o -h para más información.\n");
            exit(EXIT_FAILURE);
        }

        if(COMPSTR(argv[2], "-f") != 1 && COMPSTR(argv[2], "-FPTP") != 1 && COMPSTR(argv[2], "-WTA") != 1 && COMPSTR(argv[2], "-split") != 1 && COMPSTR(argv[2], "-dist") != 1 && COMPSTR(argv[2], "--NPIVC") != 1 && COMPSTR(argv[2], "-pop") != 1){
            printf("Argumento invalido. \nPrueba --help o -h para más información.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(dir, argv[1]);


        if (COMPSTR(argv[2], "-f") == 1) {
            strcpy(file, argv[3]);
        } else {
            strcpy(method, argv[2]);
        }
    }

    /*------------------------------------- Recorrer directorios -------------------------------------*/

    Pair* pair =(Pair*) malloc(sizeof(Pair));
    pais = (Reporte*) malloc(sizeof(Reporte));
    // inicializando el modo segun la opcion que el usuario proveyo
    if (!COMPSTR(method,"")){
        // cuando se provee un metodo, entonces el
        // default se convierte en el metodo
        strcpy(pair->fst,"default");
        if (COMPSTR(method,"-split") || COMPSTR(method,"-dist")){
            pair->snd = 1;
        } else if (COMPSTR(method,"-WTA") || COMPSTR(method,"-FPTP")){
            pair->snd = 0;
        } else {
            pair->snd = 2;
        }
        modos = cons((void*) pair,modos);
    } else if (!COMPSTR(file,"")){
        // cuando se provee el archivo, se lee del archivo
        lecturaModo(file);
    } else {
        // y si no se provee nada, el default sera WTA.
        strcpy(pair->fst, "default");
        pair->snd = 0;
    }

    sem_init(&mutexVotosColegio,0,1);
    sem_init(&comienzoSem,0,1);
    struct Reporte *inRp;
    char dst[MAX_LEN] = "", *path;

    //Inicializar reporte que representa al país
    inRp = initReport("Pais", 0);
    lvlListas[0] = snoc((void*) inRp, lvlListas[0]);
    //Concatenar nombre del archivo para conseguir path
    path = concatenateStrings(dst, "./", dir, "");
    //printf("path is: %s\n",path);
    //printf("method is: %s\n",method);
    //printf("file is: %s\n", file);
    printHeader();
    //Leer directorios, subdirectorios y archivos .txt
    readAllDir(path, "", 0, inRp);

    // DE AQUI EN ADELANTE es pura impresion
    pais = inRp;
    sem_wait(&comienzoSem);
    comienzo = 1;
    sem_post(&comienzoSem);
    while(pendientes != 0){
        ;
    }

    float porcentajeGanador  = ((float) MAX(inRp->cand1, inRp->cand2) / (float) (inRp->cand1 + inRp->cand2)) * 100;
    if (inRp->cand1 > inRp->cand2){
        printf("\nEl ganador es: %s\n", nombre1);
        printf("Porcentaje de votos brutos: %.3f\n",porcentajeGanador);
        printf("Con %d votos electorales\n",colegioGlob1);
    } else {
        printf("\nEl ganador es: %s\n",nombre2);
        printf("Porcentaje de votos brutos: %.3f\n",porcentajeGanador);
        printf("Con %d votos electorales\n",colegioGlob2);
    }

    printf("\nCantidad de votos retenidos por empates: %d\n",empates);
    return EXIT_SUCCESS;
}

// //Test
// Queue *q = lvlListas[0], *k;
// while(q){
//     printf("Directorio: %s\n", ((Reporte *) head(q))->nombre);
//     printf("cand1: %ld\n", ((Reporte *) head(q))->cand1);
//     printf("subCounts: %d\n", ((Reporte *) head(q))->subCounts);
//     k = ((Reporte *) head(q))->subDirs;
//     while(k){
//         printf("Subdirectorio: %s\n", ((Reporte *) head(k))->nombre);
//         k = tail(k);
//     }
//     q = tail(q);
// }

// int main(int argc, char *argv[]){
    
//     if(argc < 2 || (argc > 2 && argc < 5)) {
//         printf("Falta un argumento. \nPrueba --help o -h para más información.\n");
//         exit(EXIT_FAILURE);
//     }

//     if(argc == 2 && (COMPSTR(argv[1], "-h") == 1 || COMPSTR(argv[1], "--help") == 1)) {
//         printf(helpMessage);
//         exit(EXIT_SUCCESS);
//     } 

//     char dir[25], method[10], file[15];
    
//     strcpy(dir, argv[1]);
//     strcpy(file, argv[4]);

//     if(COMPSTR(argv[2], "-FPTP") != 1 && COMPSTR(argv[2], "-WTA") != 1 && COMPSTR(argv[2], "-split") != 1 && COMPSTR(argv[2], "-dist") != 1 && COMPSTR(argv[2], "--NPIVC") != 1 && COMPSTR(argv[2], "-pop") != 1){
//         printf("Argumento invalido. \nPrueba --help o -h para más información.\n");
//     }
//     strcpy(method, argv[2]);
    
    
//     readAllDir("./0", "", 0);
//     return EXIT_SUCCESS;
// }
