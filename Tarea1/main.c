/**
 * @file main.c
 * @author Daniel Pinto, Mariangela Rizzo, Ka Shing Fung.
 * @brief Modulo principal.
 * @version 0.1
 * @date 2021-03-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../LinkedList/LinkedList.h"
#include "./Categorias.h"
#include "./Persona.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_LEN 100
#define INTRO_MSG "Bienvenido. Acabas de ingresar a ELIZAG6, una version del bot conversacional ELIZA desarrollado por "\
                  "IBM entre 1964 y 1966 buscando parodiar al psicologo Carl Rogers.\n\nELIZA actua como un piscoterapeuta "\
                  "y te ayudara a que expreses tus emociones. Para lograrlo, tienes que comunicarte con «ella» como si estuvieses hablando con una persona real.\n\n"\
                  "Debes tener en cuenta que ELIZA no es un psicoterapeuta real, por lo que solo sirve como desahogo temporal y no puede sustituir la ayuda profesional.\n\n"\
                  "## Caveats de la version actual:\n\n"\
                  " - La respuesta de Eliza se eligen en base a la frencuencia de palabras por categoria, en vez de por jerarquia, es decir. E.g: me dio el ultimo adios, murio, fallecio"\
                  " es clasificado como mensaje de pesame y no mensaje de despedida, pues hay mas palabras de pesame que de despedida. Si dos categorias empatan, se selecciona"\
                  " la que tenga mayor prioridad.\n\n"
#define FALSE 0
#define TRUE !FALSE


/**
 * @brief Dado un input del usuario, le asigna un mensaje de respuesta según la categoría a la que pertenezca.
 * 
 * @param char*
 * @param Queue*
 * @return int
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
    if (position == 0){exit(EXIT_SUCCESS);}
    return 1;
    //return messages[position];
}

/**
 * @brief Imprime un mensaje conversacional random.
 * 
 * @return int
 */
int randomMessage(){
    char conversationalMessages[4][MAX_LEN] = {"¿Que esta pasando ahora por tu mente?", "De lo que has hecho ultimamente, ¿hay algo que quieras contarme?", 
                                                "¿Has tenido otros psicoterapeutas? Cuentame sobre ellos.", "Dime algo de lo que te guste hacer en tu tiempo libre."};

    srand (time(NULL)); 
    int position = rand() % 4;

    if (position == -1 || position > 3){
        return 0;
    }

    printf("Eliza: %s\n",conversationalMessages[position]);
    return 1;
}

int main(int argc, char const *argv[])
{
    
    crearCategorias();
    char* input = NULL;
    size_t len = 0;
    int i;
    printf(INTRO_MSG);
    printf("Eliza: Hola, soy Eliza, tu psicoterapeuta. ¿Hay algo que te esté perturbando?\n");
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
        randomMessage();

    }
    
    return EXIT_SUCCESS;
}
