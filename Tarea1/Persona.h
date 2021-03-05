#ifndef PERSONA_H_
#define PERSONA_H_

typedef struct Persona Persona;
extern Queue* personas;
char* nombre(Persona* p);
char* relacion(Persona* p);
char* origen(Persona* p);
int origenModo(Persona* p);
Persona* lookupByName(char* nombre);
Persona* lookupByRelacion(char* relacion);
void updatePersona(char* name, char* relacion, char* origen, int origenModo);
void parentezco(Queue* input, char** relacion);
void updateNombre(Persona** p);
void updateOrigen(Persona** p);
void discoveryP(Persona** p);
int discovery(char* s);
void freudiano(char* s);
int isComplete(Persona* p);

#endif // PERSONA_H_
