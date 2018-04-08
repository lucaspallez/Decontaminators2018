#ifndef SIMULATION_H
#define SIMULATION_H
#include "error.h"
#include "utilitaire.h"
#include "tolerance.h"
#define INITIALISATION			0
#define INIT_TOKEN_POS			25
#define INIT_TOKEN_ALPHA		4
#define INIT_TOKEN_R			5
#define INIT_TOKEN_T			2
#define NBR_COORDONNEES_R		3
#define NBR_COORDONNEES_P		4

typedef struct Robot STR_ROBOT;
struct Robot
{
	double pos_x;
	double pos_y;
	double angle;
};

typedef struct Particule STR_PARTICULE;

struct Particule
{
	double energie;
	double rayon;
	double pos_x;
	double pos_y;
};

int ouverture_fichier (const char *file_name);



#endif
