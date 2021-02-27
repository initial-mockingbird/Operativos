#ifndef LinkedList_H_
#define LinkedList_H_

typedef struct Queue Queue;
void createEmptyQ(Queue* q);
Queue* reverse (Queue* q);
Queue* snoc(void* value,Queue* q);
Queue* flipSnoc(Queue* q, void* value);
Queue* cons(void* value, Queue* q);
Queue* flipCons(Queue* q, void* value);
Queue* emptyQ();
Queue* append(Queue* qq, Queue* ll);
Queue* concat(Queue*qq);

void consP(void* value, Queue** q);
void* foldr(void* (*f)(void*, void*), void* acc, Queue* qq);
void* foldl(void* (*f)(void*, void*), void* acc, Queue* qq);
void sequenceIO(void (*f)(void*), Queue* qq);
void mapP(void* (*f)(void*), Queue* qq);

#endif // LinkedList_H_