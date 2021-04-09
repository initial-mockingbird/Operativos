#ifndef MENSAJE_H_
#define MENSAJE_H_
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
} ME;

typedef struct MR {
    char* pais;
    long long altaNuevosInfectados;
    long long  mediaNuevosInfectados;
    long long  bajaNuevosInfectados;
    long long  altaNuevosMuertos;
    long long  mediaNuevosMuertos;
    long long  bajaNuevosMuertos;
    //fecha :: date; // como sea que se modele una fecha
    long long  totalNuevosInfectados;
    long long  totalNuevosMuertos;
} MR;

typedef union MI {
    // Mensaje Eventual
    ME* eventualidad;
    // Mensaje Reporte
    MR* reporteDiario;
}MI;


typedef struct MensajeInformacional {
    int tipo;
    MI* mensaje;
}MensajeInformacional;


Mensaje* emptyMessage(char* regionOrigen, char* paisOrigen);
long long altaNuevosInfectados(MR *MR);
long long mediaNuevosInfectados(MR *MR);
long long bajaNuevosInfectados(MR *MR);
long long altaNuevosMuertos(MR *MR);
long long mediaNuevosMuertos(MR *MR);
long long bajaNuevosMuertos(MR *MR);

#endif // MENSAJE_H_