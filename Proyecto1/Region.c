#include "../LinkedList/LinkedList.h"
#include "./Region.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---------------------- 
// |      MACROS        |
// ----------------------

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


/** @def MAX_LEN
 * @brief Maxima longitud de una cadena de texto.
 * 
 */
#define MAX_LEN 100
/** @def ELEMENT
 * @brief Tipo de elemento que almacena la lista (en este caso, un char)
 * 
 */
#define ELEMENT char*
/** @def FILE_NAME
 * @brief Path del archivo a leer.
 * 
 */

// ---------------------- 
// |      ESTADO        |
// ----------------------

extern Queue* regiones = NULL;

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Inicializa una lista enlazada sin header asignándole el valor pasado como parámetro al primer nodo. Si el malloc falla, no hace nada. 
 * 
 * @param first 
 * @return struct regiones_aliadas* 
 */
struct regiones_aliadas * generarRegionesAliadas(ELEMENT first){
    struct regiones_aliadas *list = malloc(sizeof(struct regiones_aliadas));

    if (list != NULL) {
        list->siglas = strdup(first);
        list->next = NULL;
    }

    return list;
}

/**
 * @brief Inicializa una lista enlazada sin header asignándole el nombre y una lista enlazada pasados como parametros. Si el malloc falla, no hace nada. 
 * 
 * @param region_siglas 
 * @param element 
 * @return struct region* 
 */
struct region *generarListaRegiones(char* region_siglas, struct regiones_aliadas *element){
    struct region *list = malloc(sizeof(struct region));

    if (list != NULL) {
        list->siglas = strdup(region_siglas);
        list->aliadas = element;
        list->next = NULL;
    }

    return list;
}

/**
 * @brief Añade un nueva palabra a una lista enlazada. Retorna EXIT_FAILURE si no se pudo agregar, EXIT_SUCCESS si no hubo inconveniente.
 * 
 * @param newElement 
 * @param start 
 * @return int 
 */
int appendAliada(ELEMENT newElement, struct regiones_aliadas *start){
    
	struct regiones_aliadas *aliada = generarRegionesAliadas(newElement);
    if (aliada == NULL)
        return EXIT_FAILURE;
    while (start->next != NULL)
        start = start->next;
    start->next = aliada;
    return EXIT_SUCCESS; 
}

/**
 * @brief Añade una nueva region a una lista enlazada. Retorna EXIT_FAILURE si no se pudo agregar, EXIT_SUCCESS si no hubo inconveniente.
 * 
 * @param siglas 
 * @param newElement 
 * @param region_list 
 * @return int 
 */
int appendRegion(char* siglas, struct regiones_aliadas *newElement, struct region *region_list){
    
	struct region *reg = generarListaRegiones(siglas, newElement);
    if (reg == NULL)
        return EXIT_FAILURE;
    while (region_list->next != NULL)
        region_list = region_list->next;
    region_list->next = reg;
    return EXIT_SUCCESS; 
}

/**
 * @brief Convierte una lista de palabras en una lista generica.
 * 
 * @param aliadas 
 * @return Queue* 
 */
Queue* aliadasToList(struct regiones_aliadas* aliadas){
    Queue* aliadasL = emptyQ();
    char *token;
    Queue* q;
    Queue* aliada;
    while(aliadas){
        aliada = aliadas->siglas;
        aliadasL = snoc((void*) aliada, aliadasL);
        aliadas  = aliadas->next; 
    }
    return aliadasL;
}

/**
 * @brief Convierte una lista de regiones en una lista de listas, en donde el primer elemento es el nombre de la region
 * y el resto son listas de palabras.
 * 
 * @param regions 
 * @return Queue* 
 */
Queue* regionesToList(struct region *regions){
    Queue* aliadasL = emptyQ();
    Queue* regionsL = emptyQ();
    while(regions){
        aliadasL = aliadasToList(regions->aliadas);
        aliadasL = cons((void*) regions->siglas, aliadasL);
        regionsL = snoc((void*)  aliadasL, regionsL);
        regions = regions->next;
    }

    regiones = regionsL;
    return regionsL;
}
