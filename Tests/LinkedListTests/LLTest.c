#include "../../LinkedList/LinkedList.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// ---------------------- 
// |      MACROS        |
// ---------------------- 
#define FROM_DYADIC(f) (void* (*)(void *,void*)) f
#define FROM_MONADIC(f) (void* (*)(void *)) f
#define FROM_MONADIC_VOID(f) (void (*)(void *)) f

// ---------------------- 
// |    AUX FUNCTIONS   |
// ---------------------- 

/**
 * @brief Adds one to an integer.
 * 
 * @param i 
 * @return void* 
 */
void* addOne(void* i){
    int ii = (int) i;
    return (void*) (i+1);
}

/**
 * @brief Adds to integers together
 * 
 * @param i 
 * @param j 
 * @return void* 
 */
void* add(void* i, void* j){
    int ii = (int) i;
    int jj = (int) j;
    return (void*) (ii + jj);
}


/**
 * @brief Prints a single integer
 * 
 * @param i 
 */
void printInt(int i){
    printf("%d ", i);
}

void printStrLn(char* s){
    printf("%s\n",s);
}

void printStr(char* s){
    printf("%s ",s);
}

/**
 * @brief Prints a List of integers
 * 
 * @param q 
 */
void printQInt(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printInt;
    sequenceIO(f, q);
}

void printQStr(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printStr;
    sequenceIO(f,q);
}

void printQStrPrime(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printStr;
    printf("{ ");
    sequenceIO(f,q);
    printf(" } ");
}

/**
 * @brief The constant function: \f$f(a,b) = a\f$
 * 
 * @param val 
 * @param valP 
 * @return void* 
 */
void* constF(void* val, void* valP){
         return val; 
}


// Testing every function, might be useful to see how functions behave and how input is created and passed.
int main(int argc, char const *argv[])
{   

    // ---------------------- 
    // |      TEST 1        |
    // ---------------------- 

    printf("\n");
    printf("----------------------\n|      TEST 1        |\n----------------------\n\n");
    printf("Primero crearemos una lista con los numeros del 1 al 5 usando la consP el cual modifica su argumento:\n\n");

    // --------------------------------------------
    Queue* q = emptyQ();
    for(int i=5; i>0; i--){
        consP((void *) i ,&q);
    }
    // ---------------------------------------------

    // Deberia ser: { 1 2 3 4 5}
    printf("q: { ");
    printQInt(q);
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 2        |
    // ---------------------- 

    printf("----------------------\n|      TEST 2        |\n----------------------\n\n");
    printf("La suma de los elementos de q es (usando foldl):\n\n");
    printf("%d\n\n", foldl(FROM_DYADIC(add), (void *) 0, q));

    // ---------------------- 
    // |      TEST 3        |
    // ---------------------- 

    printf("----------------------\n|      TEST 3        |\n----------------------\n\n");
    printf("La suma de los elementos de q es (usando foldr):\n\n");
    printf("%d\n\n", foldr(FROM_DYADIC(add), (void *) 0, q));

    // ---------------------- 
    // |      TEST 4        |
    // ---------------------- 

    printf("----------------------\n|      TEST 4        |\n----------------------\n\n");
    printf("Al revertir la lista obtenemos:\n\n");
    printf("{ ");
    printQInt(reverse(q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 5        |
    // ---------------------- 

    printf("----------------------\n|      TEST 5        |\n----------------------\n\n");
    printf("Insertando un elemento en la ultima posicion (sin modificar la lista original):\n\n");
    printf("{ ");
    printQInt(snoc((void*) 6, q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 6        |
    // ---------------------- 

    printf("----------------------\n|      TEST 6        |\n----------------------\n\n");
    printf("Utilizando la version de map que modifica el argumento, anadimos 1 a cada elemento de la lista original.\n\n");
    mapP(FROM_MONADIC( addOne),q);
    printf("{ ");
    printQInt(q);
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 7        |
    // ---------------------- 

    printf("----------------------\n|      TEST 7        |\n----------------------\n\n");
    printf("Creando dos listas: {1 2 3 4 5} y {10 11 12 13 14 15} para concatenarlas:\n\n");

    q = emptyQ();
    for(int i=1; i<=5; i++){
        q = snoc((void*) i, q);
    }
    Queue* qq = emptyQ();
    for(int i=10; i<=15; i++){
        qq = snoc((void *) i ,qq);
    }

    printf("{ ");
    printQInt(append(q,qq));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 8        |
    // ---------------------- 

    printf("----------------------\n|      TEST 8        |\n----------------------\n\n");

    printf("Creando la lista: {{ 1 2 3 4 5 } {1 2 3 4 5}} y concatenando los resultados:\n\n");
    
    Queue* qqq = emptyQ();
    for(int i=0; i<2; i++){
        consP((void *) q ,&qqq);
    }
    printf("{ ");
    printQInt(concat(qqq));
    printf("}\n\n");
    
    // ---------------------- 
    // |      TEST 9        |
    // ---------------------- 

    printf("----------------------\n|      TEST 9        |\n----------------------\n\n");

    printf("Creando un **arreglo** de **arreglos** de strings, los cuales se parsean a lista:\n\n");

    char wss[3][4][15] = {{"lorem", " ipsum"}, {"papa", "mamma", "boi",">:V"}, {"colita", "sana"} };
    Queue* ll = emptyQ();
    for (int i = 0; i<3; i++){
        Queue* l = emptyQ();
        for(int j = 0; j<4; j++){
             l = snoc((void*) wss[i][j],l);
        }
        ll = snoc((void*) l, ll);
    }
    printf("{ ");
    sequenceIO(FROM_MONADIC_VOID(printQStrPrime),ll);
    printf(" }\n\n");


    return EXIT_SUCCESS;
}
