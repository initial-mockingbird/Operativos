#ifndef CATEGORIAS_H_
#define CATEGORIAS_H_
#define ELEMENT char*
struct words_list {
    ELEMENT value;
    struct words_list * next;
};

struct category
{
    char* name;
    struct words_list *words;
    struct category* next;
};

void readData(struct category **categories);
Queue* wordsToList(struct words_list* words);
Queue* categoriesToList(struct category *categories);
int countPhraseBelongsToString(Queue*s, Queue* ph);
int countsCategory(char* s, Queue* cat);
int yieldCategory(char*s, Queue* categories);
#endif // CATEGORIAS_H_