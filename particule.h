#ifndef PARTICULE_H
#define PARTICULE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "utilitaire.h"
#include "constantes.h"
#define INITIALISATION			0
#define NBR_COORDONNEES_P		5
#define DECOMPOSEE				0
#define CIBLEE					1
#define NON_CIBLEE				0
#define SUPERIEURE				1
#define INFERIEURE				0
#define NB_POSITION				4


typedef struct Particule STR_PARTICULE;

struct Particule
{
	double energie;
	double rayon;
	double pos_x;
	double pos_y;
	bool ciblee;
};

double *particule_lecture_fichier(const char *fichier);
double particule_nombre_particules();
int particule_avancement(int k, char*tab);
bool particule_collision();
STR_PARTICULE** particule_donnees();
STR_PARTICULE** particule_decomposition(int i);
void particule_free_particules();
STR_PARTICULE** particule_tri();
double particule_reallocation(int k ,bool a);
STR_PARTICULE** particule_desintegration(int i);
STR_PARTICULE** particule_get_particules();
STR_PARTICULE** particule_ciblage_deciblage(int i , bool etat);

#endif
