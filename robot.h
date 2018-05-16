#ifndef ROBOT_H
#define ROBOT_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "utilitaire.h"
#include "constantes.h"
#define INITIALISATION			0
#define INIT_TOKEN_POS			25
#define INIT_TOKEN_ALPHA		4
#define NBR_COORDONNEES_R		5
#define LIBRE					-1


typedef struct Robot STR_ROBOT;
struct Robot
{
	double pos_x;
	double pos_y;
	double angle;
	S2D occup;
	bool actif;
};



double * robot_lecture_fichier(const char *fichier);
double robot_nombre_robots();
int robot_avancement(int k, char *tab);
bool robot_collision();
STR_ROBOT** robot_donnees();
void robot_free_robots();
STR_ROBOT ** robot_vrot(int i, double*angle);
double robot_temps_rot_calcul(double*angle);
double robot_vtran(double L);
STR_ROBOT** robot_deplacement(S2D rob, int i);
STR_ROBOT** robot_activation_desactivation(int i, bool a);
STR_ROBOT ** robot_occupation(double x , double y, int i);




#endif
