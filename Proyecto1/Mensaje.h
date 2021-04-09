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
    double altaNuevosInfectados;
    double mediaNuevosInfectados;
    double bajaNuevosInfectados;
    //fecha :: date; // como sea que se modele una fecha
    double totalNuevosInfectados;
    double totalNuevosMuertos;
} MR;

typedef union MI {
    // Mensaje Eventual
    ME eventualidad;
    // Mensaje Reporte
    MR reporteDiario;
}MI;


typedef struct MensajeInformacional {
    int tipo;
    MI mensaje;
}MensajeInformacional;


Mensaje* emptyMessage(char* regionOrigen, char* paisOrigen);

#endif // MENSAJE_H_