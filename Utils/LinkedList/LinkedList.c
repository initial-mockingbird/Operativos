#include "./LinkedList.h"
#include<stdlib.h>

// ---------------------- 
// |      MACROS        |
// ---------------------- 
#define FROM_DYADIC(f) (void* (*)(void *,void*)) f
#define FROM_MONADIC(f) (void* (*)(void *)) f
#define FROM_MONADIC_VOID(f) (void (*)(void *)) f


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
// |   AUX FUNCTIONS    |
// ----------------------

static int andL(int p, int q){
    return p && q;
}

static int orL(int p, int q){
    return p || q;
}

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------


void* head(Queue* q){
    return q->val;
}

Queue* tail(Queue* q){
    return q->sig;
}

/**
 * @brief List index (subscript) function, starting from 0.
 * 
 * @param q 
 * @param i 
 * @return void* 
 */
void* valueAt(Queue* q, int i){
    while(i){
        q = q->sig;
        i--;
    }
    return q->val;
}

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
 * @brief Given a Closure that performs IO, sequence the series of actions for each element of the list.
 * 
 * @param c 
 * @param qq 
 */
void sequenceIOC(Closure* c, Queue* qq){
    Queue* q = qq;
    while(q){
        apply(c,q->val);
        q = q->sig;
    }
}

/**
 * @brief Reduces a list from left to right (left associativity)
 * 
 * @param f 
 *  Function that combines the accumulator with the values of the list.
 *  f has type: `(acc -> a -> acc)` where `a` is the type of the list and `acc` is the type of the accumulator.
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
 * @brief Reduces a list from left to right (left associativity)
 * 
 * @param c 
 *  Closure that combines the accumulator with the values of the list.
 *  c has type: `(acc -> a -> acc)` where `a` is the type of the list and `acc` is the type of the accumulator.
 * @param acc 
 *  Accumulator
 * @param qq 
 *  Queue to apply the function
 * @return void* 
 *  Reduced value.
 */
void* foldlC(Closure* c, void* acc, Queue* qq){
    
    while(qq){
        acc = getVal(apply2(c,acc,qq->val));
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
 * @brief Reduce a list from right to left (right associativity)
 * 
 * @param c 
 *  Closure that combines the accumulator with the values of the list.
 *  f has type: `(a->acc->acc)` where `a` is the type of the list and `acc` is the type of the accumulator.
 * @param acc 
 *  Accumulator
 * @param qq 
 *  Queue to apply the function
 * @return void* 
 *  Reduced value
 */
void* foldrC(Closure* c, void* acc, Queue* qq){
    
    if (!qq){
        return acc;
    }

    return getVal(apply2(c,qq->val,foldrC(c,acc,qq->sig)));
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
 * @brief Create a Empty Q.
 * 
 * @param q 
 */
void createEmptyQ(Queue* q){
    q = (Queue*) malloc(sizeof(Queue));
    q = emptyQ();
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
 * @brief Appends an element to the tail of the list modifying it. 
 * 
 * @param value 
 *  Value to be appended at the tail of the list.
 * @param q 
 *  List where the append happens
 */
void snocP(void* value, Queue* q){

    while(q->sig){
        q = q->sig;
    }
    Queue * qq = emptyQ();
    consP(value,&qq);
    q->sig = qq;
}

/**
 * @brief Flipped arguments version of snoc
 * 
 * @param value 
 *  Value to be appended at the tail of the list.
 * @param q 
 *  List where the append happens
 * @return Queue* 
 *  Returns a new list with `value` as the last element of the list.
 */
Queue* flipSnoc(Queue* q, void* value){
    return snoc(value,q);
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


/**
 * @brief \f$ \mathcal{O}(n) \f$ filter, applied to a predicate and a list, returns the list of those elements that satisfy the predicate; i.e.,
 * 
 *      filter(odd,[1:2:3:4:5:6]) == [1,3,5]
 * 
 * @param c
 * @param qq 
 * @return Queue* 
 */
Queue* filterC(Closure* c,Queue* qq){
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q = emptyQ();
    while(qq){
        if (applyAndGetVal(c,qq->val)){
            q = snoc(qq->val,q);
        }
        qq = qq->sig;
    }
    return q;
}


/**
 * @brief Appends two lists
 * 
 * @param qq 
 * @param ll 
 * @return Queue* 
 */
Queue* append(Queue* qq, Queue* ll){
    Queue* q = (Queue*) malloc(sizeof(Queue));
    void* (*f)(void*,void*) = (void* (*)(void *,void*)) flipSnoc;
    q = qq;
    return (Queue*) foldl(f,(void*)q,ll);

}

/**
 * @brief Flattens a list of lists.
 * 
 * @param qq 
 * @return Queue* 
 */
Queue* concat(Queue*qq){
    Queue* q = emptyQ();
    while(qq){
        q = append(q,(Queue*) (qq->val));
        qq = qq->sig;
    }
    return q;
}

/**
 * @brief maps every element of the list qq using the function f.
 * 
 * @param f 
 * @param qq 
 * @return Queue* 
 */
Queue* map(void* (*f)(void*), Queue* qq){
    Queue* q = emptyQ();

    while(qq){
        q = snoc(f(qq->val),q);
        qq = qq->sig;
    }

    return q;
}

/**
 * @brief maps every element of the list qq using the closure c.
 * 
 * @param c
 * @param qq 
 * @return Queue* 
 */
Queue* mapC(Closure* c, Queue* qq){
    Queue* q = emptyQ();

    while(qq){
        q = snoc(applyAndGetVal(c,qq->val),q);
        qq = qq->sig;
    }

    return q;
}

/**
 * @brief Zips together two Lists using a function `f`. Shortcircuits on the list of the shortest lenght, thus the list can have different length.
 * 
 * @param f 
 * @param qq 
 * @param pp 
 * @return Queue* 
 */
Queue* zipWith (void* (*f)(void*,void*), Queue* qq, Queue* pp){
    Queue* q = emptyQ();
    while(qq && pp){
        q = snoc(f(qq->val,pp->val),q);
        qq = qq->sig;
        pp = pp->sig;
    }
    return q;
}


/**
 * @brief Zips together two Lists using a closure `c`. Shortcircuits on the list of the shortest lenght, thus the list can have different length.
 * 
 * @param c
 * @param qq 
 * @param pp 
 * @return Queue* 
 */
Queue* zipWithC (Closure* c, Queue* qq, Queue* pp){
    Queue* q = emptyQ();
    while(qq && pp){
        q = snoc(applyAndGelValN(c,2,qq->val,pp->val),q);
        qq = qq->sig;
        pp = pp->sig;
    }
    return q;
}

/**
 * @brief Takes `min(n,length(q))` elements out of a list.
 * 
 * @param n 
 * @param qq 
 * @return Queue* 
 */
Queue* take(int n, Queue* qq){
    Queue* q = emptyQ();
    while(n && qq){
        q = snoc(qq->val,q);
        qq = qq->sig;
        n--;
    }
    return q;
}
/**
 * @brief Drops `min(n,length(q))` elements out of a list.
 * 
 * @param n 
 * @param qq 
 * @return Queue* 
 */
Queue* drop(int n, Queue* qq){
    Queue* q = emptyQ();
    while(n && qq){
        qq = qq->sig;
        n--;
    }
    while(qq){
        q = snoc(qq->val,q);
        qq = qq->sig;
    }
    return q;

}

/**
 * @brief Given a predicate `f`, tells whether every element on the list satisfies the predicate.
 * 
 * @param f 
 * @param qq 
 * @return int 
 */
int all(int (*f)(void*),Queue* qq){
    while(qq){
        if (!f(qq->val)){
            return 0;
        }
        qq = qq->sig;
    }
    return 1;
}

/**
 * @brief Given a predicate (closure) `c`, tells whether every element on the list satisfies the predicate.
 * 
 * @param c
 * @param qq 
 * @return int 
 */
int allC(Closure* c,Queue* qq){
    while(qq){
        if (!((int) applyAndGetVal(c,qq->val))){
            return 0;
        }
        qq = qq->sig;
    }
    return 1;
}


int and(Queue* qq){
    while(qq){
        if (! ((int) (qq->val))){
            return 0;
        }
        qq = qq->sig;
    }
    return 1;
}

/**
 * @brief Given a predicate `f`, tells if any element on the list satisfies the predicate.
 * 
 * @param f 
 * @param qq 
 * @return int 
 */
int any(int (*f)(void*),Queue* qq){
    while(qq){
        if (f(qq->val)){
            return 1;
        }
        qq = qq->sig;
    }
    return 0;
}

/**
 * @brief Given a predicate (closure) `c`, tells if any element on the list satisfies the predicate.
 * 
 * @param c
 * @param qq 
 * @return int 
 */
int anyC(Closure* c,Queue* qq){
    while(qq){
        if ((int) applyAndGetVal(c,qq->val)){
            return 1;
        }
        qq = qq->sig;
    }
    return 0;
}

int or(Queue* qq){
    while(qq){
        if ((int) (qq->val)){
            return 1;
        }
        qq = qq->sig;
    }
    return 0;
}

/**
 * @brief Given a comparison function `f`, a value `a` and a List, tells whether the value belongs to the list.
 * 
 * @param f 
 * @param a 
 * @param qq 
 * @return int 
 */
int elemBy(int (*f)(void*,void*),void* a, Queue*qq){
    while(qq){
        if (f(qq->val,a)){
            return 1;
        }
        qq = qq->sig;
    }
    return 0;
}


/**
 * @brief Given a comparison function (closure) `c`, a value `a` and a List, tells whether the value belongs to the list.
 * 
 * @param c
 * @param a 
 * @param qq 
 * @return int 
 */
int elemByC(Closure* c,void* a, Queue*qq){
    while(qq){
        if ((int) applyAndGelValN(c,2,qq->val,a)){
            return 1;
        }
        qq = qq->sig;
    }
    return 0;
}

/**
 * @brief Yields the length of the list.
 * 
 * @param qq 
 * @return int 
 */
int length(Queue*qq){
    int i = 0;
    while(qq){
        i++;
        qq = qq->sig;
    }

    return i;
}