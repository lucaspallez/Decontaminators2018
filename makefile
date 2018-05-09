# Makefile

build: projet.x

graphic.o : graphic.c graphic.h
	gcc -Wall -lGL -lglut -c graphic.c -o graphic.o

utilitaire.o : utilitaire.c utilitaire.h graphic.h tolerance.h
	gcc -Wall -lm -c utilitaire.c -o utilitaire.o

error.o : error.c error.h
	gcc -Wall -c error.c -o error.o

particule.o : particule.c particule.h utilitaire.h error.h
	gcc -Wall -c particule.c -o particule.o

robot.o : robot.c robot.h utilitaire.h error.h
	gcc -Wall -c robot.c -o robot.o

simulation.o : simulation.c simulation.h utilitaire.h particule.h robot.h error.h
	gcc -Wall -c simulation.c -o simulation.o

projet.o : projet.cpp simulation.h
	gcc -Wall -lglui -c projet.cpp -o projet.o

projet.x : graphic.o utilitaire.o error.o particule.o robot.o simulation.o projet.o
	gcc projet.o simulation.o robot.o particule.o error.o utilitaire.o graphic.o -lm -lGL -lglut -lglui -o projet.x

clean:
	@echo "Cleaning .o files"
	@/bin/rm -f *.o
