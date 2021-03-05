#include "../LinkedList/LinkedList.h"
#include "./Categorias.h"
#include "./Persona.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 100
#define INTRO_MSG "Soy Eliza, y este es el mensaje de inicio\n\n"
#define FALSE 0
#define TRUE !FALSE


/**
 * @brief Dado un input del usuario, le asigna un mensaje de respuesta según la categoría a la que pertenezca.
 * 
 * @param char*
 * @param Queue*
 * @return char* 
 */
int readInput(char* inp){
    char messages[12][MAX_LEN] = {"Me alegra que hayas hablado conmigo. Espero volver a verte pronto.", "Eso es muy triste de leer. Mi mas sentido pesame.",
                "Cuentame mas sobre esa persona.", "Por favor, profundiza un poco al respecto.", "Entiendo como te sientes. Expresate con libertad.",
                "¿Como te hace sentir eso?", "¿Cual crees que sea la causa de ese sentimiento?", "Te aseguro que pronto todo va a mejorar.", 
                "Sigueme contando sobre lo que me decias antes. El tema importante aqui eres tu.", 
                "Por favor, ingresa un mensaje que exprese tus inquietudes.", "Por favor, profundiza un poco al respecto.",
                "Se un poco mas especifico al respecto."};

    int position = yieldCategory(inp, categorias);
    if (position == 2){
        freudiano(inp);
    }

    if (position == -1){
        return 0;
    }

    printf("Eliza: %s\n",messages[position]);
    return 1;
    //return messages[position];
}

int main(int argc, char const *argv[])
{
    crearCategorias();
    char* input = NULL;
    size_t len = 0;
    int i;
    printf(INTRO_MSG);
    while(TRUE){
        input = NULL;
        len = 0;
        printf("Usuario: ");
        getline(&input, &len, stdin);
        i = discovery(input);
        if (i){
            continue;
        }
        i = readInput(input);
        if (i) {
            continue;
        }

    }
    
    return EXIT_SUCCESS;
}