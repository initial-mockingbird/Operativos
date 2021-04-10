#ifndef MENSAJE_H_
#define MENSAJE_H_

#include <time.h>

typedef struct Mensaje {
    char* regionOrigen;
    char* regionDestino;
    char* paisOrigen;
    int TTL; // Time To Live
    double contenido[3];
    double infectados[3];
    int estado; // 1 identifica que el mensaje es de ida, aka: se deben anadir los pasajeros a los paises, 0 indica lo contrario: se deben quitar pasajeros a los paises.
}Mensaje;

typedef struct ME {
    char* pais;
    int tipoHito;
    struct tm* fecha;
} ME;

typedef struct MR {
    char* pais;
    double  altaNuevosInfectados;
    double  mediaNuevosInfectados;
    double  bajaNuevosInfectados;
    double  altaNuevosMuertos;
    double  mediaNuevosMuertos;
    double  bajaNuevosMuertos;
    struct tm* fecha; 
    long long  totalNuevosInfectados;
    long long  totalNuevosMuertos;
} MR;

typedef union MI {
    // Mensaje Eventual
    ME *eventualidad;
    // Mensaje Reporte
    MR *reporteDiario;
}MI;


typedef struct MensajeInformacional {
    // 0: ME, 1: MR
    int tipo;
    MI* mensaje;
}MensajeInformacional;


Mensaje* emptyMessage(char* regionOrigen, char* paisOrigen);
double altaNuevosInfectados(MR *MR);
double mediaNuevosInfectados(MR *MR);
double bajaNuevosInfectados(MR *MR);
double altaNuevosMuertos(MR *MR);
double mediaNuevosMuertos(MR *MR);
double bajaNuevosMuertos(MR *MR);

#endif // MENSAJE_H_