#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "Readdir.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    struct Reporte* inRp;

    inRp = initReport("Pais", 0);
    lvlListas[0] = snoc((void*) inRp, lvlListas[0]);
    
    readAllDir("./0", "", 0, inRp);

    //Test
    Queue *q = lvlListas[0], *k;
    while(q){
        printf("Directorio: %s\n", ((Reporte *) head(q))->nombre);
        printf("cand1: %ld\n", ((Reporte *) head(q))->cand1);
        printf("subCounts: %d\n", ((Reporte *) head(q))->subCounts);
        k = ((Reporte *) head(q))->subdirs;
        while(k){
            printf("Subdirectorio: %s\n", ((Reporte *) head(k))->nombre);
            k = tail(k);
        }
        q = tail(q);
    }
    return EXIT_SUCCESS;
}