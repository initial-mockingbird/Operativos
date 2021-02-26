#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "LinkedList.h"

// ---------------------- 
// |      STRUCTURES    |
// ---------------------- 

/** @struct Queue
 * @brief An implementation of a single linked list.
 * 
 * @var Queue::val
 *  Current node (pointer) value, NULL if it's the empty list.
 * @var Queue::sig
 *  Pointer to the next element of the single linked list.
 * 
 */
struct  Queue{
        void* val;
        struct Queue* sig;
    };

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief A version of map that modifies the structure.
 * 
 * @param f
 *  Function to apply to each element .
 * @param qq 
 *  Queue to apply the function.
 */
void mapP(void* (*f)(void*), Queue* qq){
    Queue* q = qq;
    while(q){
        q->val = (*f)(q->val);
        q      = q->sig ;
    }
}

/**
 * @brief Given a function that performs IO, sequence the series of actions for each element of the list.
 * 
 * @param f 
 *  A function that performs IO over the values of the list.
 * @param qq 
 *  Queue to apply the function.
 */
void sequenceIO(void (*f)(void*), Queue* qq){
    Queue* q = qq;
    while(q){
        (*f)(q->val);
        q      = q->sig ;
    }
}

/**
 * @brief Reduces a list from left to right (left associativity)
 * 
 * @param f 
 *  Function that combines the accumulator with the values of the list.
 *  f has type: `(acc-> a -> acc)` where `a` is the type of the list and `acc` is the type of the accumulator.
 * @param acc 
 *  Accumulator
 * @param qq 
 *  Queue to apply the function
 * @return void* 
 *  Reduced value.
 */
void* foldl(void* (*f)(void*, void*), void* acc, Queue* qq){
    
    while(qq){
        acc = f(acc,qq->val);
        qq   = qq->sig;
    }
    return acc;
}

/**
 * @brief Reduce a list from right to left (right associativity)
 * 
 * @param f 
 *  Function that combines the accumulator with the values of the list.
 *  f has type: `(a->acc->acc)` where `a` is the type of the list and `acc` is the type of the accumulator.
 * @param acc 
 *  Accumulator
 * @param qq 
 *  Queue to apply the function
 * @return void* 
 *  Reduced value
 */
void* foldr(void* (*f)(void*, void*), void* acc, Queue* qq){
    
    if (!qq){
        return acc;
    }

    return f(qq->val, foldr(f,acc,qq->sig));
}
/**
 * @brief Builds an empty list
 * 
 * @return Queue* 
 *  An empty List.
 */
Queue* emptyQ(){
    return NULL;
}
/**
 * @brief A version of cons that modifies the structure
 * 
 * @param value 
 *  Value to be inserted at the head of the list
 * @param q 
 *  List to be modified.
 */
void consP(void* value, Queue** q){
    
    Queue* node;
    node = (Queue*) malloc(sizeof(Queue)); 
    node->val = value;  
    node->sig = *q;
    
    (*q) = node;
}
/**
 * @brief Appends an element to the head of the list
 * 
 * @param value 
 *  Value to be inserted at the head of the list.
 * @param q 
 *  List where append happens
 * @return Queue* 
 *  Returns a new list with `value` as its head. 
 */
Queue* cons(void* value, Queue* q){
    
    Queue* node;
    node = (Queue*) malloc(sizeof(Queue)); 
    node->val = value;  
    node->sig = q;
    
    return node;
}

/**
 * @brief Flipped arguments version of cons
 * 
 * @param q 
 *  List where append happens
 * @param value 
 *  Value to be inserted at the head of the list.
 * @return Queue* 
 *  Returns a new list with `value` as its head.
 */
Queue* flipCons(Queue* q, void* value){
    return cons(value,q);
}

/**
 * @brief Appends an element to the tail of the list.
 * 
 * @param value 
 *  Value to be appended at the tail of the list.
 * @param q 
 *  List where the append happens
 * @return Queue* 
 *  Returns a new list with `value` as the last element of the list.
 */
Queue* snoc(void* value,Queue* q){
    void* (*f)(void*,void*) = (void* (*)(void *,void*)) cons;
    Queue* qq = (Queue*) malloc(sizeof(Queue));
    qq = emptyQ();
    consP(value,&qq);
    return foldr(f,qq,q);
}

/**
 * @brief Reverses a list.
 * 
 * @param q 
 *  List to be reversed.
 * @return Queue* 
 *  Reversed list.
 */
Queue* reverse (Queue* q){
    void* (*f)(void*,void*) = (void* (*)(void *,void*)) flipCons;
    return foldl(f,emptyQ(),q);
}

/**
 * @brief \f$ \mathcal{O}(n) \f$ filter, applied to a predicate and a list, returns the list of those elements that satisfy the predicate; i.e.,
 * 
 *      filter(odd,[1:2:3:4:5:6]) == [1,3,5]
 * 
 * @param f 
 * @param qq 
 * @return Queue* 
 */
Queue* filter(int* (*f)(void*),Queue* qq){
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q = emptyQ();
    while(qq){
        if ((*f)(qq->val)){
            q = snoc(qq->val,q);
        }
        qq = qq->sig;
    }
    return q;
}

// ---------------------- 
// |      TESTING       |
// ---------------------- 

void* addOne(void* i){
    int ii = (int) i;
    return (void*) (i+1);
}


void* add(void* i, void* j){
    int ii = (int) i;
    int jj = (int) j;
    return (void*) (ii + jj);
}



void printInt(int i){
    printf("%d\n", i);
}

void printQInt(Queue *q){
    void (*f)(void*) = (void (*)(void *)) printInt;
    sequenceIO(f, q);
}

void* constF(void* val, void* valP){
         return val; 
}

int main(int argc, char const *argv[])
{   
    void* (*h)(void*,void*) = (void* (*)(void *,void*)) flipCons;
    void* (*f)(void*,void*) = (void* (*)(void *,void*)) cons;
    void* (*fP)(void*,void*) = (void* (*)(void *,void*)) snoc;
    void* (*hP)(void*,void*) = (void* (*)(void *,void*)) add;
    Queue* q = emptyQ();
    for(int i=0; i<5; i++){
        consP((void *) i ,&q);
    }
    printf("Printing the list:\n");
    printQInt(q);
    printf("Folding left with sum should result in the sum of the elements: ");
    printf("%d\n", foldl(hP, (void *) 0, q));
    printf("Folding right with sum should result in the sum of the elements: ");
    printf("%d\n", foldr(hP, (void *) 0, q));
    printf("Reversing the list:\n");
    printQInt(reverse(q));
    printf("Printing the list again:\n");
    printQInt(q);
    printf("Inserting an element at the last position:\n");
    printQInt(snoc((void*) 5, q));
    printf("Adding +1: \n");
    mapP((void* (*)(void *)) addOne,q);
    printQInt(q);
    printf("First element: \n");
    printf("%d\n",(int) foldr((void* (*)(void *,void*)) constF, (void*) 20,q));
    return EXIT_SUCCESS;
}
