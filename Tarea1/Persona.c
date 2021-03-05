#include "../LinkedList/LinkedList.h"
#include "./Categorias.h"
#include "./Persona.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** @def FROM_DYADIC
 * @brief Casts a dyadic function of type: `a -> b -> c` to `void* -> void* -> void*`.
 * 
 */
#define FROM_DYADIC(f) (void* (*)(void *,void*)) f
/** @def FROM_MONADIC
 * @brief Casts a monadic function of type: `a -> b` to `void* -> void*`.
 * 
 */
#define FROM_MONADIC(f) (void* (*)(void *)) f
/** @def FROM_MONADIC_VOID
 * @brief Casts a monadic function of type: `a -> ()` to `void* -> void`.
 * 
 */
#define FROM_MONADIC_VOID(f) (void (*)(void *)) f
/** @def COMPSTR
 * @brief Compares two strings, returns 1 if they are the same, and 0 otherwise.
 * 
 */
#define COMPSTR(fst, snd) strcmp(fst,snd) == 0

typedef struct Persona {
    char* nombre;
    char* origen;
    char* relacion;
    int origenModo;
}Persona;

extern Queue* personas = NULL;

/**
 * @brief Retorna el nombre de una persona.
 * 
 * @param p 
 * @return char* 
 */
char* nombre(Persona* p){
    return p->nombre;
}

/**
 * @brief Retorna la relacion con el usuario de una persona.
 * 
 * @param p 
 * @return char* 
 */
char* relacion(Persona* p){
    return p->relacion;
}

char* origen(Persona* p){
    return p->origen;
}

int origenModo(Persona* p){
    return p->origenModo;
}

/**
 * @brief Retorna la persona (si existe) que posea el nombre `nombre`.
 * 
 * @param nombre 
 * @return Persona* 
 */
Persona* lookupByName(char* nombre){
    Queue* p = personas;
    char* name;
    while(p){
        name = ((Persona*) head(p))->nombre;
        if(name && COMPSTR(nombre, ((Persona*) head(p))->nombre )){
            return (Persona*) head(p);
        }
        p = tail(p);
    }
    
    return NULL;
}

/**
 * @brief Retorna la persona (si existe) que posea la relacion `relacion`.
 * 
 * @param relacion 
 * @return Persona* 
 */
Persona* lookupByRelacion(char* relacion){
    Queue* p = personas;
    char* rel;
    while(p){
        rel = ((Persona*) head(p))->relacion;
        if(rel && COMPSTR(relacion, ((Persona*) head(p))->relacion ) ){
            return (Persona*) head(p);
        }
        p = tail(p);
    }

    return NULL;
}

/**
 * @brief Actualiza la informacion de una persona, si la persona no existe, la crea.
 * 
 * @param name 
 * @param relacion 
 * @param origen 
 * @param origenModo 
 */
void updatePersona(char* name, char* relacion, char* origen, int origenModo){
    
    Persona* pp = NULL;
    if(name){
        pp = lookupByName(name);
        printf("pp es: %d\n",pp);

    }
    if (!pp && relacion){
        pp = lookupByRelacion(relacion);
    }
    if (!pp){
        
        pp = (Persona*) malloc(sizeof(Persona));
        pp->nombre=name;
        pp->relacion=relacion;
        pp->origen=origen;
        pp->origenModo=origenModo;
        personas = snoc((void*) (pp), personas);
        return ;
    }
    if (!(pp->nombre)){
        pp->nombre = name;
    }
    if (!(pp->relacion)){
        pp->relacion = relacion;
    }
    if (!(pp->origen)){
        pp->origen = origen;
        pp->origenModo = origenModo;
    }

    return ;
    
}

void findAfterMi(Queue* input, char** relacion){
    size_t len;
    char* rel;
    while(!COMPSTR((char*) head(input), "mi")){
        input = tail(input);
    }
    len = strlen((char*) head(tail(input))) + 1;
    rel = (char*) malloc(len*sizeof(char));
    strcpy(rel, (char*) head(tail(input)) );
    (*relacion) = rel;
    return ;
}

/**
 * @brief Dado un input y un apuntador a un string, copia la relacion actual del input al string.
 * 
 * @param input 
 * @param relacion 
 */
void parentezco(Queue* input, char** relacion){
    Queue* ws = tail((Queue*) valueAt(categorias,2));
    int i;
    char* rel;
    size_t len;
    while(ws){
        i = countPhraseBelongsToString(input,(Queue*) head(ws));
        if (i>0){
            if (length(head(ws)) == 1){
                len = strlen((char*) head((Queue*) head(ws)));
                rel = (char*) malloc(len*sizeof(char));
                strcpy(rel, (char*) head((Queue*) head(ws)) );
                (*relacion) = rel;
                return ;
            } else {
                findAfterMi(input,relacion);
                return ;
            }
        }
        ws = tail(ws);
    }

    return ;
}

/**
 * @brief Actualiza el nombre de una persona.
 * 
 * @param p 
 */
void updateNombre(Persona** p){
    char* nombre = NULL;
    char* s = NULL;
    size_t len = 0;
    Queue* q;
    Queue* qq;
    printf("ELIZA: Y cual es su nombre?\n");
    printf("Usuario: ");
    getline(&s, &len, stdin);
    qq = tokenizeInput(s);
    q = qq;
    while(q){
        if( COMPSTR((char*) head(q), "es")      || 
            COMPSTR((char*) head(q), "llama")   ||
            COMPSTR((char*) head(q), "llamo")   ||
            COMPSTR((char*) head(q), "dicen")   ||
            COMPSTR((char*) head(q), "digo")    ||
            COMPSTR((char*) head(q), "decimos")){
                nombre = (char*) head(tail(q));
                break;
            }
        q = tail(q);
    }
    if (!nombre){
        nombre = (char*) head(qq);
    }
    (*p)->nombre = nombre;
    printf("Gracias! continua\n");
    return ;
}

/**
 * @brief Actualiza el origen de una persona.
 * 
 * @param p 
 */
void updateOrigen(Persona** p){
    char* s = NULL;
    char* origen = NULL;
    size_t len = 0;
    Queue* q;
    Queue* qq;
    int origenModo;
    printf("ELIZA: tu %s %s ? y de donde es?\n", (*p)->relacion, (*p)->nombre);
    printf("Usuario: ");
    getline(&s, &len, stdin);
    qq = tokenizeInput(s);
    q = qq;
    while(q){
        if( COMPSTR((char*) head(q), "de") || 
            COMPSTR((char*) head(q), "en")){
                origen = (char*) head(tail(q));
                origenModo = 0;
                break;
            }
        if( COMPSTR((char*) head(q), "parece") || 
            COMPSTR((char*) head(q), "es")){
                origen = (char*) head(tail(q));
                origenModo = 1;
                break;
            }
        q = tail(q);
    }
    if (!origen){
        origen = (char*) head(qq);
        origenModo = 0;
    }
    (*p)->origen = origen;
    (*p)->origenModo = origenModo;
    //printf("Gracias! continua\n");
    return ;
}

/**
 * @brief Dada una persona, decide que puede actualizar.
 * 
 * @param p 
 */
void discoveryP(Persona** p){
    if (!((*p)->nombre)){
        updateNombre(p);
        return ;
    }
    if (!((*p)->origen)){
        updateOrigen(p);
        return ;
    }

    return ;
     
}

int isComplete(Persona* p){
    return p && p->nombre && p->origen && p->relacion;
}

/**
 * @brief Dado un input, verifica si se habla de alguna persona existente, si es asi, toma
 * las medidas necesarias, sino, no hace nada.
 * 
 * @param s 
 */
int discovery(char* s){
    Queue* words = tokenizeInput(s);
    Persona* p;
    int i;
    while(words){
        
        p  = lookupByName((char*) head(words));
        if (isComplete(p)){
            char* modo;
            if (p->origenModo){
                modo = "de";
            } else {
                modo = "que es";

            }
            printf("Eliza: te refieres a tu %s %s, %s %s?\n",p->relacion, p->nombre, modo, p->origen);
            return 1 ;
        }
        if (p){
            discoveryP(&p);
            return 1;
        }
        p = lookupByRelacion((char*) head(words));
        if (p){
            
            discoveryP(&p);
            return 1;
        }
        words = tail(words);
    }

    return 0;
}

/**
 * @brief Busca algun parentezco en el string, y actualiza la persona correspondiente, creando una nueva 
 * si no existe.
 * 
 * @param s 
 */
void freudiano(char* s){
    char* ss = NULL;
    Queue* input = tokenizeInput(s);
    parentezco(input,&ss);
    updatePersona(NULL, ss, NULL, 0);
    discovery(s);
}
