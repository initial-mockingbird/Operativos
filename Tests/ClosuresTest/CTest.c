#include "../../Utils/UsefulMacros/macros.h"
#include "../../Utils/Closures/Closure.h"
#include "../../Utils/LinkedList/LinkedList.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// ---------------------- 
// |      MACROS        |
// ---------------------- 

#define COMPSTR(fst, snd) strcmp(fst,snd) == 0

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

void* whatev(void* i, void* j, void* k){
    int ii = (int) i;
    int jj = (int) j;
    int kk = (int) k;
    return (void*) (ii + jj * kk);
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


int main(int argc, char const *argv[])
{
    // ---------------------- 
    // |      TEST 1        |
    // ---------------------- 

    printf("\n");
    printf("----------------------\n|      TEST 1        |\n----------------------\n\n");
    printf("Creando la clausura (1+) y aplicandosela al valor 5:\n\n");
    Closure* plusOne = dyadicToClosure(add);
    plusOne          = apply(plusOne, (void*) 1);
    printf("(+1) 5 = %d\n", getVal(apply(plusOne, (void*) 5)));

    // ---------------------- 
    // |      TEST 2        |
    // ---------------------- 

    printf("\n");
    printf("----------------------\n|      TEST 2        |\n----------------------\n\n");
    printf("Componiendo la clausura (+3) con (+7) deberia dar lo mismo que (+10):\n\n");
    Closure* plusThree = dyadicToClosure(add);
    plusThree          = apply(plusThree, (void*) 3);
    Closure* plusSeven = dyadicToClosure(add);
    plusSeven          = apply(plusSeven, (void*) 7);
    Closure* plusThreeAfterSeven   = compose(plusThree,plusSeven); 
    Closure* plusTen   = dyadicToClosure(add);
    plusTen            = apply(plusTen, (void*) 10);

    printf("(+3) ( (+7)  5) = %d\n", getVal(apply(plusThreeAfterSeven, (void*) 5)));
    printf("(+10) 5         = %d\n",getVal(apply(plusTen, (void*) 5)) );

    // ---------------------- 
    // |      TEST 3        |
    // ---------------------- 

    printf("\n");
    printf("----------------------\n|      TEST 3        |\n----------------------\n\n");
    printf("Realizando 1 + 2 * 3:\n\n");

    Closure* whatevC = tryadicToClosure(whatev);
    Closure* addCC   = dyadicToClosure(add);
    printf("1 + 2 * 3 = %d\n\n",(int) applyAndGelValN(whatevC, 3,(void*) 1,(void*) 2,(void*) 3));

    return EXIT_SUCCESS;
}


