#include "../../Utils/LinkedList/LinkedList.h"
#include "../../Utils/Closures/Closure.h"
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

typedef void* (*one_var_func) (void*);

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

void* geq(void* i, void* j){
    return (int) j >= (int) i;  
}


void* subs(void* i, void* j){
    int ii = (int) i;
    int jj = (int) j;
    return (void*) (ii - jj);
}

int gt3(void* i){
    int ii = (int) i;
    return ii > 3;
}

int compareStr(void* s, void*t){
    return COMPSTR((char*) s, (char*)t);
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

    // ---------------------- 
    // |      TEST 10        |
    // ---------------------- 

    printf("----------------------\n|      TEST 10        |\n----------------------\n\n");
    printf("La 4ta palabra, de la 2da lista de palabras es::\n\n");
    printf("%s\n\n",(char*) valueAt(valueAt(ll,1),3));

    // ---------------------- 
    // |      TEST 11       |
    // ---------------------- 

    printf("----------------------\n|      TEST 11       |\n----------------------\n\n");
    printf("Utilizando map, anadimos 1 a cada elemento de la lista .\n\n");
    printf("{ ");
    printQInt(q);
    printf("} -> ");
    printf("{ ");
    printQInt(map(FROM_MONADIC(addOne),q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 12       |
    // ---------------------- 

    printf("----------------------\n|      TEST 12       |\n----------------------\n\n");
    printf("Todos los elementos son mayores que 3: .\n\n");
    printf("{ ");
    printQInt(q);
    printf("} -> ");
    printf("%d\n\n",all(gt3,q));

    // ---------------------- 
    // |      TEST 13       |
    // ---------------------- 

    printf("----------------------\n|      TEST 13       |\n----------------------\n\n");
    printf("Algun elemento es mayor que 3: .\n\n");
    printf("{ ");
    printQInt(q);
    printf("} -> ");
    printf("%d\n\n",any(gt3,q));
    
    // ---------------------- 
    // |      TEST 14       |
    // ---------------------- 

    printf("----------------------\n|      TEST 14       |\n----------------------\n\n");
    printf("La palabra \"colita\" pertenece a la lista de palabras\n\n");
    printf("{ ");
    printQStr(concat(ll));
    printf("} -> %d\n\n",elemBy(compareStr, (void*) "colita", concat(ll)));


    // ---------------------- 
    // |      TEST 15       |
    // ---------------------- 

    printf("----------------------\n|      TEST 15       |\n----------------------\n\n");
    printf("Calculando la longitud de la lista:\n\n");
    printf("q: { ");
    printQInt(q);
    printf("} -> %d\n\n", length(q));

    // ---------------------- 
    // |      TEST 16       |
    // ---------------------- 

    printf("----------------------\n|      TEST 16       |\n----------------------\n\n");
    printf("Taking 3 elements of the list:");
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(take(3,q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 17       |
    // ---------------------- 

    printf("----------------------\n|      TEST 17       |\n----------------------\n\n");
    printf("Taking 100 elements of the list:");
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(take(100,q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 18       |
    // ---------------------- 

    printf("----------------------\n|      TEST 18       |\n----------------------\n\n");
    printf("Dropping 3 elements of the list:");
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(drop(3,q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 19       |
    // ---------------------- 

    printf("----------------------\n|      TEST 19       |\n----------------------\n\n");
    printf("Dropping 100 elements of the list:");
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(drop(100,q));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 20       |
    // ---------------------- 

    printf("----------------------\n|      TEST 20       |\n----------------------\n\n");
    printf("Zipping the list with itself using addition\n\n");
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(zipWith(FROM_DYADIC(add),q,q));
    printf("}\n\n");

    
    // ---------------------- 
    // |      TEST 21       |
    // ---------------------- 

    printf("----------------------\n|      TEST 21       |\n----------------------\n\n");
    printf("Zipping the list with its tail using addition:\n\n");
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(zipWith(FROM_DYADIC(add),q,tail(q)));
    printf("}\n\n");

    // ---------------------- 
    // |      TEST 22       |
    // ---------------------- 

    printf("----------------------\n|      TEST 22       |\n----------------------\n\n");
    printf("Checking if all of the elements are greater than 3 using and.\n\n");
    printf("q: { ");
    printQInt(q);
    printf("} -> %d\n\n", and(map((void * (*)(void *)) gt3,q)));

    // ---------------------- 
    // |      TEST 23       |
    // ---------------------- 

    printf("----------------------\n|      TEST 23       |\n----------------------\n\n");
    printf("Checking if any of the elements are greater than 3 using or.\n\n");
    printf("q: { ");
    printQInt(q);
    printf("} -> %d\n\n", or(map((void * (*)(void *)) gt3,q)));

    // ---------------------- 
    // |      TEST 24       |
    // ---------------------- 

    printf("----------------------\n|      TEST 24       |\n----------------------\n\n");
    printf("foldr using (+):\n\n");
    Closure* addC = dyadicToClosure(add);
    printf("q: { ");
    printQInt(q);
    printf("} -> %d\n\n", foldrC(addC,(void*) 0,q) );


    // ---------------------- 
    // |      TEST 25       |
    // ---------------------- 

    printf("----------------------\n|      TEST 25       |\n----------------------\n\n");
    printf("foldl using (+):\n\n");
    addC = dyadicToClosure(add);
    printf("q: { ");
    printQInt(q);
    printf("} -> %d\n\n", foldlC(addC,(void*) 0,q) );

    // ---------------------- 
    // |      TEST 26       |
    // ---------------------- 

    printf("----------------------\n|      TEST 26       |\n----------------------\n\n");
    printf("filter by >= 3:\n\n");
    Closure* geq3C = dyadicToClosure(geq);
    geq3C = apply(geq3C,(void*) 3);
    printf("q: { ");
    printQInt(q);
    printf("} -> { ");
    printQInt(filterC(geq3C,q) );
    printf("}\n\n");

    return EXIT_SUCCESS;
}
