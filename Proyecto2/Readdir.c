#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
//#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define helpMessage  "Modo de empleo:  <dir> [-method|-f <file>]\n"\
                        "Evalua los resultados de una elección estadounidense.\n"\
                        "dir es el directorio conteniendo los votos (ver más adelante)\n"\
                        "[-method] es uno de los siguientes flags\n"\
                        "\t-FPTP, -WTA indicando que en todos los Estados deben asignarse los votos electorales al candidato con mayoría simple en el Estado\n"\
                        "\t-split, -dist indicando que el voto correspondiente a cada Distrito Electoral debe asignarse al candidato que obtenga mayoría simple\n"\
                        "\t--NPIVC, -pop indicando que los votos del Colegio Electoral deben asignarse al candidato que obtenga mayoría simple a nivel nacional\n"\
                        "-h, --help muestra esta ayuda y finaliza\n"
#define MAX_LEN 100
#define MAX_LEVEL 3
char* concatenateStrings(char dst[], char* str1, char* str2, char* str3){
    strcat(dst, str1);
    strcat(dst, str2);
    strcat(dst, str3);
    return dst;
}

int readAllDir(char* fileDir, char* dirName, int level){
    DIR* dir;
    struct dirent* ent;    
    char *path, dst[MAX_LEN] = "";
    pid_t pid; 
    int status = 0;

    if(level > MAX_LEVEL) return EXIT_SUCCESS;

    if(level > 0){
        path = concatenateStrings(dst, fileDir, "/", dirName);
    }else{
        path = concatenateStrings(dst, fileDir, "", "");
    }

    //printf("Accediendo a: %s\n", path);
    if ((dir = opendir(path)) != NULL) {
        
        while ((ent = readdir(dir)) != NULL) {
            if(COMPSTR(ent->d_name, ".") || COMPSTR(ent->d_name, "..")) continue;
            //printf ("%s\n", ent->d_name);
            
            if(level < MAX_LEVEL - 1){
                if ((pid = fork()) == 0){
                    printf ("Proceso: %s/%s\n", path, ent->d_name);
                    readAllDir(path, ent->d_name, level + 1);
                    exit(0);
                }else if(pid < 0){
                    perror("fork() failed");
                    return EXIT_FAILURE;
                }
            }else{
                printf("%s\n", ent->d_name);
            }
            
        }

        while (wait(&status) > 0);
        closedir (dir);
        return EXIT_SUCCESS;
    } 

    perror("\t      opendir() failed");
    return EXIT_FAILURE;
}


int main(int argc, char *argv[]){
    
    if(argc < 2 || (argc > 2 && argc < 5)) {
        printf("Falta un argumento. \nPrueba --help o -h para más información.\n");
        exit(EXIT_FAILURE);
    }

    if(argc == 2 && (COMPSTR(argv[1], "-h") == 1 || COMPSTR(argv[1], "--help") == 1)) {
        printf(helpMessage);
        exit(EXIT_SUCCESS);
    } 

    char dir[25], method[10], file[15];
    
    strcpy(dir, argv[1]);
    strcpy(file, argv[4]);

    if(COMPSTR(argv[2], "-FPTP") != 1 && COMPSTR(argv[2], "-WTA") != 1 && COMPSTR(argv[2], "-split") != 1 && COMPSTR(argv[2], "-dist") != 1 && COMPSTR(argv[2], "--NPIVC") != 1 && COMPSTR(argv[2], "-pop") != 1){
        printf("Argumento invalido. \nPrueba --help o -h para más información.\n");
    }
    strcpy(method, argv[2]);
    
    
    readAllDir("./0", "", 0);
    return EXIT_SUCCESS;
}
