#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "Readdir.h"
#include "Methods.h"
#include <stdlib.h>
#include <stdio.h>

#define helpMessage "Modo de empleo:  <dir> [-method|-f <file>]\n"\
                    "Evalua los resultados de una elección estadounidense.\n"\
                    "dir es el directorio conteniendo los votos (ver más adelante)\n"\
                    "[-method] es uno de los siguientes flags\n"\
                    "\t-FPTP, -WTA indicando que en todos los Estados deben asignarse los votos electorales al candidato con mayoría simple en el Estado\n"\
                    "\t-split, -dist indicando que el voto correspondiente a cada Distrito Electoral debe asignarse al candidato que obtenga mayoría simple\n"\
                    "\t--NPIVC, -pop indicando que los votos del Colegio Electoral deben asignarse al candidato que obtenga mayoría simple a nivel nacional\n"\
                    "-h, --help muestra esta ayuda y finaliza\n"

int main(int argc, char *argv[]){
    /*------------------------------------- Verificar argumentos -------------------------------------*/

    if(argc < 2 || (argc > 2 && argc < 5)) {
        printf("Falta un argumento. \nPrueba --help o -h para más información.\n");
        exit(EXIT_FAILURE);
    }

    if(argc == 2 && (COMPSTR(argv[1], "-h") == 1 || COMPSTR(argv[1], "--help") == 1)) {
        printf(helpMessage);
        exit(EXIT_SUCCESS);
    } 

    char dir[25], method[10], file[15];
    
    strcpy(dir, argv[1]);
    strcpy(file, argv[4]);

    if(COMPSTR(argv[2], "-FPTP") != 1 && COMPSTR(argv[2], "-WTA") != 1 && COMPSTR(argv[2], "-split") != 1 && COMPSTR(argv[2], "-dist") != 1 && COMPSTR(argv[2], "--NPIVC") != 1 && COMPSTR(argv[2], "-pop") != 1){
        printf("Argumento invalido. \nPrueba --help o -h para más información.\n");
    }
    strcpy(method, argv[2]);
    
    /*------------------------------------- Recorrer directorios -------------------------------------*/
    struct Reporte *inRp;
    char dst[MAX_LEN] = "", *path;

    //Inicializar reporte que representa al país
    inRp = initReport("Pais", 0);
    lvlListas[0] = snoc((void*) inRp, lvlListas[0]);
    //Concatenar nombre del archivo para conseguir path
    path = concatenateStrings(dst, "./", dir, "");
    //Leer directorios, subdirectorios y archivos .txt
    readAllDir(path, "", 0, inRp);


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