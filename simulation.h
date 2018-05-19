#ifndef SIMULATION_H
#define SIMULATION_H
#include "error.h"
#include "utilitaire.h"
#include "constantes.h"
#include "robot.h"
#include "particule.h"
#define INITIALISATION			0
#define INIT_TOKEN_POS			25
#define INIT_TOKEN_ALPHA		4
#define INIT_TOKEN_R			5
#define INIT_TOKEN_T			2
#define DEBUT					0
#define FIN						1
#define ROBOT					0
#define PARTICULE				1
#define ACTIVATION				1
#define DESACTIVATION			0
#define TEMPS_MAX				227


int simulation_ouverture_fichier (const char *file_name);
bool simulation_colision_robot_particule();
int simulation_get_nb_robots();
int simulation_get_nb_particules();
STR_ROBOT** simulation_get_robots();
STR_PARTICULE** simulation_get_particules();
void simulation_deplacement(S2D rob, int i);
void simulation_boucle(double translat , double rotat);
int simulation_robot_colision_boucle(int i, bool rp, bool etat);
void simulation_alignement(S2D rob,int i);
STR_ROBOT** simulation_manuel (double translat , double rotat);


typedef struct Temps TEMPS;
struct Temps
{
	int indice;
	double t;
};
#endif
