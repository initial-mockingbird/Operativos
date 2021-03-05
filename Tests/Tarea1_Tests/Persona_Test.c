#include "../../LinkedList/LinkedList.h"
#include "../../Tarea1/Categorias.h"
#include "../../Tarea1/Persona.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

void printPersona(Persona* p){
    printf("Persona: {\n");
    printf("\tnombre: %s\n",nombre(p));
    printf("\trelacion: %s\n",relacion(p));
    printf("\torigen: %s\n",origen(p));
    printf("\torigen modo: %d\n", origenModo(p));
    printf("}\n");
}

void printQPersonas(Queue* q){
    void (*f)(void*) = (void (*)(void *)) printPersona;
    sequenceIO(f,q);
}

int main(int argc, char const *argv[])
{
    struct category *categories_list;
    readData(&categories_list);
    Queue* categories = emptyQ();
    Queue* words = emptyQ();
    
    categoriesToList(categories_list);
    
    updatePersona("Usuario","Usuario",NULL,0);
    updatePersona("doniel","primo","ccs",0);
    updatePersona("wariangela","mama",NULL,0);
    updatePersona(NULL,"papa",NULL,0);

    // ---------------------- 
    // |      TEST 1        |
    // ---------------------- 
    
    printf("\n");
    printf("----------------------\n|      TEST 1        |\n----------------------\n\n");
    printf("Inicialmente, la lista de personas contiene:\n");
    printQPersonas(personas);

    // ---------------------- 
    // |      TEST 2        |
    // ---------------------- 
    
    char* rel;
    printf("----------------------\n|      TEST 2        |\n----------------------\n\n");
    printf("La relacion del siguiente input deberia ser mama:\n");
    char input[250] = "mi mama me mima";
    parentezco(tokenizeInput(input),&rel);
    printf("%s -> %s\n\n", input, rel);


    // ---------------------- 
    // |      TEST 3        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 3        |\n----------------------\n\n");
    printf("Actualizando persona por relacion:\n\n");
    char input2[250] = "mi Papa, me mima >:V";
    printf("El texto es: %s\n",input2);
    discovery(input2);
    printQPersonas(personas);


    // ---------------------- 
    // |      TEST 4        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 4        |\n----------------------\n\n");
    printf("Actualizando persona que ya posee nombre y relacion:\n\n");
    char input3[250] = "mi mama me mima >:V";
    printf("El texto es: %s\n",input3);
    discovery(input3);
    printQPersonas(personas);

    // ---------------------- 
    // |      TEST 5        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 5        |\n----------------------\n\n");
    printf("Actualizando persona que ya posee todo:\n\n");
    char input4[250] = "mi primo me mima >:V";
    printf("El texto es: %s\n",input4);
    discovery(input4);
    printQPersonas(personas);


    // ---------------------- 
    // |      TEST 6        |
    // ---------------------- 
    
    printf("----------------------\n|      TEST 6        |\n----------------------\n\n");
    printf("Anadiendo una nueva persona a la lista:\n\n");
    char input5[250] = "es que, Se trata de mi gatico";
    printf("El texto es: %s\n",input5);
    freudiano(input5);
    printQPersonas(personas);
    return EXIT_SUCCESS;
}