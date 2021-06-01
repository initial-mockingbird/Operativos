#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
//#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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


int main(){
    readAllDir("./0", "", 0);
    return EXIT_SUCCESS;
}