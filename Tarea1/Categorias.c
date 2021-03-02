#include "../LinkedList/LinkedList.h"
#include "Categorias.h"
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
#define FILE_NAME "PC.txt"

// ---------------------- 
// |    AUX FUNCTIONS   |
// ---------------------- 

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
 * @brief Cuenta cuantas repeteiciones de la frase `ph` se presentan en el string `s`
 * 
 * @param s String representado como linked list de palabras
 * @param ph Frase representada como linked list de palabras
 * @return int Numero de repeticiones de la frase en el string.
 */
int countPhraseBelongsToString(Queue*s, Queue* ph){
    int l = length(ph);
    int i = length(s);
    int count = 0;
    while(i >= l){
        if (and(zipWith(FROM_DYADIC(compareStr),take(l,s),ph))){
            count++;
        }
        i--;
        s = tail(s);
    }

    return count;
}

// ---------------------- 
// |      FUNCTIONS     |
// ---------------------- 
/**
 * @brief Cuenta la frecuencia de una categoria en un string.
 * 
 * @param s 
 * @param cat 
 * @return int 
 */
int countsCategory(char* s, Queue* cat){
    char *token;
    int count = 0;
    // TODO: reemplazar con la longitud del string en vez de 250
    char* ss = malloc(250*sizeof(s));
    strcpy(ss,s);
    token = strtok(ss," ");
    Queue* q = emptyQ();
    while( token != NULL ) {
      q = snoc((void*) token,q);
      token = strtok(NULL, " ");
   }

    
   cat = tail(cat);
   while(cat){
       count += countPhraseBelongsToString(q,head(cat));
       cat = tail(cat);
   }

   return count;
}

/**
 * @brief Dado un string, y una lista con las categorias.
 * 
 * @param s 
 * @param categories 
 * @return int 
 */
int yieldCategory(char*s, Queue* categories){
    int pos = 0;
    int max = 0;
    int current = 0;
    int i = 0;
    // TODO: copiar el mensaje a una variable ss, para evitar que el side effect salga fuera de la funcion.
    str_tolower(s);
    strip_punctuation(s);
    while(categories){
        current = countsCategory(s,head(categories));
        if (current > max){
            max = current;
            pos = i;
        }
        i++;
        categories = tail(categories);
    }

    if (max = 0) {return -1;}

    return pos;
}


/**
 * @brief Inicializa una lista enlazada sin header asignándole el valor pasado como parámetro al primer nodo. Si el malloc falla, no hace nada. 
 * 
 * @param first 
 * @return struct words_list* 
 */
struct words_list * generateWordList(ELEMENT first){
    struct words_list *list = malloc(sizeof(struct words_list));

    if (list != NULL) {
        list->value = strdup(first);
        list->next = NULL;
    }

    return list;
}

/**
 * @brief Inicializa una lista enlazada sin header asignándole el nombre y una lista enlazada pasados como parametros. Si el malloc falla, no hace nada. 
 * 
 * @param category_name 
 * @param element 
 * @return struct category* 
 */
struct category *generateCategoriesList(char* category_name, struct words_list *element){
    struct category *list = malloc(sizeof(struct category));

    if (list != NULL) {
        list->name = strdup(category_name);
        list->words = element;
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
int appendWord(ELEMENT newElement, struct words_list *start){
    
	struct words_list *word = generateWordList(newElement);
    if (word == NULL)
        return EXIT_FAILURE;
    while (start->next != NULL)
        start = start->next;
    start->next = word;
    return EXIT_SUCCESS; 
}

/**
 * @brief Añade una nueva categoría a una lista enlazada. Retorna EXIT_FAILURE si no se pudo agregar, EXIT_SUCCESS si no hubo inconveniente.
 * 
 * @param name 
 * @param newElement 
 * @param category_list 
 * @return int 
 */
int appendCategory(char* name, struct words_list *newElement, struct category *category_list){
    
	struct category *categ = generateCategoriesList(name, newElement);
    if (categ == NULL)
        return 0;
    while (category_list->next != NULL)
        category_list = category_list->next;
    category_list->next = categ;
    return 1; 
}

/**
 * @brief Lee el archivo con categorías y palabras, las parsea en un diccionario que tiene la categoría con un apuntador a su lista de palabras correspondientes
 * 
 * @param categories 
 */
void readData(struct category **categories){
    char file_name[25], linebuf[MAX_LEN], cat_name[35];
    struct words_list* words;
    int i = 0; 
    
    FILE *fp = fopen(FILE_NAME, "r"); // modo lectura

    if (fp == NULL) // Se falla leyendo el archivo
    {
        perror("Ocurrió un error abriendo el archivo.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(linebuf, sizeof linebuf, fp) != NULL) {
        if(linebuf[0] == '\r' || linebuf[0] == '\n'){
            if (i > 1) //Es decir, la lista de cateogrías ya se creó
                i = 1;     
            continue; //Se salta el resto del while
        } else if (strstr(linebuf, "PRIORIDAD") != NULL || strstr(linebuf, "IDENTIFICAR") != NULL) {
            strcpy(cat_name, linebuf);
            continue; //Se salta el resto del while
        }
        if (i == 0){ //Si no se ha creado la lista de categorías
            words = generateWordList(linebuf);
            (*categories) = generateCategoriesList(cat_name, words);
            i += 2;
        }else if (i == 1){ //La lista de categorías está creada, pero se necesita insertar una categoría nueva.
            words = generateWordList(linebuf);
            appendCategory(cat_name, words, *categories);
            i++;
        }else{ //Añadimos palabras a la categoría en la que nos encontramos.
            appendWord(linebuf, words);
        }
    }

    fclose(fp); //Cerramos el archivo que está siento leido

    //PARA HACER PRUEBAS: printf("Primera Categoria: %s y Novena Palabra: %s\n", categories->name, categories->words->next->next->next->next->next->next->next->next->value);
    //printf("Primera Categoria: %s y Novena Palabra: %s\n", categories->name, categories->words->next->next->next->next->next->next->next->next->value);
}

/**
 * @brief Convierte una lista de palabras en una lista generica.
 * 
 * @param words 
 * @return Queue* 
 */
Queue* wordsToList(struct words_list* words){
    Queue* wordsL = emptyQ();
    char *token;
    Queue* q;
    char* word;
    while(words){
        q = emptyQ();
        word = words->value;
        if (word[strlen(word)-1] == '\n'){
            word[strlen(word)-1] = ' ';
            if (word[strlen(word)-2] == '\r'){
                word[strlen(word)-2] = ' ';
            }
        }

        str_tolower(word);
        token = strtok(word, " ");
        while(token != NULL){
            q = snoc((void*) token,q);
            token = strtok(NULL, " ");
        }
        wordsL = snoc((void*) q, wordsL);
        words  = words->next; 
    }
    return wordsL;
}
/**
 * @brief Convierte una lista de categorias en una lista de listas, en donde el primer elemento es el nombre de la categoria
 * y el resto son listas de palabras.
 * 
 * @param categories 
 * @return Queue* 
 */
Queue* categoriesToList(struct category *categories){
    Queue* wordsL = emptyQ();
    Queue* categoriesL = emptyQ();
    while(categories){
        wordsL = wordsToList(categories->words);
        wordsL = cons((void*) categories->name, wordsL);
        categoriesL = snoc((void*)  wordsL, categoriesL);
        categories = categories->next;
    }

    return categoriesL;
}