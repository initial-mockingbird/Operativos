#Los comentarios se denotan con numeral, y ocupan una linea completa
#Generalmente un comentario inicial explica cual(es) programa(s) compila este archivo
#para ejecutar, llame al comando "make" de UNIX
#El archivo debe llamarse "Makefile" (sin extensiones)



#Suele comenzarse con los siguientes DEFINEs, los cuales definen nombres de variables
#Esto permite cambiar cualquiera de ellas facilmente
#Una variable se incluye con $(NombreDeVariable)
#Los nombres son en mayusculas por convencion

#Cambiar el compilador es comun
COMPILAR = cc -c

#A veces se especifica un comando separado para el linkeo
#Es buena practica definirlo para que otro tenga la opcion de cambiarlo
LINKEAR  = cc

#Suelen colocarse los archivos intermedios en una carpeta
#Se indenta dentro usando espacios
#el codigo se considera elegante si los = estan alineados
#No se puede indentar al principio de la linea
OBJDIR   = archivosO

#tambien suele hacerse un define con todos los archivos intermedios que se pueden crear
#cuando hay varios archivos, debe ir separados por espacios en la misma linea
#Pueden incluirse variables anteriores en las definiciones de otras variables
OBJETOS  = $(OBJDIR)/Calculo.o $(OBJDIR)/Etapa1.o $(OBJDIR)/Etapa2.o $(OBJDIR)/Lectura.o $(OBJDIR)/Mensaje.o $(OBJDIR)/Modelos.o $(OBJDIR)/LinkedList.o $(OBJDIR)/main.o

#El nombre del ejecutable suele darse como un DEFINE tambien
PROGRAMA = modeloEpim

#pueden especificarse parametros con ".PHONY" (archivos de mentira)
#permitir hacer "make all" y "make clean" es muy comun
.PHONY: all clean

#el comando "make all" por convencion crea todos los programas
#En este ejemplo se le dice a make "haz lo necesario para que aparezca el archivo casa.exe"
all: $(PROGRAMA)

#el comando "make clean" por convencion deshace todas las llamadas anteriores a "make"
#en este ejemplo se le indica unos comandos de consola que hacen esto
#es NECESARIO indentar los comandos con tabulador NO con espacios
clean:
	rm $(PROGRAMA)
	rm $(OBJETOS)
	rmdir $(OBJDIR)



#Finalmente, se le dice a "make" como hacer aparecer cada archivo
#Luego de los dos puntos, se indica que archivos deben estar presentes
#"make" no ejecutara los comandos indentados a menos que todos esten presentes
#Puede pasarle cualquiera de estos como parametro a make ej.: "make archivosO/techo.o"
$(PROGRAMA): $(OBJETOS)
	$(LINKEAR) $(OBJETOS) -o $(PROGRAMA) -lm -lpthread

$(OBJDIR)/main.o: main.c $(OBJDIR) ../Utils/LinkedList/LinkedList.h ../Utils/UsefulMacros/macros.h ../Utils/Closures/Closure.h Calculo.h Etapa1.h Etapa2.h Lectura.h Mensaje.h Modelos.h
	$(COMPILAR) main.c -o $(OBJDIR)/main.o

$(OBJDIR)/Calculo.o: Calculo.c $(OBJDIR) ../Utils/LinkedList/LinkedList.h ../Utils/Closures/Closure.h ../Utils/UsefulMacros/macros.h Modelos.h Mensaje.h Calculo.h 
	$(COMPILAR) Calculo.c -o $(OBJDIR)/Calculo.o 

$(OBJDIR)/Etapa1.o: Etapa1.c $(OBJDIR) ../Utils/LinkedList/LinkedList.h ../Utils/Closures/Closure.h ../Utils/UsefulMacros/macros.h Mensaje.h Modelos.h Etapa1.h
	$(COMPILAR) Etapa1.c -o $(OBJDIR)/Etapa1.o 

$(OBJDIR)/Etapa2.o: Etapa2.c $(OBJDIR) Modelos.h Etapa2.h
	$(COMPILAR) Etapa2.c -o $(OBJDIR)/Etapa2.o

$(OBJDIR)/Lectura.o: Lectura.c $(OBJDIR) ../Utils/UsefulMacros/macros.h Mensaje.h Lectura.h
	$(COMPILAR) Lectura.c -o $(OBJDIR)/Lectura.o

$(OBJDIR)/Mensaje.o: Mensaje.c $(OBJDIR) Mensaje.h 
	$(COMPILAR) Mensaje.c -o $(OBJDIR)/Mensaje.o

$(OBJDIR)/Modelos.o: Modelos.c $(OBJDIR) ../Utils/UsefulMacros/macros.h Modelos.h 
	$(COMPILAR) Modelos.c -o $(OBJDIR)/Modelos.o 

$(OBJDIR)/LinkedList.o: ../Utils/LinkedList/LinkedList.c $(OBJDIR) ../Utils/Closures/Closure.h ../Utils/LinkedList/LinkedList.h
	$(COMPILAR) ../Utils/LinkedList/LinkedList.c -o $(OBJDIR)/LinkedList.o

$(OBJDIR):
	mkdir $(OBJDIR)

#siempre debe haber una linea en blanco al final
