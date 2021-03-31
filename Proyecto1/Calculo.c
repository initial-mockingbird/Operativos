#include "../Utils/LinkedList/LinkedList.h"
#include "./Region.h"
#include "./Paises.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 50
int days = 0;
int tasaContagio = 0;

MensajeInformacional *contagioPais(struct pais *pais){
    MensajeInformacional *msj = malloc(sizeof(struct MensajeInformacional));
    ME *ME = msj->MI->ME;
    MR *MR = msj->MI->MR;

    //Mensaje Reporte
    MR->pais = pais->nombre;

    if(!days){
        MR->altaNuevosInfectados = 0;
        MR->mediaNuevosInfectados = 0;
        MR->bajaNuevosInfectados = 0;
        MR->altaNuevosMuertos = 0;
        MR->mediaNuevosMuertos = 0;
        MR->bajaNuevosMuertos = 0;
    }else{
        ;
    }

    MR->fecha = 0;
    MR->totalNuevosInfectados = (MR->altaNuevosInfectados) + (MR->mediaNuevosInfectados) + (MR->bajaNuevosInfectados);
    MR->totalNuevosMuertos = 0;

    //Mensaje Eventual
    ME->pais = pais->nombre;
    ME->tipoHito = 0;         // Modificar

    return msj;
}

/**
 * @brief Guarda la informacion correspondiente a un pais en un archivo .txt.
 * @param fp
 * @param pais
 * @param MR
 * @param dato
 * @param estrato
 */
void printPais(FILE *fp, Pais *p, MR *MR, char dato, char *estrato){
    // dato: i (infectados), m (muertos)
    // estrato: Alta, Media, Baja
    double aux;
    switch(dato){
        case 'i':
            if(COMPSTR(estrato, "Alta")){
                aux = ((MR -> altaNuevosInfectados) * 100) / (poblacionTotal(p) * claseAlta(p));
            }else if(COMPSTR(estrato, "Media")){
                aux = ((MR -> mediaNuevosInfectados) * 100) / (poblacionTotal(p) * claseMedia(p));
            }else if(COMPSTR(estrato, "Baja")){
                aux = ((MR -> bajaNuevosInfectados) * 100) / (poblacionTotal(p) * claseBaja(p));
            }
            fprintf(fp, "%s, %s, nuevos infectados, %d, %0.3f\n", MR->pais, estrato, MR->fecha, aux);
            return;
        case 'm':
            if(COMPSTR(estrato, "Alta")){
                aux = ((MR -> altaNuevosMuertos) * 100) / (poblacionTotal(p) * claseAlta(p));
            }else if(COMPSTR(estrato, "Media")){
                aux = ((MR -> mediaNuevosMuertos) * 100) / (poblacionTotal(p) * claseMedia(p));
            }else if(COMPSTR(estrato, "Baja")){
                aux = ((MR -> bajaNuevosMuertos) * 100) / (poblacionTotal(p) * claseBaja(p));
            }
            fprintf(fp, "%s, %s, nuevos muertos, %d, %0.3f\n", MR->pais, estrato, MR->fecha, aux);
            return;
    }
}

/**
 * @brief Guarda la informacion global e información correspondiente a cada pais en un archivo .txt.
 * @param mundo
 * @param filename
 * @return int 
 */
int print(Mundo *mundo, char *fileName[]){
    FILE *fp;
    MensajeInformacional *msj = mundo->eventos;   
    MR *MR;
    Pais *pais;
    double globalInfectados = 0, globalMuertos = 0;
    char buffer[MAX_LEN];
    
    // Abrir archivo en modo append+, si no existe se crea
    strcpy(buffer, fileName);
    strcat(buffer, ".txt");
    fp = fopen(buffer, "a+");
    // Si ocurre un error al abrir 
    if(fp) return EXIT_FAILURE;

    // Imprimir todos los mensajes informacionales
    while(msj){
        MR = msj->MI->MR;
        pais = lookupByName(MR->pais);

        //Guardar información sobre el país en el archivo
        printPais(fp, pais, MR, 'i', "Alta");
        printPais(fp, pais, MR, 'm', "Alta");
        printPais(fp, pais, MR, 'i', "Media");
        printPais(fp, pais, MR, 'm', "Media");
        printPais(fp, pais, MR, 'i', "Baja");
        printPais(fp, pais, MR, 'm', "Baja");
        // Sumar el numero de infectados y numero de muertos por pais
        globalInfectados += (MR->totalNuevosInfectados);
        globalMuertos += (MR->totalNuevosMuertos);

        msj = msj->next;
    }
    // Imprimir informacion global
    fprintf(fp, "Global, Total, nuevos infectados, %d, %0.3f\n", MR->fecha, globalInfectados);
    fprintf(fp, "Global, Total, nuevos muertos, %d, %0.3f\n", MR->fecha, globalMuertos);
    
    fclose(fp);
    return EXIT_SUCCESS;
}