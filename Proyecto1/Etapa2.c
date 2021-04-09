#include "Etapa2.h"
#include "Modelos.h"

struct Region* thread_data;

void* intercambioClase(void *threadarg){
    Region* r = (Region*) threadarg;

    double alta, compradores, cambioMedia;
    double media, mercado, cambioBaja;
    double baja;

    double porcentajeContagiadoCA, porcentajeContagiadoCM, porcentajeContagiadoCB;

    Queue* paises = r->paises;
    Pais* p;
    for (int i = 0; i < length(paises); i++){
        p = (Pais*) valueAt(paises, i);
        

        //Personas en clase alta que se mueven a clase media
        alta = (double)((p->poblacionTotal*p->claseAlta)/100); //Personas en clase alta segun porcentaje(con decimales incluidos)
        compradores = (double)((alta*p->compradores)/100); //Personas en clase alta que van de compras segun el porcentaje (con decimales incluidos)
        cambioMedia = alta - compradores; //Se eliminan las personas que van de compras de la clase alta
        p->claseAlta = (double)((cambioMedia*100) / p->poblacionTotal); //Nuevo porcentaje de clase alta

        porcentajeContagiadoCA = (p->infectadosClase[0] * 100) / alta; //porcentaje de personas contagiadas en la clase alta
        p->infectadosSalen[0] = (long)round((compradores * porcentajeContagiadoCA) / 100); //personas que van de compras contagiadas

        //Personas de clase media que se mueven a clase baja
        media = (double)((p->poblacionTotal*p->claseMedia)/100); //Personas en clase media segun porcentaje(con decimales incluidos)
        media += compradores; //Se suman al total de personas de clase media aquellas que pasaron de clase alta y seran ahora tratados como media
        mercado = (double)((media*p->mercado)/100); //Personas en clase media que van al mercado segun el porcentaje (con decimales incluidos)
        cambioBaja = media - mercado; //Se restan las personas que van al mercado de la clase media
        p->claseMedia = (double)((cambioBaja*100) / p->poblacionTotal); //Nuevo porcentaje de clase media

        porcentajeContagiadoCM = (p->infectadosClase[1] * 100) / media; //porcentaje de personas contagiadas en la clase media
        p->infectadosSalen[1] = (long)round((mercado * porcentajeContagiadoCM) / 100); //personas que van al mercado contagiadas
        
        //Clase baja nuevo porcentaje 
        baja = (double)((p->poblacionTotal*p->claseBaja)/100); //Personas en clase baja segun porcentaje(con decimales incluidos)
        baja += mercado; //Se suman al total de personas de clase baja aquellas que pasaron de clase media y seran ahora tratados como baja
        p->claseBaja = (double)((baja*100) / p->poblacionTotal); //Nuevo porcentaje de clase baja

        porcentajeContagiadoCB = (p->infectadosClase[2] * 100) / baja; //porcentaje de personas contagiadas en la clase baja
        p->infectadosSalen[2] = (long)round((p->domesticos * porcentajeContagiadoCB) / 100); //personas que van al mercado contagiadas
    }
}

/*
int main(void){
    Mundo* mundo = (Mundo*) malloc(sizeof(Mundo));
    readData(mundo);

    pthread_t threads[NUM_THREADS]; //EL NUMERO DE THREADS ES EL NUMERO DE REGIONES
    int *taskids[NUM_THREADS];
    int rc, t, sum;

    for(t=0;t<NUM_THREADS;t++) {
        thread_data = (Region*)valueAt(mundo->regiones, t);
        rc = pthread_create(&threads[t], NULL, intercambioClase, (void *) thread_data);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);

}*/