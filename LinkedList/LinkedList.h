#ifndef LinkedList_H_
#define LinkedList_H_

typedef struct Queue Queue;
void createEmptyQ(Queue* q);

Queue* tail(Queue* q);
Queue* reverse (Queue* q);
Queue* snoc(void* value,Queue* q);
Queue* flipSnoc(Queue* q, void* value);
Queue* cons(void* value, Queue* q);
Queue* flipCons(Queue* q, void* value);
Queue* emptyQ();
Queue* append(Queue* qq, Queue* ll);
Queue* concat(Queue*qq);
Queue* map(void* (*f)(void*), Queue* qq);
Queue* zipWith (void* (*f)(void*,void*), Queue* qq, Queue* pp);
Queue* take(int n, Queue* qq);
Queue* drop(int n, Queue* qq);

void* head(Queue* q);
void* valueAt(Queue* q, int i);
void consP(void* value, Queue** q);
void* foldr(void* (*f)(void*, void*), void* acc, Queue* qq);
void* foldl(void* (*f)(void*, void*), void* acc, Queue* qq);
void sequenceIO(void (*f)(void*), Queue* qq);
void mapP(void* (*f)(void*), Queue* qq);

int length(Queue*qq);
int all(int (*f)(void*),Queue* qq);
int and(Queue* qq);
int any(int (*f)(void*),Queue* qq);
int or(Queue* qq);
int elemBy(int (*f)(void*,void*),void* a, Queue*qq);

#endif // LinkedList_H_