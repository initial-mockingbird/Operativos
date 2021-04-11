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
//#include <pthread.h>
//#include <semaphore.h>
#include <unistd.h>
#include "Etapa1.h"
#include <math.h>
#include "Modelos.h"


#define TIEMPO_VUELO 2

sem_t buzonMundo;

/**
 * @brief Dice si un pais posee cerrados sus aeropuertos.
 * 
 * @param pais 
 * @return int 1 si es asi, 0 de otra forma.
 */
int aeropuertoCerrado(Pais* pais){
    long long infectados = 0;
    for (int i =0; i< 3; i++){
        infectados += pais->infectadosClase[i];
    }

    return infectados > pais->contagiadosCierreAeropuertos;
}

/**
 * @brief Publica los mensajes de salida generados por un pais al buzon de salida de una region.
 * 
 * @param region Region a la cual publicar los mensajes.
 * @param mensajes Una lista de mensajes a publicar.
 */
void publish_messages(Region* region, Queue* mensajes,sem_t* mutex, int* mensajesSalida){
    // Esperemos a que nadie esta publicando (paises) o despachando (region) mensajes.
    sem_wait(mutex);

    // Luego publicamos TODOS los mensajes de una
    while(mensajes){
        region->buzonSalida = snoc((Mensaje*) head(mensajes), region->buzonSalida);
        mensajes = tail(mensajes);
    }

    // Y le decimos a la region que ya un pais termino su envio de mensajes,
    *mensajesSalida += 1;

    // liberamos el semaforo.
    sem_post(mutex);
}

/**
 * @brief Produce y publica los mensajes de un pais a todas sus regiones aliadas.
 * 
 * @param region Region a la que publicar los mensajes en su buzon de salida.
 * @param pais   Pais perteneciente a la region al cual debemos crear los mensajes.
 * @param mutex  Semaforo del buzon de salida.
 */
void produce_flight_msg(Region* region, Pais* pais,sem_t* mutex, int* mensajesSalida){

    Mensaje* egresados;
    Mensaje* mensajeInterno;
    Queue* mensajes = emptyQ();

    // Si el aeropuerto del pais esta cerrado
    if (aeropuertoCerrado(pais)){

        // Creamos un mensaje vacio, con destino el propio pais, TTL = 0, y sin pasajeros.
        egresados = emptyMessage(pais->region, pais->nombre);
        mensajes = cons(egresados, mensajes);
        // lo publicamos
        publish_messages(region,mensajes, mutex, mensajesSalida);
        // y shacabo.
        return;
    }


    // asumiendo que las regiones aliadas no se contienen a si mismas.
    int numeroRegiones  = length(region->aliadas);
    // sacamos tantos pasajeros como los viajeros diarios, 
    // a menos que haya mas viajeros que poblacion, en tal caso sacamos la poblacion
    // (si pasa esto, entonces o los datos de la simulacion son bien XD o c murieron toditos).
    int numeroViajeros = MIN(pais->viajerosDiarios, pais->poblacionTotal);
    // Solo las proporciones de infectados: personas en la clase / personas infectadas en la clase.
    // Si no hay infectados, entonces la proporcion es 0.
    double proporcion_infectados_alta  = pais->claseAlta  == 0 ? 0 : pais->infectadosClase[0] / pais->claseAlta;
    double proporcion_infectados_media = pais->claseMedia == 0 ? 0 : pais->infectadosClase[1] / pais->claseMedia;
    double proporcion_infectados_baja  = pais->claseBaja  == 0 ? 0 : pais->infectadosClase[2] / pais->claseBaja;
    // el calculo de cuantos pasajeros tenemos que mandar al exterior, en punto flotante.
    double contenido[3] = {     MIN( 0.6 * numeroViajeros, pais->claseAlta), 
                                MIN( 0.3 * numeroViajeros, pais->claseMedia),
                                MIN( 0.1 * numeroViajeros, pais->claseBaja) };
    // el calculo de cuantos de estos pasajeros estan infectados.
    double infectados[3] = {    contenido[0] * proporcion_infectados_alta, 
                                contenido[1] * proporcion_infectados_media, 
                                contenido[2] * proporcion_infectados_baja };

    // Ahora, la mitad de los pasajeros calculados en contenido van a ir a los paises aliados de la misma region
    double interno[3]           = {contenido[0] / 2, contenido[1] / 2, contenido[2] / 2 };
    // Lo mismo para los infectados.
    double infectadosInterno[3] = {infectados[0] / 2, infectados[1] / 2, infectados[2] / 2 };

    // Ahora creamos los mensajes.

    // Hay un mensaje que se manda a la misma region, este es el mensaje interno.
    mensajeInterno = (Mensaje*) malloc(sizeof(Mensaje));
    mensajeInterno->regionOrigen   = (char*) malloc((strlen(pais->region) +1) * sizeof(char));
    mensajeInterno->regionDestino  = (char*) malloc((strlen(pais->region) +1) * sizeof(char));
    mensajeInterno->paisOrigen     = (char*) malloc((strlen(pais->nombre) +1) * sizeof(char));
    strcpy(mensajeInterno->regionOrigen, pais->region);
    strcpy(mensajeInterno->regionDestino, pais->region);
    strcpy(mensajeInterno->paisOrigen, pais->nombre);
    mensajeInterno->regionOrigen  = pais->region;
    mensajeInterno->regionDestino = pais->region;
    mensajeInterno->paisOrigen    = pais->nombre;
    mensajeInterno->TTL           = TIEMPO_VUELO;
    mensajeInterno->estado        = 1;

    
    
    for (int i = 0; i < 3; i++){

        mensajeInterno->contenido[i]  = interno[i];
        mensajeInterno->infectados[i] = infectadosInterno[i];
        
    }
    
    
    // Anadiendo el mensaje interno a la lista de mensajes por enviar.
    mensajes = cons(mensajeInterno, mensajes);

    // Ahora, la otra mitad se debe dividir entre las demas regiones aliadas, como  la otra mitad contiene la misma
    // cantidad que la mitad de internos, usamos esta variable como referencia.
    // TODO: de verdad considerar crear: fmap, fold, sequence y closures para arreglos, ya sera para el proximo proyecto.
    // a C le faltan demasiadas vainas cul.


    double externo[3]            = {0,0,0};
    double infectadosExterno [3] = {0,0,0};
    if (numeroRegiones != 0){
        for (int i = 0; i < 3; i ++){
            externo[i]            = interno[i]/numeroRegiones;
            infectadosExterno [i] = infectadosInterno[i] / numeroRegiones;
        }
        
    } 
    
    

    Queue* regionesAliadas = region->aliadas;

    // Ahora, creamos un mensaje para cada region.
    while(regionesAliadas){
        Mensaje* mensajeExterno        = (Mensaje*) malloc(sizeof(Mensaje));
        mensajeExterno->regionOrigen   = (char*) malloc((strlen(pais->region) +1) * sizeof(char));
        mensajeExterno->regionDestino  = (char*) malloc((strlen((char*) head(regionesAliadas)) +1) * sizeof(char));
        mensajeExterno->paisOrigen     = (char*) malloc((strlen(pais->nombre) +1) * sizeof(char));
        strcpy(mensajeExterno->regionOrigen, pais->region);
        strcpy(mensajeExterno->regionDestino, (char*) head(regionesAliadas));
        strcpy(mensajeExterno->paisOrigen, pais->nombre);
        mensajeExterno->TTL            = TIEMPO_VUELO;
        mensajeExterno->estado         = 1;
        for (int i =0; i<3; i++){
            mensajeExterno->contenido[i]  = externo[i];
            mensajeExterno->infectados[i] = infectadosExterno[i];
        }
        
        mensajes                      = cons(mensajeExterno, mensajes);
        regionesAliadas               = tail(regionesAliadas);
    }

    // Tambien notemos que no es necesario ninguna sincronizacion ya que el proceso de devolucion toma lugar DESPUES
    // de que todos hayan enviado sus mensajes, es decir, despues de llamar a publish_message. 
    pais->poblacionTotal  = pais->poblacionTotal - (interno[0] + interno[1] + interno[2] + numeroRegiones * (externo[0] + externo[1] + externo[2]) );
    pais->claseAlta       = pais->claseAlta  - (interno [0] + numeroRegiones * externo[0]);
    pais->claseMedia      = pais->claseMedia - (interno [1] + numeroRegiones * externo[1]);
    pais->claseBaja       = pais->claseBaja  - (interno [2] + numeroRegiones * externo[2]);
    /*for (int i = 0; i<3; i++){
        pais->infectadosClase[i] = pais->infectadosClase[i] - (infectadosInterno[i] + numeroRegiones * infectadosExterno[i]);
    }*/

    // publicamos todos los mensajes
    publish_messages(region,mensajes,mutex, mensajesSalida);

    // En este punto, ya el pais no tiene que generar mas mensajes :D.
    return ;
}

/**
 * @brief Despacha los mensajes del buzon de salida de una region.
 * 
 * @param mundo El MUNDO, contiene a todas las regiones y un buzon que redistribuye los mensajes.
 * @param region La region que debe despachar sus mensajes al mundo.
 * @param mutexBuzonSalida Un semaforo relacionado al buzon de salida de la region.
 * @param mutexBuzonMundo  Un semaforo relacionado al buzon del mundo.
 */
void consume_flight_msg(Mundo* mundo, Region* region,sem_t* mutexBuzonSalida, sem_t* mutexBuzonMundo){
    
    // Mientras el buzon de salida no este vacio.
    // (nota, no necesitamos sincronizacion puesto que solo deseamos vaciar el buzon de salida
    // y esta rutina se llamara de forma constante).
    while(region->buzonSalida){
        Mensaje* mensaje = (Mensaje*) malloc(sizeof(Mensaje));
        // esperamos a que el buzon de salida este libre para despachar el mensaje
        sem_wait(mutexBuzonSalida);
        mensaje = (Mensaje*) head(region->buzonSalida);
        region->buzonSalida = tail(region->buzonSalida);
        // terminamos con el buzon de salida.
        sem_post(mutexBuzonSalida);
        // esperamos a que el buzon del mundo este listo para publicar el mensaje
        sem_wait(mutexBuzonMundo);
        mundo->buzonMundo = snoc(mensaje, mundo->buzonMundo);
        // Terminamos con el buzon del mundo.
        sem_post(mutexBuzonMundo);        
    }

    return ;
}

/**
 * @brief Publica un mensaje en el buzon de entrada de una region si el nombre de la region
 * es el mismo que el proveido.
 * 
 * @param mssgs un arreglo de dos posiciones, la primera alberga un mensaje, y la segunda una region.
 * @return void* EXIT_SUCCESS si se finaliza la ejecucion.
 */
void* map_if_region_name(void* mssgs []){
    Mensaje* mensaje = (Mensaje*) mssgs[0];
    Region*  region  = (Region*) mssgs[1];
    
    if (COMPSTR(mensaje->regionDestino, region->siglas)){
        region->buzonEntrada = cons(mensaje, region->buzonEntrada);
    }

    return region->buzonEntrada;
}

int count_opened_countries(Mensaje* mensaje, Region* region){
    
    int totalPaises = length(region->paises);

    if (!COMPSTR(mensaje->regionDestino, region->siglas)){
        return 0;
    }

    if (COMPSTR(mensaje->regionDestino, mensaje->regionOrigen)){
        totalPaises --;
    }

    Queue* paises = region->paises;

    while(paises){
        if (aeropuertoCerrado((Pais*) head(paises))){
            totalPaises --;
        }
        paises = tail(paises);
    }

    totalPaises = MAX(0,totalPaises);

    return totalPaises;
}


/**
 * @brief Despacha TODOS los mensajes del buzon del mundo.
 * 
 * @param mundo 
 * @param mutexBuzonMundo 
 */
void consume_world_inbox(Mundo* mundo, sem_t* mutexBuzonMundo){
    // Espera a que este disponible er buzon del mundo
    sem_wait(mutexBuzonMundo);
    int totalPaisesAbiertos;
    Queue* aux;
    while(mundo->buzonMundo){
        // Saque el mensaje del buzon.
        Mensaje* m = head(mundo->buzonMundo);
        // ya cuenta como despachao.
        mundo->buzonMundo = tail(mundo->buzonMundo);
        // cree una clausura para la funcion de mapeo de buzones de entradas.
        Closure* addToInbox = mkClosure(2,map_if_region_name);
        // Aplica el mensaje que se quiere enviar a la clausura, aka: CURRY IT.
        addToInbox = apply(addToInbox,m);
        // y mapeamos cada region con la funcion para actualizarlo.
        mapC(addToInbox,mundo->regiones);
    }
    // Libere er buzon del mundo.
    sem_post(mutexBuzonMundo);
}



/**
 * @brief Si el pais no posee el nombre dado, entonces le resta el numero de pasajeros que debe devolver.
 * 
 * @param mssgs un arreglo de dos posiciones, la primera alberga el total de paises, la segunda un mensaje, y la tercera una region.
 * @return void* EXIT_SUCCESS si se finaliza la ejecucion.
 */
void* if_not_country_name_take(void* mssgs []){
    int totalPaises  = (int)      mssgs[0];
    Mensaje* mensaje = (Mensaje*) mssgs[1];
    Pais*  pais      = (Pais*)    mssgs[2];
    
    // Si el pais de origen origino el mensaje, entonces eso significa que no hay que quitarle nada.
    if (COMPSTR(mensaje->paisOrigen, pais->nombre)){
        return EXIT_SUCCESS;
    }

    // Si el pais de origen no es el mismo que el que origino el mensaje, entonces le quitamos tanta poblacion
    // como el mensaje diga / la cantidad de oaises de la region (-1 si es la misma region, pero de esto se encarga
    // el que llama la funcion).
    if (totalPaises != 0){
        pais->claseAlta   =  MAX(pais->claseAlta  - mensaje->contenido[0] / totalPaises,0);
        pais->claseMedia  =  MAX(pais->claseMedia - mensaje->contenido[1] / totalPaises,0);  
        pais->claseBaja   =  MAX(pais->claseBaja  - mensaje->contenido[2] / totalPaises,0);
    }
    

    // actualizamos la poblacion total.
    pais->poblacionTotal = ceil(pais->claseAlta + pais->claseMedia + pais->claseBaja);

    // y actualizamos los infectados.
    if (totalPaises != 0){
        for (int i=0; i <3 ;i++){
            pais->infectadosClase[i] = MAX(pais->infectadosClase[i] - mensaje->infectados[i] / totalPaises,0);
        }
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Funcion que actualiza la lista de paises para expresar que un vuelo ha de regresar.
 * 
 * @param mensaje 
 * @param region 
 */
void return_passengers(Mensaje* mensaje, Region* region){


    Queue* paises   = region->paises;
    int totalPaises = length(paises);
    // Si el pais es intra region, entonces le quitamos 1 al total de paises.
    if (!COMPSTR(mensaje->regionDestino, mensaje->regionDestino)){
        totalPaises = totalPaises - 1;
    }

    // Y aplicamos la funcion a cada uno de los paises, con totalPaises y mensajes fijos.
    Closure* updatePopulation = mkClosure(3,if_not_country_name_take);
    updatePopulation = apply(updatePopulation,(void*) totalPaises);
    updatePopulation = apply(updatePopulation,mensaje);
    mapC(updatePopulation,region->paises);

}

/**
 * @brief Si el pais posee el nombre dado, entonces le suma el numero de pasajeros que debe reincorporar.
 * 
 * @param mssgs Un arreglo cuya primera posicion contiene el mensaje, y cuya segunda posicion contiene un pais.
 * @return void* 
 */
void* if_not_country_name_give(void* mssgs []){
    int totalPaises  = (int)      mssgs[0];
    Mensaje* mensaje = (Mensaje*) mssgs[1];
    Pais*  pais      = (Pais*)    mssgs[2];
    
    // Si es un viaje de regreso Y el pais de origen del mensaje es el mismo al pais actual
    // seteamos el total de paises como 1 para que los calculos fluyan.
    if (!mensaje->estado && COMPSTR(mensaje->paisOrigen, pais->nombre)){
        totalPaises = 1;
    }

    // Si el mensaje es de ida, y:
    // - el pais de origen es el mismo que el pais actual (mensaje intra region).
    // - o el aeropuerto esta cerrado
    // no hacemos nada.
    if ( mensaje->estado && (COMPSTR(mensaje->paisOrigen, pais->nombre) || aeropuertoCerrado(pais) )) {

        return EXIT_SUCCESS;
    }

    
    // Sino, anadimos de vuelta los pasajeros que se fueron.
    pais->claseAlta   = pais->claseAlta  + mensaje->contenido[0] / totalPaises;
    pais->claseMedia  = pais->claseMedia + mensaje->contenido[1] / totalPaises;
    pais->claseBaja   = pais->claseBaja  + mensaje->contenido[2] / totalPaises;

    pais->poblacionTotal = round(pais->claseAlta + pais->claseMedia + pais->claseBaja);
    
    for (int i=0; i <3 ;i++){        
        pais->infectadosClase[i] = pais->infectadosClase[i] + mensaje->infectados[i] / totalPaises;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief  Funcion que actualiza la lista de paises para expresar que un vuelo ha regresado y los pasajeros estan listos
 * para volver a su pais.
 * 
 * @param mensaje 
 * @param region 
 */
void add_passengers(Mensaje* mensaje, Region* region){

    Queue* paises   = region->paises;
    int totalPaises = length(region->paises);
    // Si es un mensaje intra region, le quitamos 1 al total de paises, para no enviar gente al mismo pais.
    if (!COMPSTR(mensaje->regionDestino, mensaje->regionDestino) && mensaje->estado){
        totalPaises = totalPaises - 1;
    }

    // Por cada pais con aeropuerto cerrado, le quitamos 1 al total de paises que tenemos que enviar personas.
    /*while(paises){
        if (aeropuertoCerrado((Pais*) head(paises))){
            totalPaises --;
        }
        paises = tail(paises);
    }*/

    // debido si el mensaje es intra region, y el pais que lo envio esta en cuarentena,
    // entonces acabaremos con una cota inferior de -1, ergo, necesitamos este statement.
    // Sin embargo, esto no deberia ocurrir puesto que la funcion padre se encarga del chequeo
    // de 0.
    totalPaises = MAX(0,totalPaises);

    Closure* updatePopulation = mkClosure(3,if_not_country_name_give);
    updatePopulation = apply(updatePopulation,(void*) totalPaises);
    updatePopulation = apply(updatePopulation,mensaje);
    mapC(updatePopulation,region->paises);

}

void intercambiarOrigenDestino(Mensaje* m){
    char* aux = (char*) malloc(strlen(m->regionDestino) * sizeof(char));
    strcpy(aux, m->regionDestino);

    m->regionDestino = (char*) malloc(strlen(m->regionOrigen) * sizeof(char));
    strcpy(m->regionDestino, m->regionOrigen);

    m->regionDestino = (char*) malloc(strlen(aux) * sizeof(char));
    strcpy(m->regionOrigen,aux);
}

/**
 * @brief Consume todos los mensajes del inbox, actualizando la poblacion de los paises de la region.
 * 
 * @param region 
 */
void consume_in_inbox(Region* region){
    
    Queue* mensajesProcesados = emptyQ();
    int totalMensajesRecibidos = length(region->buzonEntrada);
    
    for (int i = 0; i < totalMensajesRecibidos; i++){
        Mensaje* m = (Mensaje*) head(region->buzonEntrada);
        
        region->buzonEntrada = tail(region->buzonEntrada);
        
        if (m->TTL == 0){
            
            // Si el TTL es 0, entonces:
            if (m->estado){
                // o el mensaje es de un vuelo que se esta regresando.
                // en dicho caso, anadimos los pasajeros al pais de origen
                // y pasamos al siguiente mensaje, descartando el presente.
                add_passengers(m,region);
                continue;
            } else {
                // o el mensaje es de un vuelo que tenemos que regresar.
                // en dicho caso, le quitamos la poblacion a los paises.
                return_passengers(m,region);
                // cambiamos el estado del vuelo de regreso a ida.
                m->estado = ! m->estado;
                intercambiarOrigenDestino(m);
                // y lo encolamos al buzon de salida.
                region->buzonSalida = cons(m,region->buzonSalida);
                continue;
            }
        } else if (m->TTL == TIEMPO_VUELO && m->estado){
            
            // Si no hay paises abiertos en la region, entonces 
            // toca devolver el vuelo :(
            if (count_opened_countries(m,region) == 0){
                // seteamos el TTL a 0
                m->TTL = 0;
                // intercambiamos el origen y el destino.
                intercambiarOrigenDestino(m);
                // y encolamos el mensaje en el buzon de salida.
                region->buzonSalida = cons(m,region->buzonSalida);
                // cambiamos el estado a un vuelo de regreso.
                m->estado = ! m->estado;
                continue;
            }
            
            // Si el TTL es el tiempo del vuelo Y es un vuelo de ida.
            // anadimos los pasajeros a la region.
            add_passengers(m,region);
            // cambiamos el estado a un vuelo de regreso.
            m->estado = ! m->estado;
        }

        // y lo encolamos en una lista de mensajes pendientes.
        mensajesProcesados = cons(m,mensajesProcesados);
        
    }
    // finalmente, encolamos los mensajes pendientes de nuevo en el buzon de entrada, esperando a que su TTL sea 0
    // eventualmente.
    region->buzonEntrada = mensajesProcesados;
}

/**
 * @brief Un wrapper sobre la funcion `produce_flight_msg` que nos permite llamarla desde un hilo 
 *  
 * @param argsP Una lista la cual posee los mismos parametros que `produce_flight_msg`
 * en el mismo orden.
 * 
 * @remark Los parametros de argsP en orden son:
 *          -# Region* region: un apuntador a la region la cual producira los mensajes.
 *          -# Pais* pais: un apuntador al pais que produce los mensajes.
 *          -# sem_t* mutex: un apuntador al semaforo del buzon de salida de la region.
 *          -# int* mensajesSalida:  un apuntador a un contador que señala cuantos paises han terminado de producir mensajes. 
 * @see produce_flight_msg
 */
void* produce_flight_msg_wrapper(void* argsP){
    Queue* args = (Queue*) argsP;
    Region* region = (Region*) head(args);
    args = tail(args);

    Pais* pais = (Pais*) head(args);
    args = tail(args);
    
    sem_t* mutex = (sem_t*) head(args);
    args = tail(args);
    
    int* mensajesSalida = head(args);
    
    produce_flight_msg(region,pais,mutex,mensajesSalida);

    return EXIT_SUCCESS;
}

/**
 * @brief Representa la fase de creacion de mensajes, crea los hilos y hace las llamadas necesarias para 
 * realizar esta etapa.
 * 
 * @param region Apuntador a la Region que crea los mensajes.
 * @param vectorRegiones Apuntador al vector que representa cuales regiones han terminado esta fase.
 * @param pos Posicion de la region en el vector de regiones.
 * @param buzonSalida Semaforo que controla el acceso al buzon de salida.
 */
void creacion_de_mensajes(Region* region,int* vectorRegiones, int pos, sem_t* buzonSalida){
    
    // Cuenta cuantos paises han terminado de crear los mensajes.
    int paisesFinalizados = 0;
    int totalPaises       = length(region->paises);
    int i = 0;
    Queue* paises = region->paises;
    Queue* args   = emptyQ();
    // Arreglo de hilos, cada hilo representa un pais en el proceso de creacion de mensajes.
    pthread_t* threads = (pthread_t*) malloc(totalPaises * sizeof(pthread_t));
    // Por cada pais:
    while(paises){
        // Creamos su lista de argumentos.
        args = cons(&paisesFinalizados,args);
        args = cons(buzonSalida,args);
        args = cons((Pais*) head(paises), args);
        args = cons(region,args);
        // creamos el hilo correspondiente.
        pthread_create( &threads[i], NULL, produce_flight_msg_wrapper, (void*) args);
        i++;
        // y pasamos al siguiente pais.
        paises = tail(paises);
    }

    // Esperamos que todos los hilos terminen su trabajo.
    for (int j = 0; j < totalPaises; j++){
        pthread_join(threads[j],NULL);
    }

    // y senalizamos que terminamos la tarea.
    vectorRegiones[pos] = 1;
    // not sure if it's necessary???
    //exit(0);
}

/**
 * @brief Un wrapper sobre la funcion `creacion_de_mensajes` que nos permite llamarla desde un hilo 
 * 
 * @param argsP Una lista la cual posee los mismos parametros que `creacion_de_mensajes`
 * en el mismo orden.
 * 
 * @remark Los parametros de argsP en orden son:
 *          -# Region* region
 *          -# int* vectorRegiones
 *          -# int pos
 *          -# sem_t* buzonSalida
 * @see creacion_de_mensajes
 */
void* creacion_de_mensajes_wrapper(void* argsP){

    Queue* args    = (Queue*) argsP;
    Region* region = (Region*) head(args);
    args = tail(args);
    int* vectorRegiones = (int*) head(args);
    args = tail(args);
    int pos = (int) head(args);
    args = tail(args);
    sem_t* buzonSalida = (sem_t*) head(args);
    args = tail(args);

    
    
    creacion_de_mensajes(region,vectorRegiones,pos,buzonSalida);
    
    return EXIT_SUCCESS;
}

/**
 * @brief Un wrapper sobre `consume_flight_msg`  que nos permite llamarla desde un hilo 
 * 
 * @param argsP Una lista la cual posee los mismos parametros que `consume_flight_msg` en el mismo orden, 
 * junto a un puntero a booleano senalando cuando se debe terminar de ejecutar.
 * 
 * @remark Los parametros de argsP en orden son:
 *          -# Mundo* mundo
 *          -# Region* region
 *          -# sem_t* mutexBuzonSalida
 *          -# int* continuarEnviando
 * @see consume_flight_msg
 */
void* envio_buzon_salida_wrapper(void* argsP){
    Queue* args              = (Queue*) argsP;
    Mundo * mundo            = (Mundo*) head(args);
    args = tail(args);
    Region* region           = (Region*) head(args);
    args = tail(args);
    sem_t* mutexBuzonSalida = (sem_t*) head(args);
    args = tail(args);
    int* continuarEnviando = (int*) head(args);
    args = tail(args);

    while(*continuarEnviando){
        consume_flight_msg(mundo, region,mutexBuzonSalida,  &buzonMundo);
    }
    
    return EXIT_SUCCESS;
}

/**
 * @brief Un wrapper sobre `consume_in_inbox` que nos permite llamarla desde un hilo.
 * 
 * @param argsP Una lista la cual posee los mismos parametros que `consume_in_inbox` en el mismo orden.
 * 
 * @remark Los parametros de argsP en orden son:
 * 
 *          -# Region* region
 * @see consume_in_inbox
 */
void* consumir_inbox_wrapper(void* argsP){
    Queue* args = (Queue*) argsP;
    Region* region = (Region*) head(args);
    args = tail(args);
    

    consume_in_inbox(region);
    
    return EXIT_SUCCESS;
}
/**
 * @brief Decrementa el TTL de cada mensaje del buzon de entrada de cada region.
 * 
 * @param mundo 
 */
void decrementarTTL(Mundo* mundo){
    Queue* regiones = mundo->regiones;
    Region* region;
    Queue* mensajes;
    Mensaje* mensaje;
    while(regiones){
        region   = (Region*) head(regiones);
        mensajes =  region->buzonEntrada;
        while(mensajes){
            mensaje = (Mensaje*) head(mensajes);
            mensaje->TTL = MAX(mensaje->TTL - 1,0);
            mensajes = tail(mensajes);
        }
        regiones = tail(regiones);
    }
}

/**
 * @brief Un wrapper sobre `consume_world_inbox` que nos permite llamarla desde un hilo.
 * 
 * @param argsP Una lista la cual posee los mismos parametros que `consume_world_inbox` en el mismo orden.
 * 
 * @remark Los parametros de argsP en orden son:
 * 
 *          -# Region* region
 *          -# int* continuar: un apuntador a un entero que senala hasta cuando se debe continuar despachando mensajes.
 * @see consume_world_inbox
 */
void* consume_world_inbox_wrapper(void* argsP){
    Queue* args = (Queue*) argsP;
    Mundo* mundo = head(argsP);
    args = tail(args);
    int* continuar = (int*) head(args);
    
    while(*continuar){
        consume_world_inbox(mundo,&buzonMundo);
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Funcion auxiliar que revisa si el buzon de salida de una region esta vacio.
 * 
 * @param r Apuntador a region.
 * @return int 1 si esta vacio, 0 de otro modo.
 */
int nullBuzonsalida(void* r){
    Region* region = (Region *) r;
    return region->buzonSalida == NULL;
}

/**
 * @brief Realiza la etapa 1 de la simulacion: intercambio entre paises.
 * 
 * @param mundo Apuntador al mundo.
 */
void etapa1(Mundo* mundo){
    
    // Primero, iniciamos el semafoto del buzon del mundo.
    sem_init(&buzonMundo, 0, 1);
    // Encontramos el numero de regiones.
    int numeroRegiones = length(mundo->regiones);
    // Pedimos espacio para el vector de regiones (que senala cuando una region ha terminado alguna tarea
    // como la creacion de mensajes).
    int *vectorRegiones = (int*) malloc(numeroRegiones * sizeof(int));
    // Un vector de hilos para la creacion de mensajes.
    pthread_t* threadsRegionesCreacionMensaje = (pthread_t*) malloc(numeroRegiones * sizeof(pthread_t));
    // Un vector de hilos para el envio de mensajes desde regiones.
    pthread_t* threadsRegionesEnvioSalida = (pthread_t*) malloc(numeroRegiones * sizeof(pthread_t));
    // Un vector de semaforos para cada buzon de salida de cada region.
    sem_t* buzonesSalida = (sem_t*) malloc(numeroRegiones * sizeof(sem_t));
    // Un hilo para el buzon del mundo.
    pthread_t threadBuzonMundo;
    int j = 0;
    // Inicializamos el vector de regiones en 0.
    for (int i = 0; i<numeroRegiones; i++){
        vectorRegiones[i] = 0;
    }
    
    // Variable que indica si el buzon del mundo puede seguir enviando.
    int continuarEnviando = 1;
    Queue* argsPGenerarMensajes = emptyQ();
    Queue* argsPEnviarMensajes  = emptyQ(); 
    Queue* argsPConsumirMundo   = emptyQ();
    for (int i = 0; i < numeroRegiones; i++){
        sem_init(&buzonesSalida[i], 0, 1);
        // primero, crearemos el hilo para la creacion de mensajes
        argsPGenerarMensajes = emptyQ();
        // metemos la region
        argsPGenerarMensajes = snoc(valueAt(mundo->regiones,i),argsPGenerarMensajes);
        // luego el vector de regiones
        argsPGenerarMensajes = snoc(vectorRegiones,argsPGenerarMensajes);
        // luego la posicion
        argsPGenerarMensajes = snoc((void*) i,argsPGenerarMensajes);
        // finalmente el buzon de salida.
        argsPGenerarMensajes = snoc(&buzonesSalida[i],argsPGenerarMensajes);
        
        
        // y creamos el hilo de creacion de mensajes.
        pthread_create( &threadsRegionesCreacionMensaje[i], NULL, creacion_de_mensajes_wrapper, (void*) argsPGenerarMensajes);
        
        
        // luego crearemos el hilo para despachar el buzon de salida.
        argsPEnviarMensajes  = emptyQ();
        // metemos el mundo.
        argsPEnviarMensajes  = snoc(mundo, argsPEnviarMensajes);
        // la region.
        argsPEnviarMensajes  = snoc(valueAt(mundo->regiones,i), argsPEnviarMensajes);
        //  luego el buzon de salida.
        argsPEnviarMensajes  = snoc(&buzonesSalida[i], argsPEnviarMensajes);
        // y finalmente la variable de control para continuar enviando los mensajes.
        argsPEnviarMensajes  = snoc(&continuarEnviando, argsPEnviarMensajes);
        
        // y creamos el hilo de despacho del buzon de salida.
        pthread_create( &threadsRegionesEnvioSalida[i], NULL, envio_buzon_salida_wrapper, (void*) argsPEnviarMensajes);
        
    }
    
    // Señaliza cuando el buzon del mundo debe parar de despachar.
    int continuarConsumiendo = 1;
    // Anadimos al mundo al buzon.
    argsPConsumirMundo = snoc(mundo, argsPConsumirMundo);
    // Y la variable de control
    argsPConsumirMundo = snoc(&continuarConsumiendo, argsPConsumirMundo);
    // y creamos el hilo.
    pthread_create(&threadBuzonMundo, NULL,consume_world_inbox_wrapper, (void*) argsPConsumirMundo);

    // Hacemos poll al vector de regiones, cada vez que se escribe un 1 en el,
    // significa que una region termino de enviar mensajes.
    while(j<numeroRegiones){
        if (vectorRegiones[j]){
            vectorRegiones[j] = 0;
            j++;
        }
    }
    
    // Una vez que las regiones terminaron de crear los mensajes, esperamos
    // a que terminen de despacharlos.
    while (!all(nullBuzonsalida,mundo->regiones))
        ;
    
    
    // Y luego esperamos a que el mundo termine de despechar sus mensajes.
    while (mundo->buzonMundo != NULL)
        ;
    // Para senalizar que podemos parar de enviar y consumir.
    continuarConsumiendo = 0;
    continuarEnviando = 0;

    
    // Esperamos a que cada region termine de ejecutar sus rutinas.
    for (int i=0; i<numeroRegiones; i++){
        pthread_join(threadsRegionesEnvioSalida[i], NULL);
    }
    // Y ademas esperamos que el buzon del mundo tambien finiquite
    pthread_join(threadBuzonMundo, NULL);
    
    // Ahora, entramos en la fase de actualizacion.
    // Actualizamos el inbox 
    Queue* argsPBuzonEntrada;
    for (int i=0; i<numeroRegiones; i++){
        argsPBuzonEntrada = emptyQ();
        argsPBuzonEntrada = snoc(valueAt(mundo->regiones,i),argsPBuzonEntrada);
        pthread_create( &threadsRegionesEnvioSalida[i], NULL, consumir_inbox_wrapper, (void*) argsPBuzonEntrada );
    }
    
    // Esperamos a que se actualicen todos los inboxes.
    for (int i=0; i<numeroRegiones; i++){
        pthread_join(threadsRegionesEnvioSalida[i], NULL);
    }
    
    // Una vez actualizados, enviamos a la gente de regreso que se necesite enviar.
    continuarConsumiendo = 1;
    continuarEnviando    = 1;

    // hacemos que cada region envie al mundo
    for (int i = 0; i < numeroRegiones; i++){
        argsPEnviarMensajes  = emptyQ();
        argsPEnviarMensajes  = snoc(mundo, argsPEnviarMensajes);
        argsPEnviarMensajes  = snoc(valueAt(mundo->regiones,i), argsPEnviarMensajes);
        argsPEnviarMensajes  = snoc(&buzonesSalida[i], argsPEnviarMensajes);
        argsPEnviarMensajes  = snoc(&continuarEnviando, argsPEnviarMensajes);
        pthread_create( &threadsRegionesEnvioSalida[i], NULL, envio_buzon_salida_wrapper, (void*) argsPEnviarMensajes);
    }
    
    // hacemos que el mundo envie los mensajes
    argsPConsumirMundo = emptyQ();
    argsPConsumirMundo = snoc(mundo, argsPConsumirMundo);
    argsPConsumirMundo = snoc(&continuarConsumiendo, argsPConsumirMundo);
    pthread_create(&threadBuzonMundo, NULL,consume_world_inbox_wrapper, (void*) argsPConsumirMundo);

    // Esperamos a que se terminen de enviar los mensajes.
    while (!all(nullBuzonsalida,mundo->regiones))
        ;
    while (mundo->buzonMundo != NULL)
        ;

    continuarConsumiendo = 0;
    continuarEnviando = 0;
    
    // Actualizamos los inboxes de nuevo.

    for (int i=0; i<numeroRegiones; i++){
        argsPBuzonEntrada = emptyQ();
        argsPBuzonEntrada = snoc(valueAt(mundo->regiones,i),argsPBuzonEntrada);
        pthread_create( &threadsRegionesEnvioSalida[i], NULL, consumir_inbox_wrapper, (void*) argsPBuzonEntrada );
    }

    // Esperamos a que se actualicen todos los inboxes.
    for (int i=0; i<numeroRegiones; i++){
        pthread_join(threadsRegionesEnvioSalida[i], NULL);
    }

    decrementarTTL(mundo);

}
