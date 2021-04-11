#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "Modelos.h"
#include "Mensaje.h"
#include "Calculo.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

// ---------------------- 
// |      MACROS        |
// ----------------------
/** @def MAX_LEN
 * @brief Maxima longitud de una cadena de texto.
 * 
 */
#define MAX_LEN 100

// ---------------------- 
// |    AUX FUNCTIONS   |
// ---------------------- 
/**
 * @brief Aplica la funcion techo y convierte en long long.
 * @param dato
 * @return double
 */
long long ceiLL(double dato){
    return (long long) ceil(dato);
}

/**
 * @brief Saca el porcentaje de un dato con respecto a otro.
 * @param partial 
 * @param total
 * @return double
 */
double porcentaje(long long partial, long long total){
    return (double) (partial * 100) / (double) total;
}

/**
 * @brief Saca la porción de un dato con respecto a un porcentaje.
 * @param partial 
 * @param total
 * @return long long
 */
long long porcentajeR(double partial, long long total){
    return ceiLL((double) (partial * total) / 100);
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
int firstCase(Pais *p){
    //Si no hay infectados, retorna false 
    
    return ( (altaInfectados(p) > 0) || (mediaInfectados(p) > 0) || (bajaInfectados(p) > 0) );
}

/**
 * @brief Añade un día a una fecha determinada.
 * @param date
 * @return bool
 */
struct tm *addDay(struct tm* date){
    time_t base;
    // Agregar un día
    date->tm_mday += 1;
    base = mktime(date);

    return localtime(&base);
}

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Guarda informacion del mensaje de reporte de un pais en un archivo .txt.
 * @param fp
 * @param p
 * @param MR
 * @param dato
 * @param estrato
 */
void printMR(FILE *fp, Pais *p, MR *MR, char dato, char *estrato){
    // dato: i (infectados), m (muertos)
    // estrato: Alta, Media, Baja
    char buffer[MAX_LEN];
    double aux;
    switch(dato){
        case 'i':
            if(COMPSTR(estrato, "Alta")){
                aux = altaNuevosInfectados(MR);
            }else if(COMPSTR(estrato, "Media")){
                aux = mediaNuevosInfectados(MR);
            }else if(COMPSTR(estrato, "Baja")){
                aux = bajaNuevosInfectados(MR);
            }
            strftime(buffer, MAX_LEN, "%d/%m/%Y", MR->fecha);
            fprintf(fp, "%s, %s, nuevos infectados, %s, %0.3f\n", MR->pais, estrato, buffer, aux);   
            return;
        case 'm':
            if(COMPSTR(estrato, "Alta")){
                aux = altaNuevosMuertos(MR);
            }else if(COMPSTR(estrato, "Media")){
                aux = mediaNuevosMuertos(MR);
            }else if(COMPSTR(estrato, "Baja")){
                aux = bajaNuevosMuertos(MR);
            }
            strftime(buffer, MAX_LEN, "%d/%m/%Y", MR->fecha);
            fprintf(fp, "%s, %s, nuevos muertos, %s, %0.3f\n", MR->pais, estrato, buffer, aux);      
            return;
    }
}

/**
 * @brief Guarda informacion del mensaje eventual de un pais en un archivo .txt.
 * @param fp
 * @param ME
 */
void printME(FILE *fp, ME *ME){
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
    char buffer[MAX_LEN];
    strftime(buffer, MAX_LEN, "%d/%m/%Y", ME->fecha);
    fprintf(fp, "%s %s %s.\n", ME->pais, Hitos[ME->tipoHito], buffer);     
}

/**
 * @brief Guarda la informacion global e información correspondiente a cada pais en un archivo .txt.
 * @param mundo
 * @param filename
 * @param days
 * @return int 
 */
int print(Mundo *mundo, char *fileName, int days){
    MensajeInformacional *msj;
    struct tm *date;
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
            //Guardar informacion sobre un evento en cierto pais en el archivo
            printME(bi, ME);
            
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
                date = MR->fecha;
            }
            
            // Sumar el numero de infectados y numero de muertos por pais
            globalInfectados += (MR->totalNuevosInfectados);
            globalMuertos += (MR->totalNuevosMuertos);
        }
        q = tail(q);
    }
    // Imprimir informacion global
    strftime(buffer, MAX_LEN, "%d/%m/%Y", date);
    fprintf(fp, "Global, Total, nuevos infectados, %s, %lld\n", buffer, globalInfectados);   
    fprintf(fp, "Global, Total, nuevos muertos, %s, %lld\n", buffer, globalMuertos);         
    
    fclose(fp);
    fclose(bi);

    mundo->eventos = NULL;

    return EXIT_SUCCESS;
}

/**
 * @brief Calcula el contagio de un pais.
 * @param p
 * @param msj
 * @param listas
 * @param tasaContagio
 * @param mortalidadNoTratarla
 * @param date
 * @return struct MensajeInformacional*
 */
MensajeInformacional *contagioPais(Mundo *mundo, Pais *p, Queue *listas[], double tasaContagio, double mortalidadNoTratarla, struct tm *date){
    
    MensajeInformacional *msj = malloc(sizeof(struct MensajeInformacional));
    
    MR *MR = &((msj->mensaje)->reporteDiario);

    
    long long tratado, ntratado, aux;
    
    msj->tipo = 1;
    
    //------------------------------------ Mensaje Reporte ------------------------------------//
    MR->pais = (char*) malloc(26*sizeof(char));
    printf("%s\n",p->nombre);
    strcpy(MR->pais,p->nombre);
    
    // Nuevos infectados por clase
    aux = abs(ceiLL(altaInfectados(p) * tasaContagio) - altaInfectados(p));
    MR->altaNuevosInfectados = porcentaje(aux, ceiLL(poblacionTotal(p) * claseAlta(p)));  
    MR->totalNuevosInfectados = aux;
    
    aux = abs(ceiLL(mediaInfectados(p) * tasaContagio) - mediaInfectados(p));
    MR->mediaNuevosInfectados = porcentaje(aux, ceiLL(poblacionTotal(p) * claseMedia(p)));
    MR->totalNuevosInfectados += aux;

    aux = abs(ceiLL(bajaInfectados(p) * tasaContagio) - bajaInfectados(p));
    MR->bajaNuevosInfectados = porcentaje(aux, ceiLL(poblacionTotal(p) * claseBaja(p)));
    MR->totalNuevosInfectados += aux;

    // Nuevos muertos por clase
    aux = ceiLL(altaInfectados(p) * ((double) 1/8));
    MR->altaNuevosMuertos = porcentaje(aux, ceiLL(poblacionTotal(p) * claseAlta(p)));
    MR->totalNuevosMuertos = aux;

    ntratado = ceiLL(mediaInfectados(p) * limitacionesMedia(p));
    tratado = mediaInfectados(p) - ntratado;
    aux = ceiLL(tratado * 0.25) + ceiLL(ntratado * mortalidadNoTratarla);
    MR->mediaNuevosMuertos = porcentaje(aux, ceiLL(poblacionTotal(p) * claseMedia(p)));
    MR->totalNuevosMuertos += aux;

    ntratado = ceiLL(bajaInfectados(p) * limitacionesBaja(p));
    tratado = bajaInfectados(p) - ntratado;
    aux = ceiLL(tratado * 0.5) + ceiLL(ntratado * mortalidadNoTratarla);
    MR->bajaNuevosMuertos = porcentaje(aux, ceiLL(poblacionTotal(p) * claseBaja(p)));
    MR->totalNuevosMuertos += aux;

    
    // Actualizar datos restantes de MR
    MR->fecha = date;     

    //------------------------------------ Datos Pais p ------------------------------------//
    // Actualizar la poblacion del pais p
    actualizarClaseAlta (p, - altaNuevosMuertos(MR));
    actualizarClaseMedia(p, - mediaNuevosMuertos(MR));
    actualizarClaseBaja (p, - bajaNuevosMuertos(MR));
    actualizarPoblacionTotal(p, - (MR->totalNuevosMuertos));

    // Actualizar cantidad de contagiados del pais p 
    actualizarAltaInfectados (p, (double) poblacionTotal(p) * ((MR->altaNuevosInfectados)  - (MR->altaNuevosMuertos)));
    actualizarMediaInfectados(p, (double) poblacionTotal(p) * ((MR->mediaNuevosInfectados) - (MR->mediaNuevosMuertos)));
    actualizarBajaInfectados (p, (double) poblacionTotal(p) * ((MR->bajaNuevosInfectados)  - (MR->bajaNuevosMuertos)));

    if(!searchQueue(listas[1], nombre(p))){
        if((MR->altaNuevosMuertos > 0) || (MR->mediaNuevosMuertos > 0) || (MR->bajaNuevosMuertos > 0)){
            listas[1] = snoc((void*) nombre(p), listas[1]);
        }
    }
    
    return msj;
}

/**
 * @brief Dado un hito, genera un MensajeInformacional.
 * @param pais
 * @param tipoHito
 * @param date
 * @return struct MensajeInformacional*
 */
MensajeInformacional *writeME(char *pais, int tipoHito, struct tm *date){
    MensajeInformacional *msj = malloc(sizeof(struct MensajeInformacional));
    ME *ME = (msj->mensaje)->eventualidad; 
    msj->tipo = 0;
    //Asignar la informarcion al ME
    ME->pais = pais;
    ME->tipoHito = tipoHito;
    ME->fecha = date;

    return msj;
}

/**
 * @brief Dado un pais, genera un todos los MensajeInformacionales con los hitos correspondientes.
 * @param eventos
 * @param p
 * @param listas
 * @param date
 * @return struct Queue*
 */
Queue *hitoPais(Queue *eventos, Pais *p, Queue *listas[], struct tm* date){
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
    if(!searchQueue(listas[3], nombre(p)) && searchQueue(listas[0], nombre(p)) && !firstCase(p)){
        listas[3] = snoc((void*) nombre(p), listas[3]);
        eventos = snoc((void*) writeME(nombre(p), 2, date), eventos);
    }else if(searchQueue(listas[3], nombre(p)) && firstCase(p)){
        listas[3] = withoutNode(listas[3], nombre(p));
    }
    //0. Pais recibe paciente cero
    if(!searchQueue(listas[0], nombre(p)) && firstCase(p)){
        listas[0] = snoc((void*) nombre(p), listas[0]);
        eventos = snoc((void*) writeME(nombre(p), 0, date), eventos);
    }
    //1. Un país tiene su primer muerto 
    if(searchQueue(listas[1], nombre(p)) && !searchQueue(listas[2], nombre(p))){
        listas[2] = snoc((void*) nombre(p), listas[2]);
        eventos = snoc((void*) writeME(nombre(p), 1, date), eventos);
    }
    //3. Un país entra en cuarentena
    aux = altaInfectados(p) + mediaInfectados(p) + bajaInfectados(p);
    if(!searchQueue(listas[4], nombre(p)) && (aux >= contagiadosCuarentena(p))){
        listas[4] = snoc((void*) nombre(p), listas[4]); 
        eventos = snoc((void*) writeME(nombre(p), 3, date), eventos);
    }
    //4. Un país sale de cuarentena
    if(searchQueue(listas[4], nombre(p)) && (aux < contagiadosCuarentena(p))){
        listas[4] = withoutNode(listas[4], nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 4, date), eventos);
    }
    //5. Un país cierra sus aeropuertos por 1ra vez
    if(!searchQueue(listas[5], nombre(p)) && (aux >= contagiadosCierreAeropuertos(p))){
        listas[5] = snoc((void*) nombre(p), listas[5]);
        eventos = snoc((void*) writeME(nombre(p), 5, date), eventos);
    }
    //6. Un país reabre sus aeropuertos
    if(searchQueue(listas[6], nombre(p)) && (aux < contagiadosCierreAeropuertos(p))){
        listas[6] = withoutNode(listas[6], nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 6, date), eventos);
    }else if(!searchQueue(listas[6], nombre(p)) && (aux >= contagiadosCierreAeropuertos(p))){
        listas[6] = snoc((void*) nombre(p), listas[6]);
    }
    // 7. Un país cierra sus negocios por 1ra vez 
    if(!searchQueue(listas[7], nombre(p)) && (aux >= contagiadosCierreNegocios(p))){
        listas[7] = snoc((void*) nombre(p), listas[7]);
        eventos = snoc((void*) writeME(nombre(p), 7, date), eventos);
    }
    //8. Un país reabre sus negocios
    if(searchQueue(listas[8], nombre(p)) && (aux < contagiadosCierreNegocios(p))){
        listas[8] = withoutNode(listas[8], nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 8, date), eventos);
    }else if(!searchQueue(listas[8], nombre(p)) && (aux >= contagiadosCierreNegocios(p))){
        listas[8] = snoc((void*) nombre(p), listas[8]);
    }
    //9. Un país clausura sus mercados por 1ra vez
    if(!searchQueue(listas[9], nombre(p)) && (aux >= contagiadosClausuraMercados(p))){
        listas[9] = snoc((void*) nombre(p), listas[9]);
        eventos = snoc((void*) writeME(nombre(p), 9, date), eventos);
    }
    //10. Un país reabre sus mercados
    if(searchQueue(listas[10], nombre(p)) && (aux < contagiadosClausuraMercados(p))){
        listas[10] = withoutNode(listas[10], nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 10, date), eventos);
    }else if(!searchQueue(listas[10], nombre(p)) && (aux >= contagiadosClausuraMercados(p))){
        listas[10] = snoc((void*) nombre(p), listas[10]);
    }
    //11. Un  país  detiene  su  transporte  publico  por 1ra vez
    if(!searchQueue(listas[11], nombre(p)) && (aux >= contagiadosDetenerTransporte(p))){
        listas[11] = snoc((void*) nombre(p), listas[11]);
        eventos = snoc((void*) writeME(nombre(p), 11, date), eventos);
    }
    //12. Un país reactiva su transporte publico
    if(searchQueue(listas[12], nombre(p)) && (aux < contagiadosDetenerTransporte(p))){
        listas[12] = withoutNode(listas[12], nombre(p));
        eventos = snoc((void*) writeME(nombre(p), 12, date), eventos);
    }else if(!searchQueue(listas[12], nombre(p)) && (aux >= contagiadosDetenerTransporte(p))){
        listas[12] = snoc((void*) nombre(p), listas[12]);
    }

    return eventos;
}

/**
 * @brief Calcula el contagio por region de todos los paises, y emite mensajes de eventualidad si se presenta un hito.
 * @param mundo
 * @param listas
 * @param tasaContagio
 * @param mortalidadNoTratarla
 * @param date
 */
void calculoContagio(Mundo *mundo, Queue *listas[], double tasaContagio, double mortalidadNoTratarla, struct tm *date){
    Queue *eventos = emptyQ();    
    Queue *r = mundo->regiones;
    Pais *pais;
    Queue *pp;

    while(r){
        
        pp = ((Region*)head(r))->paises;
        while(pp){
            
            pais = (Pais*) head(pp);
            pp   = tail(pp);
            
            //Si el pais no ha recibido su primer contagiado, continue
            if(!firstCase(pais)){
                
                continue;
            } 
            
            //Caso contrario, calcular el contagio
            mundo->eventos = snoc((void*) contagioPais(mundo, pais, listas, tasaContagio, mortalidadNoTratarla, date), mundo->eventos);
            //Generar mensajes de eventualidad
            
            mundo->eventos = hitoPais(mundo->eventos, pais, listas, date);

            
        }
        r = tail(r);
    }
}

/**
 * @brief Calcula el contagio por region de todos los paises.
 * @param mundo
 * @param listas
 * @param tasaContagio
 * @param mortalidadNoTratarlar
 * @param date
 */
void etapa3(Mundo *mundo, Queue *listas[], double tasaContagio, double mortalidadNoTratarla, struct tm *date){
    calculoContagio(mundo, listas, tasaContagio, mortalidadNoTratarla, date);
}

/**
 * @brief Imprime todos los mensajes informacionales.
 * @param mundo
 * @param filename
 * @param days
 */
pid_t etapa5(Mundo *mundo, char *fileName, int days){
    pid_t child_pid = fork(); 

    if (child_pid < 0){
        fprintf(stderr, "Etapa5: No se pudo crear nuevo proceso.");
        return EXIT_FAILURE;
    }else if (child_pid == 0){
        print(mundo, fileName, days);
    }else{
        return child_pid;
    }
    
    return 0;    
}
