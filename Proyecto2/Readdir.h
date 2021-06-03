#ifndef READDIR_H_
#define READDIR_H_

extern Queue* lvlListas[4];
#define MAX_LEN 256

typedef struct Reporte{
    char   nombre[MAX_LEN];     //Nombre del dir archivo
    int    nivel;               //Niveles: [0] Pais [1] Estado [2] Distrito Electoral [3] Condado
    long   cand1;               //Votos por candidato 1 en ese nivel
    long   cand2;               //Votos por candidato 2 en ese nivel
    int    subCounts;           //Cantidad de subdir o subarchivos en ese nivel
    Queue* subdirs;             //Lista de reportes de los subdirectorios inmediatos
}Reporte;

Reporte* initReport(char *nombre, int level);
int readAllDir(char *fileDir, char *dirName, int level, Reporte *inRp);

#endif // READDIR_H_