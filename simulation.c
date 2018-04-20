#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "utilitaire.h"
#include "simulation.h"
#include "tolerance.h"
#include "constantes.h"
#include "robot.h"
#include "particule.h"


static double * pointer_r;
static double * pointer_p;
static double nb_robots, nb_particules;

int simulation_ouverture_fichier (const char *file_name)
{
	
	pointer_r = robot_lecture_fichier(file_name);
	if (pointer_r == NULL)
	{
		free(pointer_r);
		return EXIT_FAILURE;
		
	}
	 	
	pointer_p = particule_lecture_fichier(file_name);
	if (pointer_p == NULL)
	{
		free(pointer_p);
		free(pointer_r);
		return EXIT_FAILURE;
	}
	nb_robots = robot_nombre_robots();
	nb_particules = particule_nombre_particules();
		
	if (simulation_colision_robot_particule() == 0)
	{
		free(pointer_p);
		free(pointer_r);
		return EXIT_FAILURE;
	}
	error_no_error_in_this_file();
	
	return EXIT_SUCCESS;
}

bool simulation_colision_robot_particule()
{
 	double x1, x2, y1, y2, r1;
 	double dist_x ,dist_y ;

	for (int j = 0 ; j < nb_robots ; j++)
	{
		for (int k = 0 ; k < nb_particules ; k++)
		{
		
			x1 = *(pointer_r + (j * NBR_COORDONNEES_R));
			y1 = *(pointer_r + (j * NBR_COORDONNEES_R) + 1);
			r1 = *(pointer_p + (NBR_COORDONNEES_P*k) + 1);
			x2 = *(pointer_p + (NBR_COORDONNEES_P*k) + 2);
			y2 = *(pointer_p + (NBR_COORDONNEES_P*k) + 3);
			
			dist_x = fabs(x2-x1);
			dist_y = fabs(y2-y1);
	
			if ( sqrt( dist_x*dist_x + dist_y*dist_y) < r1+R_ROBOT)
			{
				error_collision(ROBOT_PARTICULE, j+1 , k+1);
				return 0;
			}
		}
	}
	return 1;
}

int simulation_get_nb_robots()
{
	return nb_robots;
}
int simulation_get_nb_particules()
{
	return nb_particules;
}
double *simulation_get_robots()
{
	return pointer_r;
}
double *simulation_get_particules()
{
	return pointer_p;
}
