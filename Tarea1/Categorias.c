#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
//Maxima longitud de una cadena de texto.
#define MAX_LEN 100
//Tipo de elemento que almacena la lista (en este caso, un char)
#define ELEMENT char*

//Estructura que representa un nodo de la lista enlazada que contiene palabras.
struct words_list {
    ELEMENT value;
    struct words_list * next;
};

//Estructura que representa un nodo de la lista enlazada que funciona como diccionario.
//Contiene el nombre de la categoria, un apuntador a la lista de palabras de esa categoria y un apuntador al siguiente nodo. 
struct category
{
    char* name;
    struct words_list *words;
    struct category* next;
};


/*
*Recibe: ELEMENT
*Retorna: struct word_list *
*Inicializa una lista enlazada sin header asignándole el valor pasado como parámetro al primer nodo. Si el malloc falla, no hace nada. 
*/
struct words_list * generateWordList(ELEMENT first){
    struct words_list *list = malloc(sizeof(struct words_list));

    if (list != NULL) {
        list->value = strdup(first);
        list->next = NULL;
    }

    return list;
}

/*
*Recibe: char*, struct words_list*
*Retorna: struct category*
*Inicializa una lista enlazada sin header asignándole el nombre y una lista enlazada pasados como parametros. Si el malloc falla, no hace nada. 
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

/*
*Recibe: ELEMENT, struct words_list *
*Retorna: int
*Añade un nueva palabra a una lista enlazada. Retorna EXIT_FAILURE si no se pudo agregar, EXIT_SUCCESS si no hubo inconveniente.
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

/*
*Recibe: char*, struct words_list*, struct category*
*Retorna: int
*Añade una nueva categoría a una lista enlazada. Retorna EXIT_FAILURE si no se pudo agregar, EXIT_SUCCESS si no hubo inconveniente.
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

/*
*Recibe: struct category*
*Retorna: void
*Lee el archivo con categorías y palabras, las parsea en un diccionario que tiene la categoría con un apuntador a su lista de palabras correspondientes
*/
void readData(struct category *categories){
    char file_name[25], linebuf[MAX_LEN], cat_name[35];
    struct words_list* words;
    int i = 0; 
    
    FILE *fp = fopen("Palabras Clave en Orden Alfabetico.txt", "r"); // modo lectura

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
            categories = generateCategoriesList(cat_name, words);
            i += 2;
        }else if (i == 1){ //La lista de categorías está creada, pero se necesita insertar una categoría nueva.
            words = generateWordList(linebuf);
            appendCategory(cat_name, words, categories);
            i++;
        }else{ //Añadimos palabras a la categoría en la que nos encontramos.
            appendWord(linebuf, words);
        }
    }

    fclose(fp); //Cerramos el archivo que está siento leido

    //PARA HACER PRUEBAS: printf("Primera Categoria: %s y Novena Palabra: %s\n", categories->name, categories->words->next->next->next->next->next->next->next->next->value);
}


int main()
{
    struct category *categories_list;
    readData(categories_list);
    return 0;
}