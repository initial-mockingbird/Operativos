#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "Readdir.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

// ---------------------- 
// |      MACROS        |
// ----------------------
/** @def MAX_LEN
 * @brief Maxima longitud de una cadena de texto.
 * 
 */
#define MAX_LEN 256

/** @def MAX_LEVEL
 * @brief Número de accesos que hay que realizar hasta llegar a los archivos .txt
 * 
 */
#define MAX_LEVEL 3

Queue* lvlListas[4] = {NULL};
char  nombre1[MAX_LEN], nombre2[MAX_LEN];
int    flag = 0;

// ---------------------- 
// |    AUX FUNCTIONS   |
// ----------------------

/**
 * @brief Concatena tres strings.
 * @param dst   Arreglo en donde se guarda el string resultante
 * @param str1  String No.1
 * @param str2  String No.2
 * @param srt3  String No.3
 * @return char*
 */
char* concatenateStrings(char dst[], char *str1, char *str2, char *str3){
    //Concatenar dst con str_i, para i = 1, 2, 3
    strcat(dst, str1);
    strcat(dst, str2);
    strcat(dst, str3);
    return dst;
}

char* trim(char *str) {
    int l = strlen(str);

    while(isspace(str[l - 1])) --l;
    while(*str && (isspace(*str) || *str == '\r' || *str == '\n' || *str == '\t')) ++str, --l;

    return strndup(str, l);
}

/**
 * @brief Chequea si un string contiene sólo cifras [0-9].
 * @param str String a chequear
 * @return int
 */
int isNumber(char *str){
    str = trim(str);
    //Iterar sobre string y confirmar que cada carácter es una cifra [0-9]
    while(*str != '\0'){
        if(*str < '0' || *str > '9') return 0;
        str++;
    }
    return 1;
}

// ---------------------- 
// |      FUNCTIONS     |
// ----------------------

/**
 * @brief Crea, inicializa y devuelve un reporte.
 * @param nombre    Nombre del directorio/archivo a reportar
 * @param level     Nivel en que se encuentra el directorio 
 * @return Reporte*
 */
Reporte* initReport(char *nombre, int level){
    struct Reporte *rp;
    //Asignar memoria
    rp = malloc(sizeof(struct Reporte));
    //Inicializar valores
    strcpy(rp->nombre, nombre);
    rp->nivel     = level;
    rp->cand1     = 0;
    rp->cand2     = 0;
    rp->subCounts = 0;
    rp->subDirs   = NULL;

    return rp;
}

/**
 * @brief Actualiza un reporte, considerando los datos de un reporte de su subdirectorio/archivo.
 * @param nextRp    Reporte del directorio a actualizar
 * @param prevRP    Reporte de un subdirectorio/archivo
 */
void updateReport(Reporte *nextRp, Reporte *prevRp){
    //Sumar votos, y aumentar en 1 el número de directorios/archivos leídos.
    //Añadir ptr del subdirectorio al una lista en el directorio
    prevRp->cand1     += nextRp->cand1;
    prevRp->cand2     += nextRp->cand2;
    prevRp->subCounts += 1;
    prevRp->subDirs    = snoc((void*) nextRp, prevRp->subDirs);
}

/**
 * @brief Chequea si un archivo es un archivo .txt.
 * @param filename Nombre del archivo
 * @return int
 */
int checkTXT(char *fileName){
    char str[MAX_LEN], *token;
    const char s[2] = ".";

    strcpy(str, fileName);

    token = strtok(str, s);
    while( token != NULL ){
        if( COMPSTR(token, "txt") ) return 1;
        token = strtok(NULL, s);
    }
    return 0;
}

/**
 * @brief Obtiene la cantidad de votos de un string.
 * @param str String a procesar
 * @return int 
 */
int getVotes(char *str){
    int   votes = 0;
    char  buff [MAX_LEN], data [MAX_LEN];
    char *token;
    const char s[2] = ":";
    
    strcpy(buff, str);

    //Obtener string después del ":", en caso de no existir devolver el previo
    token = strtok(buff, s);
    while( token != NULL ){
        //Copiar nombres de los candidatos
        if ( flag == 0 ){
            strcpy(nombre1, token);
            flag += 1;
        }else if ( flag == 1 ){
            flag += 1;
        }else if ( flag == 2 ){
            strcpy(nombre2, token);
            flag += 1;
        }
        
        //Resguardar token
        strcpy(data, token);
        token = strtok(NULL, s);
    }

    //Caso: "Candidato:" cuando no se indica los votos se considera como 0
    if(isNumber(data)){
        votes = atoi(data);
    }
    
    return votes;
}

/**
 * @brief Lee y obtiene votos de un archivo .txt, y actualiza el reporte correspondiente.
 * @param fileDir   Path que contiene el directorio en donde está el archivo
 * @param dirName   Nombre del archivo
 * @param outRp     Reporte a actualizar
 * @return int 
 */
Reporte* readFile(char *fileDir, char *dirName, Reporte *outRP){
    //Si no es un archivo .txt return 
    if( checkTXT(dirName) < 1 ) return;

    FILE *fp;
    char linebuf[MAX_LEN], *path;
    char dst[MAX_LEN] = "";
    
    //Obtener path al archivo
    path = concatenateStrings(dst, fileDir, "/", dirName);
    //Abrir archivo
    if( (fp = fopen(path, "r")) == NULL ){
        printf("Error opening file: %s\n", path);
        return;
    }
    //Leer las dos primeras líneas y contabilizar votos
    fgets(linebuf, sizeof(linebuf), fp);
    outRP->cand1 = getVotes(linebuf);
    fgets(linebuf, sizeof(linebuf), fp);
    outRP->cand2 = getVotes(linebuf);
    //Cerrar archivo
    fclose(fp);

    return outRP;
}

/**
 * @brief Recorre recursivamente directorios y subdirectorios, de un path dado.
 * @param fileDir   Path que contiene el directorio en donde está el archivo
 * @param dirName   Nombre del archivo
 * @param inRP      Reporte a actualizar
 * @return int 
 */
int readAllDir(char *fileDir, char *dirName, int level, Reporte *inRp){
    struct Reporte *outRp;  
    struct DIR     *dir;
    struct dirent  *ent; 
    char dst[MAX_LEN] = "", *path;
    int status        = 0;

    //Obtener path al directorio
    if( level > 0 ){
        path = concatenateStrings(dst, fileDir, "/", dirName);
    }else{
        path = concatenateStrings(dst, fileDir, "", "");
    }

    //Abrir el directorio 
    if ( (dir = opendir(path)) != NULL ) {
        
        while ( (ent = readdir(dir)) != NULL ){
            //Continue, si el nombre de archivo es "." o "..". 
            //Se evita retornar a directorios ya visitados
            if( COMPSTR(ent->d_name, ".") || COMPSTR(ent->d_name, "..") ) continue;

            //Imprimir cuál es el archivo que se está leyendo al momento
            //printf("Reading: %s\n", ent->d_name);

            //Inicializar reporte
            outRp = initReport(ent->d_name, level + 1);
            //Guardar reporte en la lista del nivel correspondiente
            lvlListas[level + 1] = snoc((void*) outRp, lvlListas[level + 1]);

            //Niveles: [0] Pais [1] Estado [2] Distrito Electoral [3] Condado
            //Si no se ha llegado al nivel de los condados, continuar recursión
            //Caso contrario, contar los votos
            if( level < MAX_LEVEL - 1 ){
                readAllDir(path, ent->d_name, level + 1, outRp);
            }else{
                printf("Counting: %s\n", ent->d_name);
                outRp = readFile(path, ent->d_name, outRp);
            }
            updateReport(outRp, inRp);                        
        }
        closedir(dir);        
        return EXIT_SUCCESS;
    }

    //En caso de no poder abrir el directorio, indicar cuál e imprimir error
    printf("%s \n", path);
    perror("\t opendir() failed");
    return EXIT_FAILURE;
}



