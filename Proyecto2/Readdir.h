#ifndef READDIR_H_
#define READDIR_H_

typedef struct Reporte{
    char* nombre;
    int   nivel;    //Niveles: [0] Pais [1] Estado [2] Distrito Electoral [3] Condado
    long  cand1;
    long  cand2;
}Reporte;

#endif // READDIR_H_