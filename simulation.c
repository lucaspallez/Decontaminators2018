#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "utilitaire.h"
#include "simulation.h"
#include "constantes.h"
#include "robot.h"
#include "particule.h"


static double * pointer_r;
static double * pointer_p;
static double nb_robots, nb_particules;
static STR_PARTICULE** particule;
static STR_ROBOT ** robot;

int simulation_ouverture_fichier (const char *file_name)
{
	pointer_r = robot_lecture_fichier(file_name);
	robot = robot_donnees();
	if (pointer_r == NULL)
	{
		robot_free_robots();
		free(pointer_r);
		return EXIT_FAILURE;
	}
	 	
	pointer_p = particule_lecture_fichier(file_name);
	particule = particule_donnees();
	if (pointer_p == NULL)
	{
		particule_free_particules();
		robot_free_robots();
		free(pointer_p);
		free(pointer_r);
		return EXIT_FAILURE;
	}
	nb_robots = robot_nombre_robots();
	nb_particules = particule_nombre_particules();
		
	if (simulation_colision_robot_particule() == 0)
	{
		robot_free_robots();
		particule_free_particules();
		free(pointer_p);
		free(pointer_r);
		return EXIT_FAILURE;
	}
	error_no_error_in_this_file();
	free(pointer_p);
	free(pointer_r);
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
	
			if( sqrt( dist_x*dist_x + dist_y*dist_y) < r1+R_ROBOT)
			{
				error_collision(ROBOT_PARTICULE, j+1 , k+1);
				return 0;
			}
		}
	}
	return 1;
}

void simulation_boucle()
{	
	double*delta_gamma=NULL;
	double ecart_angle;
	delta_gamma = &ecart_angle;
	double temps_rot, temps_tran;
	int taille = nb_robots;
	double temps_tot[taille];
	TEMPS token = {0,-1};
	int q = nb_particules;
	double vtran;
	for(int i = 0; i < q ; i++)
	{
		if (((double) rand()/RAND_MAX) <= DECOMPOSITION_RATE)
		{
			particule = particule_decomposition(i);
			nb_particules = particule_nombre_particules();
		}
		
		
		//nb_particules = particule_reallocation(0);
	}
	particule = particule_tri();
	//~ printf("%lf" , nb_particules);
	//~ for (int i = 0 ; i < nb_particules ; i++)
	//~ {
		//~ printf("%lf \n", particule[i]->rayon);
	//~ }
	
	
	
	for (int i=0 ; i < nb_robots ; i++)
	{
		S2D rob = {0,0};
		S2D part= {0,0};
		rob.x = robot[i]->pos_x;
		rob.y = robot[i]->pos_y;
		
		int k = simulation_robot_colision_boucle(i); 
		//~ printf("%d \n" , k);
		if ( k != LIBRE)
		{
			robot = robot_activation_desactivation(i, 1);
			part.x = particule[k]->pos_x;
			part.y = particule[k]->pos_y;
			robot_occupation(part.x , part.y , i);
			util_ecart_angle(rob,robot[i]->angle,part,delta_gamma);

			
			if(fabs(*delta_gamma) > EPSIL_ZERO)
			{
				//~ printf("%lf \n" , *delta_gamma);
				robot = robot_vrot(i,delta_gamma);
				
			}
			else
			{
				robot = robot_activation_desactivation(i, 0);
				particule = particule_desintegration(k);
				//~ nb_particules = particule_reallocation(0);
			}
		}
		else
		{
			if (robot[i]->actif)
			{
				if(rob.x - robot[i]->occup.x < EPSIL_ZERO && rob.y - robot[i]->occup.y < EPSIL_ZERO)
					robot = robot_activation_desactivation(i,0);
				else
					robot = robot_deplacement(rob, i);
			}
		}
	}
	// EDITION DE BUT
	for ( int i = 0 ; i< nb_particules ; i++)
	{
		if(particule[i]->rayon > 0)
		{
			S2D part = {particule[i]->pos_x,particule[i]->pos_y};
			S2D rob = {0,0};
			double L;
			for (int k=0 ; k < nb_robots ; k++)
			{
				//~ printf("k= %d \n" , k);
				rob.x = robot[k]->pos_x;
				rob.y = robot[k]->pos_y;
				L = util_distance(part,rob);
				if(util_ecart_angle(rob,robot[k]->angle,part,delta_gamma))
				{
					temps_rot = robot_temps_rot_calcul(delta_gamma);
					vtran = robot_vtran(L);
					temps_tran= L / vtran ;
					temps_tot[k] = temps_tran+temps_rot;
					//~ printf ("%lf \n" , temps_tot[k]);
				}
			}
				token.t = 10000000000000;
				for(int k = 0 ; k < nb_robots; k++)
				{
					if (!robot[k]->actif)
					{
						if (temps_tot[k] < token.t)
						{
							token.t = temps_tot[k];
							token.indice = k;
						}
					}
				}
			if (!robot[token.indice]->actif)
			{
				robot = robot_activation_desactivation(token.indice, 1);
				robot = robot_occupation(part.x , part.y, token.indice);
				rob.x = robot[token.indice]->pos_x;
				rob.y = robot[token.indice]->pos_y;
				robot = robot_deplacement(rob,token.indice);
			}
				
			
		}
	}
}

int simulation_robot_colision_boucle(int i)
{
	double x1, x2, y1, y2, r1;
 	double dist_x ,dist_y ;
		for (int k = 0 ; k < nb_particules ; k++)
		{
			if(particule[k]->rayon != 0)
			{
				x1 = robot[i]->pos_x;
				y1 = robot[i]->pos_y;
				r1 = particule[k]->rayon;
				x2 = particule[k]->pos_x;
				y2 = particule[k]->pos_y;
				
				dist_x = fabs(x2-x1);
				dist_y = fabs(y2-y1);
		
				if( sqrt( dist_x*dist_x + dist_y*dist_y) <= r1+R_ROBOT)
				{
					return k;
				}
			}
		}
	return LIBRE;
}

int simulation_get_nb_robots()
{
	return nb_robots;
}
int simulation_get_nb_particules()
{
	return nb_particules;
}
STR_ROBOT **simulation_get_robots()
{
	return robot;
}
STR_PARTICULE **simulation_get_particules()
{
	return particule;
}

