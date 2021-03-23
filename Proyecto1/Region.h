
#ifndef REGION_H_
#define REGION_H_
#define ELEMENT char*
struct regiones_aliadas {
    ELEMENT siglas;
    struct regiones_aliadas * next;
};

struct region
{
    char* siglas;
    struct regiones_aliadas *aliadas;
    struct category* next;
};

extern Queue* regiones;
Queue* aliadasToList(struct regiones_aliadas* words);
Queue* regionesToList(struct region *categories);
void crearCategorias();
#endif // REGION_H_