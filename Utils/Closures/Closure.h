#ifndef Closure_H_
#define Closure_H_

typedef struct Closure Closure;

Closure* mkClosure(int argc, void* func);
void* getVal (Closure* c);
void* reduceToVal(Closure* c);
Closure* apply(Closure* c,void* argument);
Closure* applyN(Closure* c, int argc, ... );
Closure* apply2(Closure*c , void* arg1, void* arg2);
Closure* applyAndGelValN(Closure* c, int argc, ... );
void* applyAndGetVal(Closure* c, void* arg1);
Closure* compose(Closure* f, Closure* g);
Closure* monadicToClosure(void* (*f)(void*) );
Closure* dyadicToClosure(void* (*f)(void *,void*) );
Closure* tryadicToClosure(void* (*f)(void *,void*, void*) );
Closure* monadicVoidToClosure(void (*f)(void*) );
#endif // Closure_H_