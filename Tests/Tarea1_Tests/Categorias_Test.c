#include "../../LinkedList/LinkedList.h"
#include "../../Tarea1/Try.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// ---------------------- 
// |      MACROS        |
// ---------------------- 
#define FROM_DYADIC(f) (void* (*)(void *,void*)) f
#define FROM_MONADIC(f) (void* (*)(void *)) f
#define FROM_MONADIC_VOID(f) (void (*)(void *)) f
#define COMPSTR(fst, snd) strcmp(fst,snd) == 0


// ---------------------- 
// |    AUX FUNCTIONS   |
// ---------------------- 

void printStrLn(char* s){
    printf("%s\n",s);
}

void printStr(char* s){
    printf("%s ",s);
}

void printQStr(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printStr;
    sequenceIO(f,q);
}

void printQStrLn(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printStrLn;
    sequenceIO(f,q);
}

void printQStrPrime(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printStr;
    printf("{ ");
    sequenceIO(f,q);
    printf(" } ");
}

int main(int argc, char const *argv[])
{
    /*
    struct category *categories_list;
    readData(categories_list);
    return 0;*/

    struct category *categories_list;
    readData(&categories_list);
    Queue* categories = emptyQ();
    Queue* words = emptyQ();

    // ---------------------- 
    // |      TEST 1        |
    // ---------------------- 
    
    // Recordar pedirle a mari que quite los \n. 
    printf("\n");
    printf("----------------------\n|      TEST 1        |\n----------------------\n\n");
    printf("Palabras de la primera categoria:\n\n");

    
    words = wordsToList(categories_list->words);
    sequenceIO(FROM_MONADIC_VOID(printQStr),words);
    printf("\n\n");

    
    // ---------------------- 
    // |      TEST 2        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 2        |\n----------------------\n\n");
    printf("Las categorias junto a las palabras son:\n\n");

    categories = categoriesToList(categories_list);

    while(categories){
        words = head(categories);
        printf("%s\n",(char*) head(words));
        sequenceIO(FROM_MONADIC_VOID(printQStr),tail(words));
        categories = tail(categories);
        printf("\n");
    }

    // ---------------------- 
    // |      TEST 3        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 3        |\n----------------------\n\n");
    printf("Cuantas veces se repite la frase: \"POLLITO RICO\" en la expresion: \"POLLITO RICO AAAA POLLITO POLLITO MORE RICO POLLITO RICO\"\n\n");

    Queue *pollito = snoc((void*)"RICO", cons((void*) "POLLITO", emptyQ()));
    Queue *pollito_frase = emptyQ();
    pollito_frase = snoc((void*)"POLLITO", pollito_frase);
    pollito_frase = snoc((void*)"RICO", pollito_frase);
    pollito_frase = snoc((void*)"AAAA", pollito_frase);
    pollito_frase = snoc((void*)"POLLITO", pollito_frase);
    pollito_frase = snoc((void*)"POLLITO", pollito_frase);
    pollito_frase = snoc((void*)"MORE", pollito_frase);
    pollito_frase = snoc((void*)"RICO", pollito_frase);
    pollito_frase = snoc((void*)"POLLITO", pollito_frase);
    pollito_frase = snoc((void*)"RICO", pollito_frase);

    printf("Se repite %d veces\n", countPhraseBelongsToString(pollito_frase, pollito));

    // ---------------------- 
    // |      TEST 4        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 4        |\n----------------------\n\n");
    printf("Contaremos cuantas palabras de la categoria 4: Exploracion se presentan en la siguiente franse:\n\n");
    categories = categoriesToList(categories_list);
    char frase[500] = "Me voy, cree creo dice digo";
    printf("%s\n->",frase);
    
    Queue* categoria4 = valueAt(categories,3);

    
    printf("%d\n\n", countsCategory(frase, categoria4));


    // ---------------------- 
    // |      TEST 5        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 5        |\n----------------------\n\n");
    printf("Cual es la categoria que mejor representa el siguente mensaje: :\n\n");
    categories = categoriesToList(categories_list);
    char frase2[500] = "Me voy, cree creo dice digo";
    printf("%s\n->",frase2);


    
    printf("%d\n\n", yieldCategory(frase2, categories));
    
    return EXIT_SUCCESS;
}