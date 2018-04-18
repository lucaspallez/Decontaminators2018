#ifndef SIMULATION_H
#define SIMULATION_H
#include "error.h"
#include "utilitaire.h"
#include "tolerance.h"
#include "robot.h"
#include "particule.h"
#define INITIALISATION			0
#define INIT_TOKEN_POS			25
#define INIT_TOKEN_ALPHA		4
#define INIT_TOKEN_R			5
#define INIT_TOKEN_T			2
#define NBR_COORDONNEES_R		3
#define NBR_COORDONNEES_P		4


int simulation_ouverture_fichier (const char *file_name);
bool simulation_colision_robot_particule();


#endif
