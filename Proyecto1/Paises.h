 
#ifndef PAIS_H_
#define PAIS_H_

typedef struct Pais Pais;
extern Queue* paises;
char* nombre(Pais* p);
char* region(Pais* p);
int poblacionTotal(Pais* p);
int viajerosDiarios(Pais* p);
int contagiadosCierreAeropuertos(Pais* p);
int contagiadosCuarentena(Pais* p);
int contagiadosCierreNegocios(Pais* p);
int contagiadosClausuraMercados(Pais* p);
int contagiadosDetenerTransporte(Pais* p);
int domesticos(Pais* p);
double claseAlta(Pais* p);
double claseMedia(Pais* p);
double claseBaja(Pais* p);
double compradores(Pais* p);
double mercado(Pais* p);
double limitacionesMedia(Pais* p);
double limitacionesBaja(Pais* p);
Pais* lookupByName(char* nombre);
void updatePais(char* name);

#endif // PAIS_H_