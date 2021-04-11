#include "../Utils/UsefulMacros/macros.h"
#include "Mensaje.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "Lectura.h"
#include <string.h>
#include <math.h>
/** @def MAX_LEN
 * @brief Maxima longitud de una cadena de texto.
 * 
 */
#define MAX_LEN 100

/** @def FILE_NAME
 * @brief Path del archivo a leer.
 * 
 */
#define FILE_NAME "Ebola.txt"

/**
 * @brief Dado un apuntador a string, modifica el string convirtiendo cada letra a minuscula.
 * 
 * @param s 
 * @return EXIT_SUCCESS si no hubo ningun problema. 
 */
int str_tolower(char* s){
    for ( ; *s; ++s) *s = tolower(*s);

    return EXIT_SUCCESS;
}

/**
 * @brief Dado un apuntador a string, reemplaza cada no letra minuscula por un espacio en blanco.
 * 
 * @param s 
 * @return EXIT_SUCCESS si no hubo ningun problema. 
 */
int strip_punctuation(char* s){
    for ( ; *s; ++s) {
        if (*s<'a' || *s > 'z'){
            *s = ' ';
        }
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Un wrapper sobre el macro COMPSTR que castea sus argumentos.
 * 
 * @param s 
 * @param t 
 * @return EXIT_SUCCESS si no hubo ningun problema. 
 */
static int compareStr(void* s, void*t){
    return COMPSTR((char*) s, (char*)t);
}

/**
 * @brief Dada una linea, retorna un char con las palabras que contiene la linea, todas en minuscula y sin simbolos.
 * 
 * @param s 
 * @return char* 
 */
char* tokenizeInput(char* s){
    char *token;
    char *concatenate = (char *) malloc(MAX_LEN * sizeof(char));
    char* ss = malloc(strlen(s) * sizeof(s));
    ss = s;
    str_tolower(ss);
    strip_punctuation(ss);
    if (ss[strlen(ss)-1] == '\n'){
            ss[strlen(ss)-1] = ' ';
            if (ss[strlen(ss)-2] == '\r'){
                ss[strlen(ss)-2] = ' ';
            }
        }
    token = strtok(ss," ");
    while(token != NULL) {
        strcat(concatenate, token);
        token = strtok(NULL, " ");

        if(token != NULL)
            strcat(concatenate, " ");   
    }

    return concatenate;
}

double roundTo(double number, double accuracy){
    return (round(number * pow(10.0, accuracy)) / pow(10.0, accuracy));
}

/**
 * @brief Retorna la region (si existe en el Mundo 'm') que posea la siglas 'siglas'.
 * 
 * @param siglas
 * @param m 
 * @return Region* 
 */
Region* lookupByNameRegion(char* siglas, Mundo* m){
    Queue* r = m->regiones;
    char* name;
    while(r){
        name = ((Region*) head(r))->siglas;
        if(name && COMPSTR(siglas, ((Region*) head(r))->siglas)){
            return (Region*) head(r);
        }
        r = tail(r);
    }

    return NULL;
}

/**
 * @brief Retorna el pais (si existe en la Region 'r') que posea el nombre 'nombre'.
 * 
 * @param nomre 
 * @param r 
 * @return Pais* 
 */
Pais* lookupByNamePais(char* nombre, Region* r){
    Queue* pp = r->paises;
    char* name;
    while(pp){
        name = ((Pais*) head(pp))->nombre;
        if(name && COMPSTR(nombre, ((Pais*) head(pp))->nombre)){
            return (Pais*) head(pp);
        }
        pp = tail(pp);
    }

    return NULL;
}


/**
 * @brief Crea una nueva region en el mundo si no existe una con sus mismas siglas.
 * 
 * @param siglas
 * @param m 
 * 
*/ 
void crearRegion(char* siglas, Mundo* m){
    
    Region* r = NULL;

    if(siglas){
        r = lookupByNameRegion(siglas, m);
    }
    if (!r){   
        r = (Region*) malloc(sizeof(Region));
        
        strcpy(r->siglas, siglas);
        r->aliadas = emptyQ();
        m->regiones = snoc((void*) (r), m->regiones);
        r->buzonSalida = emptyQ();
        r->buzonEntrada = emptyQ();
        r->buzonPendientes = emptyQ();
        r->paisesListos = 0;
        r->tareasPendientes = 0;
        return ;
    }
    return ;
    
}

/**
 * @brief Crea un pais con el nombre 'nombre' y lo retorna.
 * 
 * @param nombre
 * @return Pais
 *  
*/ 
Pais* crearPais(char* nombre){
    
    Pais* p = NULL;

    if (!p){   
        p = (Pais*) malloc(sizeof(Pais));       
        strcpy(p->nombre, nombre);
        for(int i = 0; i < 3; i++){
            p->sucesos[i] = 0;
            p->infectadosClase[i] = 0;
            p->infectadosSalen[i] = 0;
        }
        return p;
    }
    return NULL;
    
}


/**
 * @brief Agrega un pais 'p' a una region 'r'
 * 
 * @param p
 * @param r
 *  
*/ 
void appendPais(Pais* p, Region* r){
    if(!lookupByNamePais(p->nombre, r)){
        r->paises = snoc((void*) p, r->paises);
    }
}

/**
 * @brief lee los datos de un text y crea el mundo en torno a ello.
 * @param m
 * @param file_name 
 * @param fechaContagio
 * @param infoE
*/ 
void readData(Mundo* m, char* file_name, struct tm* fechaContagio, infoEnfermedad* infoE){
    char linebuf[MAX_LEN];

    char enfermedad[MAX_LEN]; 
    char paisOrigen[MAX_LEN];
    char clase[MAX_LEN];
    
    int n_regiones;

    Region* region;
    Pais* p;

    FILE *fp = fopen(file_name, "r"); // modo lectura

    if (fp == NULL) // Se falla leyendo el archivo
    {  
        perror("Ocurrió un error abriendo el archivo.\n");
        exit(EXIT_FAILURE);
    }

    if (linebuf[0] != '\n' || linebuf[0] != '\r'){
        //datos iniciales
        fscanf(fp, "%s", enfermedad);
        fscanf(fp, "%d",  &(infoE->duracionDias));
        fscanf(fp, "%lf", &(infoE->tasaContagio));
        fscanf(fp, "%lf", &(infoE->mortalidad));
        fscanf(fp, "%s %s %d-%d-%d", paisOrigen, clase, &(fechaContagio->tm_year), &(fechaContagio->tm_mon), &(fechaContagio->tm_mday));
        fscanf(fp, "%d", &n_regiones);
    }

    for(int i = 0; paisOrigen[i]; i++){
        paisOrigen[i] = tolower(paisOrigen[i]);
    }

    fgets(linebuf, sizeof linebuf, fp);

    //regiones
    while(fgets(linebuf, sizeof linebuf, fp) != NULL && linebuf[0] != '\r' ){
        if (linebuf[0] != '\t' && linebuf[0] != '\r'){
            crearRegion(tokenizeInput(linebuf), m);
            region = lookupByNameRegion(linebuf, m);
        }else if (linebuf[0] == '\t'){
            region->aliadas = snoc((void*)(tokenizeInput(linebuf)), region->aliadas); //regiones aliadas
        }
    }

    

    //paises
    while(fgets(linebuf, sizeof linebuf, fp) != NULL){
        if (linebuf[0] == '\n' || linebuf[0] == '\r'){
            continue;
        }
        if (linebuf[0] != '\t'){
            p = crearPais(tokenizeInput(linebuf));
            fscanf(fp, "%s %d %d", linebuf, &(p->viajerosDiarios), &(p->contagiadosCierreAeropuertos));
            p->region = (char*) malloc(MAX_LEN * sizeof(char));
            strcpy(p->region,linebuf);
            fscanf(fp, "%lld %d", &(p->poblacionTotal), &(p->contagiadosCuarentena));
            fscanf(fp, "%lf %lf %d", &(p->claseAlta), &(p->compradores),  &(p->contagiadosCierreNegocios));
            fscanf(fp, "%lf %lf %lf %d", &(p->claseMedia),&(p->limitacionesMedia), &(p->mercado), &(p->contagiadosClausuraMercados));
            fscanf(fp, "%lf %lf %d %d", &(p->claseBaja), &(p->limitacionesBaja), &(p->domesticos), &(p->contagiadosDetenerTransporte));

            if (compareStr(paisOrigen, p->nombre) == 1){
                if(compareStr(clase, "Alta") == 1)
                    p->infectadosClase[0] = 1;
                else if (compareStr(clase, "Media") == 1)
                    p->infectadosClase[1] = 1;
                else 
                    p->infectadosClase[2] = 1;
            }

            p->claseAlta = roundTo(p->claseAlta, 2);
            p->claseMedia = roundTo(p->claseMedia, 2);
            p->claseBaja = roundTo(p->claseBaja, 2);

            p->limitacionesMedia = roundTo(p->limitacionesMedia, 1);
            p->limitacionesBaja = roundTo(p->limitacionesBaja, 1);

            /*char* rp = (char*) malloc(MAX_LEN * sizeof(char));
            rp = tokenizeInput(linebuf);
            p->region = (char*) malloc(MAX_LEN * sizeof(char));
            strcpy(p->region,rp);*/
            appendPais(p, lookupByNameRegion(tokenizeInput(linebuf), m));
        }
    }
    fclose(fp); //Cerramos el archivo que esta siendo leido
}