#ifndef PARTICULE_H
#define PARTICULE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "utilitaire.h"
#include "tolerance.h"
#include "constantes.h"
#define INITIALISATION			0
#define INIT_TOKEN_POS			25
#define INIT_TOKEN_R			5
#define INIT_TOKEN_T			2
#define NBR_COORDONNEES_P		4



typedef struct Particule STR_PARTICULE;

struct Particule
{
	double energie;
	double rayon;
	double pos_x;
	double pos_y;
};

double *particule_lecture_fichier(const char *fichier);
double particule_nombre_particules();

#endif