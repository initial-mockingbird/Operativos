#include "../../Utils/Closures/Closure.h"
#include "../../Utils/LinkedList/LinkedList.h"
#include "../../Proyecto1/Etapa1.h"
#include "../../Proyecto1/Modelos.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void createUK(Pais* UK){
    UK->nombre[25];
    UK->region = (char*) malloc(sizeof(6 * sizeof(char)));
    strcpy(UK->nombre,"UK");
    strcpy(UK->region,"URUP");
    UK->poblacionTotal = 10000000;
    UK->viajerosDiarios = 10;
    UK->contagiadosCierreAeropuertos = 800;
    UK->contagiadosCuarentena = 500;
    UK->contagiadosCierreNegocios = 500;
    UK->contagiadosClausuraMercados = 500;
    UK->contagiadosDetenerTransporte = 500;
    UK->claseAlta  = 3333333;
    UK->claseMedia = 3333333;
    UK->claseBaja  = 3333334;
    UK->limitacionesMedia = 0;
    UK->limitacionesBaja = 0;
    UK->compradores = 0;
    UK->mercado = 0;
    UK->infectadosClase[0] = 120;
    UK->infectadosClase[1] = 0;
    UK->infectadosClase[2] = 0;
}

void createFrance(Pais* France){
    France->nombre[25];
    France->region = (char*) malloc(sizeof(6 * sizeof(char)));
    strcpy(France->nombre,"FANJS");
    strcpy(France->region,"URUP");
    France->poblacionTotal = 20000000;
    France->viajerosDiarios = 20;
    France->contagiadosCierreAeropuertos = 1700;
    France->contagiadosCuarentena = 700;
    France->contagiadosCierreNegocios = 700;
    France->contagiadosClausuraMercados = 700;
    France->contagiadosDetenerTransporte = 700;
    France->claseAlta  = 6666668;
    France->claseMedia = 6666666;
    France->claseBaja  = 6666666;
    France->limitacionesMedia = 0;
    France->limitacionesBaja = 0;
    France->compradores = 0;
    France->mercado = 0;
    France->infectadosClase[0] = 0;
    France->infectadosClase[1] = 0;
    France->infectadosClase[2] = 0;
}


void createPoland(Pais* Poland){
    Poland->nombre[25];
    Poland->region = (char*) malloc(sizeof(6 * sizeof(char)));
    strcpy(Poland->nombre,"Kurwa");
    strcpy(Poland->region,"URUP");
    Poland->poblacionTotal = 15000000;
    Poland->viajerosDiarios = 20;
    Poland->contagiadosCierreAeropuertos = 300;
    Poland->contagiadosCuarentena = 20;
    Poland->contagiadosCierreNegocios = 20;
    Poland->contagiadosClausuraMercados = 20;
    Poland->contagiadosDetenerTransporte = 20;
    Poland->claseAlta  = 5000000;
    Poland->claseMedia = 5000000;
    Poland->claseBaja  = 5000000;
    Poland->limitacionesMedia = 0;
    Poland->limitacionesBaja = 0;
    Poland->compradores = 0;
    Poland->mercado = 0;
    Poland->infectadosClase[0] = 0;
    Poland->infectadosClase[1] = 0;
    Poland->infectadosClase[2] = 0;
}

void createURUP(Queue* paises, Region* region){
    region->paises          = paises;
    region->buzonEntrada    = emptyQ();
    region->buzonPendientes = emptyQ();
    region->buzonSalida     = emptyQ();
    strcpy(region->siglas,"URUP");
    region->tareasPendientes = 0;
    region->aliadas         = cons("LATAM",emptyQ());
}

void createMexico(Pais *mexico){
    mexico->nombre[25];
    mexico->region = (char*) malloc(sizeof(6 * sizeof(char)));
    strcpy(mexico->nombre,"Mexico");
    strcpy(mexico->region,"LATAM");
    mexico->poblacionTotal = 15000000;
    mexico->viajerosDiarios = 50;
    mexico->contagiadosCierreAeropuertos = 900;
    mexico->contagiadosCuarentena = 20;
    mexico->contagiadosCierreNegocios = 20;
    mexico->contagiadosClausuraMercados = 20;
    mexico->contagiadosDetenerTransporte = 20;
    mexico->claseAlta  = 5000000;
    mexico->claseMedia = 5000000;
    mexico->claseBaja  = 5000000;
    mexico->limitacionesMedia = 0;
    mexico->limitacionesBaja = 0;
    mexico->compradores = 0;
    mexico->mercado = 0;
    mexico->infectadosClase[0] = 0;
    mexico->infectadosClase[1] = 0;
    mexico->infectadosClase[2] = 0;
}

void createColombia(Pais *colombia){
    colombia->nombre[25];
    colombia->region = (char*) malloc(sizeof(6 * sizeof(char)));
    strcpy(colombia->nombre,"colombia");
    strcpy(colombia->region,"LATAM");
    colombia->poblacionTotal = 12500000;
    colombia->viajerosDiarios = 50;
    colombia->contagiadosCierreAeropuertos = 500;
    colombia->contagiadosCuarentena = 20;
    colombia->contagiadosCierreNegocios = 20;
    colombia->contagiadosClausuraMercados = 20;
    colombia->contagiadosDetenerTransporte = 20;
    colombia->claseAlta  = 4166666;
    colombia->claseMedia = 4166666;
    colombia->claseBaja  = 4166668;
    colombia->limitacionesMedia = 0;
    colombia->limitacionesBaja = 0;
    colombia->compradores = 0;
    colombia->mercado = 0;
    colombia->infectadosClase[0] = 0;
    colombia->infectadosClase[1] = 0;
    colombia->infectadosClase[2] = 0;
}

void createLATAM(Queue* paises, Region* region){
    region->paises          = paises;
    region->buzonEntrada    = emptyQ();
    region->buzonPendientes = emptyQ();
    region->buzonSalida     = emptyQ();
    strcpy(region->siglas,"LATAM");
    region->tareasPendientes = 0;
    region->aliadas         = cons("URUP",emptyQ());
}

void printPais(Pais* pais){
    printf("Nombre: %s\n",pais->nombre);
    printf("Region: %s\n",pais->region);
    printf("Poblacion total: %d\n", pais->poblacionTotal);
    printf("Clase alta: %f\n", pais->claseAlta);
    printf("Clase media: %f\n", pais->claseMedia);
    printf("Clase baja: %f\n", pais->claseBaja);
    printf("Infectados alta: %f\n", pais->infectadosClase[0]);
    printf("Infectados media: %f\n", pais->infectadosClase[1]);
    printf("Infectados baja: %f\n", pais->infectadosClase[2]);
}


int main(int argc, char const *argv[])
{
    // ---------------------- 
    // |      TEST 1        |
    // ---------------------- 

    Pais* UK = (Pais*) malloc(sizeof(Pais));
    
    createUK(UK);
    
    Pais* France = (Pais*) malloc(sizeof(Pais));
    createFrance(France);

    Pais* Poland = (Pais*) malloc(sizeof(Pais));
    createPoland(Poland);

    Queue* paisesURUP = emptyQ();
    paisesURUP = cons(UK,paisesURUP);
    paisesURUP = cons(France,paisesURUP);
    paisesURUP = cons(Poland,paisesURUP);

    Region* regionURUP = (Region*) malloc(sizeof(Region));
    createURUP(paisesURUP, regionURUP);

    Pais* Mexico = (Pais*) malloc(sizeof(Pais));
    createMexico(Mexico);


    Pais* Colombia = (Pais*) malloc(sizeof(Pais));
    createColombia(Colombia);

    Queue* paisesLATAM = emptyQ();
    paisesLATAM = cons(Colombia,paisesLATAM);
    paisesLATAM = cons(Mexico,paisesLATAM);

    Region* regionLATAM = (Region*) malloc(sizeof(Region));
    createLATAM(paisesLATAM, regionLATAM);


    Mundo* mundo = (Mundo*) malloc(sizeof(Mundo));
    mundo->buzonMundo = emptyQ();
    mundo->regiones = cons(regionURUP,emptyQ());
    mundo->regiones = cons(regionLATAM,mundo->regiones);

    
    

    printf("\n");
    printf("----------------------\n|      TEST 1        |\n----------------------\n\n");
    printf("Despues de correr la etapa 1:\n\n");
    
    etapa1(mundo);

    printf("Datos de UK despues de la etapa 1:\n");
    printPais(UK);
    printf("\n\n");
    
    printf("Datos de France despues de la etapa 1:\n");
    printPais(France);
    printf("\n\n");

    printf("Datos de Poland despues de la etapa 1:\n");
    printPais(Poland);
    printf("\n\n");

    printf("Datos de Mexico despues de la etapa 1:\n");
    printPais(Mexico);
    printf("\n\n");

    printf("Datos de Colombia despues de la etapa 1:\n");
    printPais(Colombia);
    printf("\n\n");

    // ---------------------- 
    // |      TEST 2        |
    // ---------------------- 

    UK = (Pais*) malloc(sizeof(Pais));
    
    createUK(UK);
    
    France = (Pais*) malloc(sizeof(Pais));
    createFrance(France);

    Poland = (Pais*) malloc(sizeof(Pais));
    createPoland(Poland);

    paisesURUP = emptyQ();
    paisesURUP = cons(UK,paisesURUP);
    paisesURUP = cons(France,paisesURUP);
    paisesURUP = cons(Poland,paisesURUP);

    regionURUP = (Region*) malloc(sizeof(Region));
    createURUP(paisesURUP, regionURUP);

    regionURUP->aliadas = emptyQ();

    UK->contagiadosCierreAeropuertos = 1;
    UK->infectadosClase[1] = 50;

    France->contagiadosCierreAeropuertos = 1;
    France->infectadosClase[0] = 20;

    Poland->contagiadosCierreAeropuertos = 1;
    Poland->infectadosClase[2] = 30;


    mundo = (Mundo*) malloc(sizeof(Mundo));
    mundo->buzonMundo = emptyQ();
    mundo->regiones = cons(regionURUP,emptyQ());

    printf("\n");
    printf("----------------------\n|      TEST 2        |\n----------------------\n\n");
    printf("Una nacion sin aliados y con todos sus paises en cuarentena no cambia los datos de sus paises:\n\n");

    etapa1(mundo);
    printf("Datos de UK despues de la etapa 1:\n");
    printPais(UK);
    printf("\n\n");

    printf("Datos de France despues de la etapa 1:\n");
    printPais(France);
    printf("\n\n");

    printf("Datos de Poland despues de la etapa 1:\n");
    printPais(Poland);
    printf("\n\n");
    

    // ---------------------- 
    // |      TEST 3        |
    // ---------------------- 

    UK = (Pais*) malloc(sizeof(Pais));
    
    createUK(UK);
    
    France = (Pais*) malloc(sizeof(Pais));
    createFrance(France);

    Poland = (Pais*) malloc(sizeof(Pais));
    createPoland(Poland);

    paisesURUP = emptyQ();
    paisesURUP = cons(UK,paisesURUP);
    paisesURUP = cons(France,paisesURUP);
    paisesURUP = cons(Poland,paisesURUP);

    regionURUP = (Region*) malloc(sizeof(Region));
    createURUP(paisesURUP, regionURUP);

    Mexico = (Pais*) malloc(sizeof(Pais));
    createMexico(Mexico);


    Colombia = (Pais*) malloc(sizeof(Pais));
    createColombia(Colombia);

    paisesLATAM = emptyQ();
    paisesLATAM = cons(Colombia,paisesLATAM);
    paisesLATAM = cons(Mexico,paisesLATAM);

    regionLATAM = (Region*) malloc(sizeof(Region));
    createLATAM(paisesLATAM, regionLATAM);


    mundo = (Mundo*) malloc(sizeof(Mundo));
    mundo->buzonMundo = emptyQ();
    mundo->regiones = cons(regionURUP,emptyQ());
    mundo->regiones = cons(regionLATAM,mundo->regiones);

    printf("\n");
    printf("----------------------\n|      TEST 3        |\n----------------------\n\n");
    printf("Hagamos 365 iteraciones\n\n");
    for (int i = 0; i < 359; i++){
        etapa1(mundo);
    }

    //etapa1(mundo);
    //etapa1(mundo);
    //etapa1(mundo);

    printf("Datos de UK despues de la etapa 1:\n");
    printPais(UK);
    printf("\n\n");
    
    printf("Datos de France despues de la etapa 1:\n");
    printPais(France);
    printf("\n\n");

    printf("Datos de Poland despues de la etapa 1:\n");
    printPais(Poland);
    printf("\n\n");

    printf("Datos de Mexico despues de la etapa 1:\n");
    printPais(Mexico);
    printf("\n\n");

    printf("Datos de Colombia despues de la etapa 1:\n");
    printPais(Colombia);
    printf("\n\n");



    return EXIT_SUCCESS;
}
