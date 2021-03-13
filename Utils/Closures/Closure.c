#include "./Closure.h"
#include<stdlib.h>
#include<stdarg.h>
// ---------------------- 
// |      MACROS        |
// ---------------------- 

/** @def MAX_ARGS
 * @brief Maximum number of arguments a Closure can have (limited by the maximum number of
 * arguments a C function can have).
 * 
 */
#define MAX_ARGS 253
/** @def TO_CLOSURE_FUNC
 * @brief Casts a function to the type: `[a] -> b`
 * 
 */
#define TO_CLOSURE_FUNC(f) (void* (*) (void* [])) f


// ---------------------- 
// |      STRUCTURES    |
// ----------------------

/** @struct Closure
 * @brief A naive implementation of a Closure in C.
 * @var Closure::argc
 *  Number of arguments that the function has.
 * @var Closure::argcc
 *  Number of arguments that the closure has avaliable.
 * @var Closure::args
 *  An array of pointers to the arguments. Since the maximum number of arguments in C
 *  is 253, we limit ourselves to that size.
 * @var Closure::func
 *  A pointer to a function. The function MUST receive an array of pointers to its args, all of type
 *  void* and receive `Closure::argc` as its first argument.
 */
struct Closure {
    int argc;
    int argcc;
    void* args[MAX_ARGS];
    void* func;
};


// ---------------------- 
// |   AUX FUNCTIONS    |
// ----------------------

/**
 * @brief Initializes the array of arguments in a closure to empty.
 * 
 * @param args 
 */
static void initArgs(void* args[]){
    for (int i = 0; i < MAX_ARGS; i++){
        args[i] = NULL;
    }
}

/**
 * @brief Wrapper around a monadic function, useful only inside a closure.
 * 
 * @param args 
 * @return void* 
 */
static void* fromMonadic(void* args[]){
    void* (*f) (void*) = args[0];
    void* x = args [1];
    return f(x);
}

/**
 * @brief Wrapper around a dyadic function, useful only inside a closure.
 * 
 * @param args 
 * @return void* 
 */
static void* fromDyadic(void* args[]){
    void* (*f) (void*, void*) = args[0];
    void* x = args [1];
    void* y = args[2];
    return f(x,y);
}

/**
 * @brief Wrapper around a tryadic function, useful only inside a closure.
 * 
 * @param args 
 * @return void* 
 */
static void* fromTryadic(void* args[]){
    void* (*f) (void*, void*, void*) = args[0];
    void* x = args [1];
    void* y = args[2];
    void* z = args[3];
    return f(x,y,z);
}

/**
 * @brief Wrapper around a monadic function that returns Unit, useful only inside a closure.
 * 
 * @param args 
 * @return void* 
 */
static void* fromMonadicVoid(void* args[]){
    void (*f) (void*) = args[0];
    void* x = args [1];
    f(x);
    return EXIT_SUCCESS;
}


/**
 * @brief Given two closures: `g` and `f`, composes `f . g` and it evaluates them.
 * 
 * @param args The arguments of the closure composition, such that: `args[0]` has a pointer to
 * the closure of `g`, `args[1]` has a pointer to the closure of `f`, and `args[2:i]`, `args[i+1:end]`
 * has the necessary arguments (aside the arguments inside the closures of `g` and `f`) in order to
 * evaluate `g` (completely) and `f` (parcially, since we need to evaluate `g`).
 * @return void* The result `(f . g)`
 */
static void* closureComp(void* args[]){
    // Taking out the closures out of the arguments.
    Closure* g = (Closure*) args[0];
    Closure* f = (Closure*) args[1];

    // Initializing the arguments that g needs in order to be evaluated.
    void* g_args[253]; // = (void*) malloc(253 * sizeof(void));
    int i = 0;
    int j = 2;
    // The arguments of g are the arguments of the closure of g.
    for (; i < g->argcc; i++){
        
        g_args[i] = (g->args)[i];
    }
    // plus the arguments of the closure of the composition.
    for (; i < g->argc; i++){
        
        g_args[i] = args[j];
        j++;
    }
    // Evaluating g with the provided arguments. The output of g is the 
    // last parameter of f.
    void* gx   = (TO_CLOSURE_FUNC(g->func))(g_args);
    
    // Initializing the arguments that f needs in order to be evaluated.
    void* f_args[253]; // = (void*) malloc(253 * sizeof(void));
    i = 0;
    // The arguments of f are the arguments of the closure of f.
    for (; i < f->argcc; i++){
        
        f_args[i] = f->args[i];
    }
    // plus the arguments of the closure of the composition that comes after the arguments of g.
    for (; i < (f->argc - 1); i++){
        
        f_args[i] = args[j];
        j++;
    }
    // plus the output of g.
    f_args[i] = gx;
    

    // Evaluating f with the provided arguments. 
    void* fgx = (TO_CLOSURE_FUNC(f->func))(f_args);
    

    return fgx;

}


// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Creates a closure given its arity and a pointer to the function.
 * 
 * @param argc Arity of the function
 * @param func A (void) pointer to the function (That should have signature: void* (*f) (void* []))
 * @return Closure* A closure object with arity `argc`, function `func`, number of avaliable arguments `0` and an array of NULL pointers.
 */
Closure* mkClosure(int argc, void* func){
    Closure* c = (Closure*) malloc(sizeof(Closure));
    c->argc  = argc;
    c->argcc = 0;
    initArgs(c->args);
    c->func  = func;

    return c;
}

/**
 * @brief evaluates the closure provided that there are enough arguments.
 * 
 * @param c 
 * @return void* 
 */
void* reduceToVal(Closure* c){

    // Deberia poner algo que mane
    if (c->argc != c->argcc){
        return NULL;
    }

    void* (*f) (void* []) = TO_CLOSURE_FUNC (c->func);
    return  f(c->args);
}

/**
 * @brief Gets the value out of the closure.
 * 
 * @param c 
 * @return void* 
 */
void* getVal (Closure* c){
    return c->func;
}

/**
 * @brief Given a closure and an argument, returns another closure with the argument applied.
 * 
 * @param c 
 * @param argument 
 * @return Closure* 
 */
Closure* apply(Closure* c,void* argument){

    // Should I return an error if  i try to apply more arguments?
    if (c->argc == c->argcc){
        return c;
    }

    // Copying the data, not sure if really necessary.
    Closure* cc = mkClosure(c->argc, c->func);
    cc-> argcc = c->argcc + 1;
    int j = 0;
    for(int i =0; i<MAX_ARGS; i++){
        (cc->args)[i] = (c->args)[i];
    }

    while((cc->args)[j]){
        j++;
    }

    (cc->args)[j] = argument;

    // Should apply also reduce?
    if (cc->argc == cc->argcc){
        cc->func = reduceToVal(cc);
    }

   return cc;
}

/**
 * @brief Applies a variable length number of arguments to our closure c.
 * 
 * @param c 
 * @param argc 
 * @param ... 
 * @return Closure* 
 */
Closure* applyN(Closure* c, int argc, ... ){
    Closure* cc = c;
    va_list ap;
    va_start(ap,argc);
    for(int i=0; i< argc; i++){
        cc = apply(cc,va_arg(ap,void*));
    }
    va_end(ap);
    return cc;
}

/**
 * @brief Shorthand for applyN(c,2,arg1,arg2).
 * 
 * @param c 
 * @param arg1 
 * @param arg2 
 * @return Closure* 
 */
Closure* apply2(Closure*c , void* arg1, void* arg2){
    return apply(apply(c, arg1),arg2);
}

/**
 * @brief Applies a variable length number of arguments to our closure c and returns the final value.
 * 
 * @param c 
 * @param argc 
 * @param ... 
 * @return Closure* 
 */
Closure* applyAndGelValN(Closure* c, int argc, ... ){
    Closure* cc = c;
    va_list ap;
    va_start(ap,argc);
    for(int i=0; i< argc; i++){
        cc = apply(cc, va_arg(ap,void*));
    }
    va_end(ap);
    return getVal(cc);
}

/**
 * @brief Shorthand for ApplyAndGetValN(c,1,arg1).
 * 
 * @param c 
 * @param arg1 
 * @return void* 
 */
void* applyAndGetVal(Closure* c, void* arg1){
    return getVal(apply(c,arg1));
}





/**
 * @brief Given a closure `f` and a closure `g`, yields the composition of two closures: `f . g`
 * 
 * @param f 
 * @param g 
 * @return Closure* 
 */
Closure* compose(Closure* f, Closure* g){
    // The total number of arguments is: whats left to evaluate f
    // + whats left to evaluate g + 2 (the two closures)
    // - 1 (since one value of f is yielded by the evaluation of g).
    int argc   = (f->argc - f->argcc) + (g->argc - g->argcc) + 2 - 1;
    Closure* c = mkClosure(argc, closureComp);
    // We must save the two closures
    (c->args)[0] = g;
    (c->args)[1] = f;
    // Since we saved the two closures, we have 2 initial arguments.
    c->argcc     = 2;
    return c;
}

/**
 * @brief Creates a closure out of a monadic function.
 * 
 * @param f 
 * @return Closure* 
 */
Closure* monadicToClosure(void* (*f)(void*) ){
    Closure* c =  mkClosure(2,fromMonadic);
    return apply(c, (void*) f);
}

/**
 * @brief Creates a closure out of a dyadic function.
 * 
 * @param f 
 * @return Closure* 
 */
Closure* dyadicToClosure(void* (*f)(void *,void*) ){
    Closure* c =  mkClosure(3,fromDyadic);
    return apply(c, (void*) f);
}

/**
 * @brief Creates a closure out of a tryadic function.
 * 
 * @param f 
 * @return Closure* 
 */
Closure* tryadicToClosure(void* (*f)(void *,void*, void*) ){
    Closure* c =  mkClosure(4,fromTryadic);
    return apply(c, (void*) f);
}

/**
 * @brief Creates a closure out of a monadic void function.
 * 
 * @param f 
 * @return Closure* 
 */
Closure* monadicVoidToClosure(void (*f)(void*) ){
    Closure* c =  mkClosure(2,fromMonadicVoid);
    return apply(c, (void*) f);
}