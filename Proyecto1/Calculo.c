#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/Closures/Closure.h"
#include "Modelos.h"
#include "Mensaje.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// ---------------------- 
// |      MACROS        |
// ----------------------
/** @def COMPSTR
 * @brief Compares two strings, returns 1 if they are the same, and 0 otherwise.
 * 
 */
#define COMPSTR(fst, snd) strcmp(fst,snd) == 0

/** @def MAX_LEN
 * @brief Maxima longitud de una cadena de texto.
 * 
 */
#define MAX_LEN 100

// ---------------------- 
// |      ESTADO        |
// ----------------------

extern int days = 0;
extern double tasaContagio = 0;
extern int mortalidadNoTratarla = 0;

extern Queue* pacienteCero = NULL;
extern Queue* primerMuerto = NULL;
extern Queue* libraEnfermedad = NULL;
extern Queue* enCuarentena = NULL;
extern Queue* primerCierreAereopuertos = NULL;
extern Queue* aereopuertosCerrados = NULL;
extern Queue* primerCierreNegocios = NULL;
extern Queue* negociosCerrados = NULL;
extern Queue* primerClausuraMercados = NULL;
extern Queue* mercadosCerrados = NULL;
extern Queue* primerTransporteDetenido = NULL;
extern Queue* transportesDetenidos = NULL;

// ---------------------- 
// |    AUX FUNCTIONS   |
// ---------------------- 
/**
 * @brief Aplica la funcion techo y convierte en long long.
 * @param dato
 * @return double
 */
long long ceiLL(double dato){
    return (long long) ceiLL(dato);
}

/**
 * @brief Saca el porcentaje de un dato con respecto a otro.
 * @param partial 
 * @param total
 * @return double
 */
double porcentaje(long long partial, long long total){
    return (double) (partial * 100) / total;
}

/**
 * @brief Saca la porción de un dato con respecto a un porcentaje.
 * @param partial 
 * @param total
 * @return double
 */
double porcentajeR(double partial, long long total){
    return ceiLL((partial * total) / 100);
}

/**
 * @brief Busca si un string aparece en una lista.
 * @param q
 * @param name
 * @return bool
 */
bool searchQueue(Queue *q, char *name){
    while(q){
        if(head(q) && COMPSTR(name, (char*) head(q))) return true;
        q = tail(q);
    }
    return false;
}

/**
 * @brief Retorna una nueva lista, sin el elemento pasado como argumento.
 * @param q
 * @param name
 * @return Queue*
 */
Queue *withoutNode(Queue *q, char *name){
    Queue *qq = emptyQ();

    while(q){
        if(head(q) && COMPSTR(name, (char*) head(q))) continue;
        qq = snoc((void*) head(q), qq);
        q = tail(q);
    }

    return qq;
}

/**
 * @brief Chequea si un pais tiene algún caso de contagio en un momento determinado.
 * @param p
 * @return bool
 */
bool firstCase(Pais *p){
    //Si no hay infectados, retorna false
    return ((altosInfectados(p) > 0) || (mediaInfectados(p) > 0) || (bajaInfectados(p) > 0));
}

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Guarda informacion del mensaje de reporte de un pais en un archivo .txt.
 * @param fp
 * @param pais
 * @param MR
 * @param dato
 * @param estrato
 */
void printMR(FILE *fp, Pais *p, MR *MR, char dato, char *estrato){
    // dato: i (infectados), m (muertos)
    // estrato: Alta, Media, Baja
    double aux;
    switch(dato){
        case 'i':
            if(COMPSTR(estrato, "Alta")){
                aux = porcentaje(altaNuevosInfectados(MR), ceiLL(poblacionTotal(p) * claseAlta(p)));
            }else if(COMPSTR(estrato, "Media")){
                aux = porcentaje(mediaNuevosInfectados(MR), ceiLL(poblacionTotal(p) * claseMedia(p)));
            }else if(COMPSTR(estrato, "Baja")){
                aux = porcentaje(bajaNuevosInfectados(MR), ceiLL(poblacionTotal(p) * claseBaja(p)));
            }
            fprintf(fp, "%s, %s, nuevos infectados, %d, %0.3f\n", MR->pais, estrato, 0, aux);   //Modificar <Agregar fecha>
            return;
        case 'm':
            if(COMPSTR(estrato, "Alta")){
                aux = porcentaje(altaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseAlta(p)));
            }else if(COMPSTR(estrato, "Media")){
                aux = porcentaje(mediaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseMedia(p)));
            }else if(COMPSTR(estrato, "Baja")){
                aux = porcentaje(bajaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseBaja(p)));
            }
            fprintf(fp, "%s, %s, nuevos muertos, %d, %0.3f\n", MR->pais, estrato, 0, aux);      //Modificar <Agregar fecha>
            return;
    }
}

/**
 * @brief Guarda informacion del mensaje eventual de un pais en un archivo .txt.
 * @param fp
 * @param pais
 * @param ME
 */
void printME(FILE *fp, Pais *p, ME *ME){
    // tipoHito:    
    //      0. Pais recibe paciente cero                    7. Un país cierra sus negocios por 1ra vez    	
    //      1. Un país tiene su primer muerto               8. Un país reabre sus negocios
    //      2. Un país se libra de la enfermedad            9. Un país clausura sus mercados por 1ra vez
    //      3. Un país entra en cuarentena                  10. Un país reabre sus mercados
    //      4. Un país sale de cuarentena                   11. Un  país  detiene  su  transporte  publico  por 1ra vez
    //      5. Un país cierra sus aeropuertos por 1ra vez   12. Un país reactiva su transporte publico
    //      6. Un país reabre sus aeropuertos

    char Hitos[13][MAX_LEN] = {"recibe paciente cero", "tiene su primer muerto", "es libre de la enfermedad",
                    "entra en cuarentena", "sale de cuarentena", "cierra sus aereopusetos por primera vez",
                    "reabre sus aereopuertos", "cierra sus negocios por primera vez",
                    "reabre sus negocios", "clausura sus mercados por primera vez", "reabre sus mercados",
                    "detiene sus transportes publicos por primera vez", "reactiva su transporte publico"};

    fprintf(fp, "%s %s %d.\n", ME->pais, Hitos[tipoHito(ME)], 0);     //Modificar <Agregar fecha>
}

/**
 * @brief Guarda la informacion global e información correspondiente a cada pais en un archivo .txt.
 * @param mundo
 * @param filename
 * @return int 
 */
int print(Mundo *mundo, char *fileName[]){
    MensajeInformacional *msj;
    FILE *fp, *bi;  
    Pais *pais;  
    ME *ME;
    MR *MR;
    Queue *q = mundo->eventos;  
    long long globalInfectados = 0, globalMuertos = 0;
    char buffer[MAX_LEN];
    
    // Abrir archivo para reportes por pais en modo append+, si no existe se crea
    strcpy(buffer, fileName);
    strcat(buffer, ".txt");
    fp = fopen(buffer, "a+");
    // Si ocurre un error al abrir 
    if(fp) return EXIT_FAILURE;
    // Abrir archivo para hitos en modo append+, si no existe se crea
    bi = fopen("BoletinInformativo.txt", "a+");
    // Si ocurre un error al abrir 
    if(bi) return EXIT_FAILURE;

    // Imprimir/Guardar los mensajes informacionales
    while(q){
        msj = (MensajeInformacional*) head(q);
        if(msj->tipo == 0){
            ME = (msj->mensaje)->eventualidad;
            pais = lookupByName(mundo, ME->pais);
            
            //Guardar informacion sobre un evento en cierto pais en el archivo
            printME(bi, pais, ME);
            
        }else if(msj->tipo == 1){
            MR = (msj->mensaje)->reporteDiario;
            pais = lookupByName(mundo, MR->pais);

            //Guardar información sobre el país en el archivo
            if(days % 7 == 0){
                printMR(fp, pais, MR, 'i', "Alta");
                printMR(fp, pais, MR, 'm', "Alta");
                printMR(fp, pais, MR, 'i', "Media");
                printMR(fp, pais, MR, 'm', "Media");
                printMR(fp, pais, MR, 'i', "Baja");
                printMR(fp, pais, MR, 'm', "Baja");
            }
            
            // Sumar el numero de infectados y numero de muertos por pais
            globalInfectados += (MR->totalNuevosInfectados);
            globalMuertos += (MR->totalNuevosMuertos);
        }
        q = tail(q);
    }
    // Imprimir informacion global
    fprintf(fp, "Global, Total, nuevos infectados, %d, %lld\n", 0, globalInfectados);   //Modificar <Agregar fecha>
    fprintf(fp, "Global, Total, nuevos muertos, %d, %lld\n", 0, globalMuertos);         //Modificar <Agregar fecha>
    
    fclose(fp);
    fclose(bi);

    mundo->eventos = NULL;

    return EXIT_SUCCESS;
}

/**
 * @brief Calcula el contagio de un pais.
 * @param p
 * @param msj
 * @return struct MensajeInformacional*
 */
MensajeInformacional *contagioPais(Mundo *mundo, char *p){
    MensajeInformacional *msj = malloc(sizeof(struct MensajeInformacional));
    MR *MR = (msj->mensaje)->reporteDiario;
    Pais *p;
    long long tratado, ntratado;
    
    p = lookupByName(mundo, p);
    //------------------------------------ Mensaje Reporte ------------------------------------//
    MR->pais = nombre(p);

    // Nuevos infectados por clase
    MR->altaNuevosInfectados = abs(ceiLL(altosInfectados(p) * tasaContagio) - altaInfectados(p));   
    MR->mediaNuevosInfectados = abs(ceiLL(mediaInfectados(p) * tasaContagio) - mediaInfectados(p));
    MR->bajaNuevosInfectados = abs(ceiLL(bajaInfectados(p) * tasaContagio) - bajaInfectados(p));

    // Nuevos muertos por clase
    MR->altaNuevosMuertos = ceiLL(altaInfectados(p)) * ((double) 1/8);

    ntratado = ceiLL(mediaInfectados(p) * limitacionesMedia(p));
    tratado = mediaInfectados(p) - ntratado;
    MR->mediaNuevosMuertos = ceiLL(tratado * 0.25) + ceiLL(ntratado * mortalidadNoTratarla);

    ntratado = ceiLL(bajaInfectados(p) * limitacionesBaja(p));
    tratado = bajaInfectados(p) - ntratado;
    MR->bajaNuevosMuertos = ceiLL(tratado * 0.5) + ceiLL(ntratado * mortalidadNoTratarla);

    // Actualizar datos restantes de MR
    //MR->fecha = 0;      //Modificar <Agregar fecha>
    MR->totalNuevosInfectados += (MR->altaNuevosInfectados) + (MR->mediaNuevosInfectados) + (MR->bajaNuevosInfectados);
    MR->totalNuevosMuertos += (MR->altaNuevosMuertos) + (MR->mediaNuevosMuertos) + (MR->bajaNuevosMuertos);

    //------------------------------------ Datos Pais p ------------------------------------//
    // Actualizar la poblacion del pais p
    actualizarClaseAlta(p, - porcentaje(altaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseAlta(p))));
    actualizarClaseMedia(p, - porcentaje(mediaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseMedia(p))));
    actualizarClaseBaja(p, - porcentaje(bajaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseBaja(p))));
    actualizarPoblacionTotal(p, - (MR->totalNuevosMuertos));
    // p->claseAlta -= porcentaje(altaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseAlta(p)));
    // p->claseMedia -= porcentaje(mediaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseMedia(p)));
    // p->claseMedia -= porcentaje(bajaNuevosMuertos(MR), ceiLL(poblacionTotal(p) * claseBaja(p)));
    // p->poblacionTotal -= MR->totalNuevosMuertos;

    // Actualizar cantidad de contagiados del pais p 
    //p->altosInfectados += (MR->altaNuevosInfectados) - (MR->altaNuevosMuertos);
    // p->mediaInfectados += (MR->mediaNuevosInfectados) - (MR->mediaNuevosMuertos);
    // p->bajaInfectados += (MR->bajaNuevosInfectados) - (MR->bajaNuevosMuertos);
    actualizarAltosInfectados(p, porcentaje((MR->altaNuevosInfectados) - (MR->altaNuevosMuertos), ceiLL(poblacionTotal(p) * claseAlta(p))));
    actualizarMediaInfectados(p, porcentaje((MR->mediaNuevosInfectados) - (MR->mediaNuevosMuertos), ceiLL(poblacionTotal(p) * claseMedia(p))));
    actualizarBajaInfectados(p, porcentaje((MR->bajaNuevosInfectados) - (MR->bajaNuevosMuertos), ceiLL(poblacionTotal(p) * claseBaja(p))));

    return msj;
}

/**
 * @brief Dado un hito, genera un MensajeInformacional.
 * @param pais
 * @param tipoHito
 * @return struct MensajeInformacional*
 */
MensajeInformacional *writeME(char *pais, int tipoHito){
    MensajeInformacional *msj = malloc(sizeof(struct MensajeInformacional));
    ME *ME = (msj->mensaje)->eventualidad; 
    //Asignar la informarcion al ME
    ME->pais = pais;
    ME->tipoHito = tipoHito;

    return msj;
}

/**
 * @brief Dado un pais, genera un todos los MensajeInformacionales con los hitos correspondientes.
 * @param eventos
 * @param p
 * @return struct Queue*
 */
Queue *hitoPais(Queue *eventos, Pais *p){
    // tipoHito:    
    //      0. Pais recibe paciente cero                    7. Un país cierra sus negocios por 1ra vez    	
    //      1. Un país tiene su primer muerto               8. Un país reabre sus negocios
    //      2. Un país se libra de la enfermedad            9. Un país clausura sus mercados por 1ra vez
    //      3. Un país entra en cuarentena                  10. Un país reabre sus mercados
    //      4. Un país sale de cuarentena                   11. Un  país  detiene  su  transporte  publico  por 1ra vez
    //      5. Un país cierra sus aeropuertos por 1ra vez   12. Un país reactiva su transporte publico
    //      6. Un país reabre sus aeropuertos

    long long aux;
    //2. Un país se libra de la enfermedad
    if(!searchQueue(libraEnfermedad, nombre(p)) && searchQueue(pacienteCero, nombre(p)) && firstCase(p)){
        libraEnfermedad = snoc((void*) nombre(p), libraEnfermedad);
        eventos = snoc((void*) writeME(nombre(p), 2), eventos);
    }
    //0. Pais recibe paciente cero
    if(!searchQueue(pacienteCero, nombre(p)) && !firstCase(p)){
        pacienteCero = snoc((void*) nombre(p), pacienteCero);
        eventos = snoc((void*) writeME(nombre(p), 0), eventos);
    }
    //1. Un país tiene su primer muerto 
    if(!searchQueue(primerMuerto, nombre(p)) && (totalMuertos(p) > 0)){
        primerMuerto = snoc((void*) nombre(p), primerMuerto);
        eventos = snoc((void*) writeME(nombre(p), 1), eventos);
    }
    //4. Un país sale de cuarentena
    aux = altosInfectados(p) + mediaInfectados(p) + bajaInfectados(p);
    if(!searchQueue(enCuarentena, nombre(p)) && (aux >= contagiadosCuarentena(p))){
        enCuarentena = snoc((void*) nombre(p), enCuarentena);
        eventos = snoc((void*) writeME(nombre(p), 4), eventos);
    }
    //5. Un país cierra sus aeropuertos por 1ra vez
    if(!searchQueue(primerCierreAereopuertos, nombre(p)) && (aux >= contagiadosCierreAeropuertos(p))){
        primerCierreAereopuertos = snoc((void*) nombre(p), primerCierreAereopuertos);
        eventos = snoc((void*) writeME(nombre(p), 5), eventos);
    }
    //6. Un país reabre sus aeropuertos
    if(searchQueue(aereopuertosCerrados, nombre(p)) && (aux < contagiadosCierreAeropuertos(p))){
        aereopuertosCerrados = withoutNode(aereopuertosCerrados, nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 6), eventos);
    }else if(!searchQueue(aereopuertosCerrados, nombre(p)) && (aux >= contagiadosCierreAeropuertos(p))){
        aereopuertosCerrados = snoc((void*) nombre(p), aereopuertosCerrados);
    }
    // 7. Un país cierra sus negocios por 1ra vez 
    if(!searchQueue(primerCierreNegocios, nombre(p)) && (aux >= contagiadosCierreNegocios(p))){
        primerCierreNegocios = snoc((void*) nombre(p), primerCierreNegocios);
        eventos = snoc((void*) writeME(nombre(p), 7), eventos);
    }
    //8. Un país reabre sus negocios
    if(searchQueue(negociosCerrados, nombre(p)) && (aux < contagiadosCierreNegocios(p))){
        negociosCerrados = withoutNode(negociosCerrados, nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 8), eventos);
    }else if(!searchQueue(negociosCerrados, nombre(p)) && (aux >= contagiadosCierreNegocios(p))){
        negociosCerrados = snoc((void*) nombre(p), negociosCerrados);
    }
    //9. Un país clausura sus mercados por 1ra vez
    if(!searchQueue(primerClausuraMercados, nombre(p)) && (aux >= contagiadosClausuraMercados(p))){
        primerClausuraMercados = snoc((void*) nombre(p), primerClausuraMercados);
        eventos = snoc((void*) writeME(nombre(p), 9), eventos);
    }
    //10. Un país reabre sus mercados
    if(searchQueue(mercadosCerrados, nombre(p)) && (aux < contagiadosClausuraMercados(p))){
        mercadosCerrados = withoutNode(mercadosCerrados, nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 10), eventos);
    }else if(!searchQueue(mercadosCerrados, nombre(p)) && (aux >= contagiadosClausuraMercados(p))){
        mercadosCerrados = snoc((void*) nombre(p), mercadosCerrados);
    }
    //11. Un  país  detiene  su  transporte  publico  por 1ra vez
    if(!searchQueue(primerTransporteDetenido, nombre(p)) && (aux >= contagiadosDetenerTransporte(p))){
        primerTransporteDetenido = snoc((void*) nombre(p), primerTransporteDetenido);
        eventos = snoc((void*) writeME(nombre(p), 11), eventos);
    }
    //12. Un país reactiva su transporte publico
    if(searchQueue(transportesDetenidos, nombre(p)) && (aux < contagiadosDetenerTransporte(p))){
        transportesDetenidos = withoutNode(transportesDetenidos, nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 12), eventos);
    }else if(!searchQueue(transportesDetenidos, nombre(p)) && (aux >= contagiadosDetenerTransporte(p))){
        transportesDetenidos = snoc((void*) nombre(p), transportesDetenidos);
    }

    return eventos;
}

/**
 * @brief Calcula el contagio por region de todos los paises.
 * @param mundo
 */
void calculoContagio(Mundo *mundo){
    Queue *eventos = emptyQ();    
    Queue *r = mundo->regiones;
    Pais *pais;
    Queue *pp;

    while(r){
        pp = ((Region*)r)->paises;
        while(pp){
            pais = (Pais*) head(pp);

            //Si el pais no ha recibido su primer contagiado, continue
            if(!firstCase(pais)) continue;
            //Caso contrario, calcular el contagio
            mundo->eventos = snoc((void*) contagioPais(mundo, pais), eventos);
            //Generar mensajes de eventualidad
            mundo->eventos = hitoPais(mundo->eventos, pais);

            pp = tail(pp);
        }
        r = tail(r);
    }
    days += 1;
}

