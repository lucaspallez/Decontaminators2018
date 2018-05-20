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
#define NBR_COORDONNEES_R		8
#define LIBRE					-1
#define BLUE					2
#define BLACK					0
#define RED					    1
#define DESACTIVATION			0
#define ACTIVATION				1
#define NUL						0
#define TRUTH					1
#define FALSE					0
#define REDUCTEUR				10
#define OUT						21

typedef struct Robot STR_ROBOT;
struct Robot
{
	double pos_x;
	double pos_y;
	double angle;
	S2D occup;
	bool actif;
	int color;
	double vrot;
	double vtran;
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
STR_ROBOT** robot_activation_desactivation(int i, bool etat);
STR_ROBOT ** robot_occupation(double x , double y, int i);
STR_ROBOT** robot_color(int i, int color);
S2D robot_alignement(S2D init, S2D rob, S2D cible, bool rp, double rayon, double angle);
STR_ROBOT** robot_recul(S2D rob, int i);
STR_ROBOT** robot_manuel(double translat , double rotat, int i);
STR_ROBOT** robot_get_robots();

#endif
