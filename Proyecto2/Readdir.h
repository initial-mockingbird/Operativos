#ifndef READDIR_H_
#define READDIR_H_
#include <pthread.h>
#include <semaphore.h>
#include "super.h"
#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"

// ---------------------- 
// |      VARIABLES     |
// ----------------------

extern Queue* lvlListas[4];
extern char nombre1[MAX_LEN], nombre2[MAX_LEN];

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------
Reporte* initReport(char *nombre, int level);
int readAllDir(char *fileDir, char *dirName, int level, Reporte *inRp);
char* concatenateStrings(char dst[], char *str1, char *str2, char *str3);
void lecturaModo(char* path);
#endif // READDIR_H_