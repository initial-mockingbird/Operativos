
typedef struct Queue Queue;
Queue* reverse (Queue* q);
Queue* snoc(void* value,Queue* q);
Queue* flipCons(Queue* q, void* value);
Queue* cons(void* value, Queue* q);
Queue* emptyQ();
void consP(void* value, Queue** q);
void* foldr(void* (*f)(void*, void*), void* acc, Queue* qq);
void* foldl(void* (*f)(void*, void*), void* acc, Queue* qq);
void sequenceIO(void (*f)(void*), Queue* qq);
void mapP(void* (*f)(void*), Queue* qq);