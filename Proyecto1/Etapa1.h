#ifndef ETAPA1_H_
#define ETAPA1_H_

/**
 * @file Etapa1.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../Utils/LinkedList/LinkedList.h"
#include "../Utils/UsefulMacros/macros.h"
#include "../Utils/Closures/Closure.h"
#include "Mensaje.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>
#include "Modelos.h"




sem_t buzonMundo;

int aeropuertoCerrado(Pais* pais);
void publish_messages(Region* region, Queue* mensajes,sem_t* mutex, int* mensajesSalida);
void produce_flight_msg(Region* region, Pais* pais,sem_t* mutex, int* mensajesSalida);
void consume_flight_msg(Mundo* mundo, Region* region,sem_t* mutexBuzonSalida, sem_t* mutexBuzonMundo);
void* map_if_region_name(void* mssgs []);

int count_opened_countries(Mensaje* mensaje, Region* region);
void consume_world_inbox(Mundo* mundo, sem_t* mutexBuzonMundo);
void* if_not_country_name_take(void* mssgs []);
void return_passengers(Mensaje* mensaje, Region* region);
void* if_not_country_name_give(void* mssgs []);
void add_passengers(Mensaje* mensaje, Region* region);
void intercambiarOrigenDestino(Mensaje* m);
void consume_in_inbox(Region* region);
void* produce_flight_msg_wrapper(void* argsP);
void creacion_de_mensajes(Region* region,int* vectorRegiones, int pos, sem_t* buzonSalida);
void* creacion_de_mensajes_wrapper(void* argsP);

void* envio_buzon_salida_wrapper(void* argsP);
void* consumir_inbox_wrapper(void* argsP);

void decrementarTTL(Mundo* mundo);

void* consume_world_inbox_wrapper(void* argsP);


int nullBuzonsalida(void* r);

void etapa1(Mundo* mundo);


#endif // ETAPA1_H_