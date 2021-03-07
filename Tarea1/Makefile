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
#cuando hay varis archivos, debe ir separados por espacios en la misma linea
#Pueden incluirse variables anteriores en las definiciones de otras variables
OBJETOS  = $(OBJDIR)/techo.o $(OBJDIR)/paredes.o $(OBJDIR)/puerta.o $(OBJDIR)/ventana.o $(OBJDIR)/piso.o $(OBJDIR)/escaleras.o $(OBJDIR)/main.o

#El nombre del ejecutable suele darse como un DEFINE tambien
PROGRAMA = casa.exe



#pueden especificarse parametros con ".PHONY" (archivos de mentira)
#perimitr hacer "make all" y "make clean" es muy comun
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
	$(LINKEAR) $(OBJETOS) -o $(PROGRAMA)

$(OBJDIR)/main.o: main.c $(OBJDIR) techo.h paredes.h piso.h escaleras.h
	$(COMPILAR) main.c -o $(OBJDIR)/main.o

$(OBJDIR)/techo.o: techo.c $(OBJDIR) techo.h paredes.h
	$(COMPILAR) techo.c -o $(OBJDIR)/techo.o

$(OBJDIR)/paredes.o: paredes.c $(OBJDIR) paredes.h puerta.h ventana.h
	$(COMPILAR) paredes.c -o $(OBJDIR)/paredes.o

$(OBJDIR)/puerta.o: puerta.c $(OBJDIR) puerta.h
	$(COMPILAR) puerta.c -o $(OBJDIR)/puerta.o

$(OBJDIR)/ventana.o: ventana.c $(OBJDIR) ventana.h
	$(COMPILAR) ventana.c -o $(OBJDIR)/ventana.o

$(OBJDIR)/piso.o: piso.c $(OBJDIR) piso.h
	$(COMPILAR) piso.c -o $(OBJDIR)/piso.o

$(OBJDIR)/escaleras.o: escaleras.c $(OBJDIR) escaleras.h piso.h
	$(COMPILAR) escaleras.c -o $(OBJDIR)/escaleras.o

$(OBJDIR):
	mkdir $(OBJDIR)

#siempre debe haber una linea en blanco al final
