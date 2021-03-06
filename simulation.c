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
		
	if (simulation_collision_robot_particule_initiale() == 0)
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
bool simulation_collision_robot_particule_initiale()
{
 	double x1, x2, y1, y2, r1; //TEST ERROR COLLISION INITIALE
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

void simulation_boucle(double translat, double rotat)
{	
	double*delta_gamma=NULL;
	double ecart_angle;
	delta_gamma = &ecart_angle;
	int temp = nb_particules;

	for(int i = 0; i < temp ; i++)  //DECOMPOSITION DES PARTICULES
	{
		if (((double) rand()/RAND_MAX) <= DECOMPOSITION_RATE)
		{
			if(particule[i])
			{
				particule = particule_decomposition(i);
				nb_particules = particule_nombre_particules();
			}
		}
	}
	particule = particule_tri();
	nb_particules = particule_nombre_particules();
	simulation_edition_but();   //EDITION DU BUT DES ROBOTS
	
	for (int i=0 ; i < nb_robots ; i++)
	{
		S2D rob;
		rob.x = robot[i]->pos_x;
		rob.y = robot[i]->pos_y;
		robot[i]->vrot=INITIALISATION;
		robot[i]->vtran=INITIALISATION;
		int k = simulation_robot_collision_boucle(i, PARTICULE, DEBUT);
		int j = simulation_robot_collision_boucle(i, ROBOT ,DEBUT);
		
		if ( k != LIBRE)    //COLLISION AVEC UNE PARTICULE
		{
			simulation_boucle_collision_particule(i , k ,rob , delta_gamma);
		}
		else if (j == LIBRE) //PAS DE COLLISION
		{
			simulation_boucle_robot_libre (i,rob);
		}
		else   //COLLISION AVEC SEULEMENT DES ROBOTS
		{
			simulation_boucle_collision_robot(i,j,delta_gamma,rob);
		}
	}
	simulation_manuel(translat,rotat); //MODE MANUEL
}

int simulation_robot_collision_boucle(int i, bool rp, bool etat)
{
	double x1, x2, y1, y2, r1;
 	double dist_x ,dist_y ;
 	if (rp)
 	{
		for (int k = 0 ; k < nb_particules ; k++)
			if(particule[k])
			{
				x1 = robot[i]->pos_x;
				y1 = robot[i]->pos_y;
				r1 = particule[k]->rayon;
				x2 = particule[k]->pos_x;
				y2 = particule[k]->pos_y;
				dist_x = fabs(x2-x1);
				dist_y = fabs(y2-y1);
				if (etat == FIN) //CONDITION D ALIGNEMENT
				{
					if(sqrt( dist_x*dist_x + dist_y*dist_y) < r1+R_ROBOT)
						return k;
				}
				else //CONDITION DE COLLISION SIMPLE
					if(sqrt(dist_x*dist_x+ dist_y*dist_y)<=r1+R_ROBOT+2*EPSIL_ZERO)
						return k;
			}
	}
	else
	{
		for (int k = 0 ; k < nb_robots ; k++)
			if (k != i)
			{
				x1 = robot[i]->pos_x;
				y1 = robot[i]->pos_y;
				x2 = robot[k]->pos_x;
				y2 = robot[k]->pos_y;
				dist_x = x2-x1;
				dist_y = y2-y1;
				if(etat == FIN) //CONDITION D ALIGNEMENT
				{
					if( sqrt( dist_x*dist_x + dist_y*dist_y) < 2*R_ROBOT)
						return k;
				}
				else //CONDITION DE COLLISION SIMPLE
					if(sqrt(dist_x*dist_x + dist_y*dist_y)<= 2*R_ROBOT+EPSIL_ZERO)
						return k;
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

void simulation_alignement(S2D rob,int i)
{
	robot = robot_get_robots();
	int j,k;
	k = simulation_robot_collision_boucle(i, PARTICULE , FIN);
	j = simulation_robot_collision_boucle(i, ROBOT, FIN);
	
	while (k != LIBRE || j!= LIBRE) //ALIGNEMENT TANT QUE NON LIBRE
	{
		j = simulation_robot_collision_boucle(i, ROBOT, FIN);
		if (j!=LIBRE)
		{
			S2D token = {robot[i]->pos_x, robot[i]->pos_y};
			S2D cible = {robot[j]->pos_x, robot[j]->pos_y};
			S2D init;
			init = robot_alignement(rob,token,cible,ROBOT,R_ROBOT,robot[i]->angle);
			robot = robot_recul(init, i);
		}
		k = simulation_robot_collision_boucle(i, PARTICULE, FIN);
		if (k!=LIBRE)
		{
			S2D token = {robot[i]->pos_x, robot[i]->pos_y};
			S2D cible = {particule[k]->pos_x, particule[k]->pos_y};
			S2D init;
			init = robot_alignement
				   (rob,token,cible,PARTICULE,particule[k]->rayon,robot[i]->angle);
			robot = robot_recul(init, i);
		}
	}
}

STR_ROBOT** simulation_manuel (double translat , double rotat)
{
	for (int i = 0; i< nb_robots ; i++) //MODE MANUEL APPEL DE ROBOT_MANUEL
	{
		if (robot[i]->color == RED)
		{
			S2D init = {robot[i]->pos_x,robot[i]->pos_y};
			robot = robot_activation_desactivation(i , DESACTIVATION);
			robot = robot_occupation(init.x , init.y , i);
			robot = robot_manuel(translat, rotat , i);
			simulation_alignement(init, i);
		}
	}
	return robot;
}

void simulation_edition_but()
{
	double temps_rot, temps_tran,ecart_angle,L;
	double*delta_gamma = &ecart_angle;
	double temps_tot[(int) nb_robots];
	TEMPS token = {INITIALISATION,TEMPS_MAX};
	bool index;
	for ( int i = 0 ; i< fmin(nb_particules,nb_robots) ; i++)
	{
		token.indice = LIBRE;    //CALCUL DES TEMPS RELATIFS AUX ROBOTS
		if(particule[i])
			if(particule[i]->rayon > DECOMPOSEE)
			{
				S2D part = {particule[i]->pos_x,particule[i]->pos_y};
				S2D rob = {INITIALISATION,INITIALISATION};
				for (int k=0 ; k < nb_robots ; k++)
				{
					rob.x = robot[k]->pos_x;
					rob.y = robot[k]->pos_y;
					L = util_distance(part,rob);
					if(util_ecart_angle(rob,robot[k]->angle,part,delta_gamma))
					{
						temps_rot = robot_temps_rot_calcul(delta_gamma);
						temps_tran= L / (robot_vtran(L)) ;
						temps_tot[k] = temps_tran+temps_rot;
					}
				}
				token.t = TEMPS_MAX;
				index = DESACTIVATION;
				for (int k = 0 ; k < nb_robots; k++)
				{
					if (particule[i]->pos_x==robot[k]->occup.x
						&& particule[i]->pos_y==robot[k]->occup.y)
						index = ACTIVATION;
				}
				if (!index)
				{
					for(int k = 0 ; k < nb_robots; k++)
						if (!robot[k]->actif && robot[k]->color != RED)
							if (temps_tot[k] < token.t)
							{
								token.t = temps_tot[k];
								token.indice = k;
							}
					if (token.indice != LIBRE)   //AFFECTATION PARTICULE_ROBOT
					{
						particule = particule_ciblage_deciblage(i , ACTIVATION);
						robot=robot_activation_desactivation
							  (token.indice,ACTIVATION);
						robot = robot_occupation(part.x,part.y,token.indice);
						rob.x = robot[token.indice]->pos_x;
						rob.y = robot[token.indice]->pos_y;
					}		
				}			
			}
	}
}

void simulation_boucle_collision_particule(int i,int k,S2D rob,double *delta_gamma)
{
	S2D part;   //SI COLLISION AVEC UNE PARTICULE
	robot = robot_activation_desactivation(i, ACTIVATION);
	particule = particule_ciblage_deciblage(k , ACTIVATION);
	part.x = particule[k]->pos_x; 
	part.y = particule[k]->pos_y;
	robot_occupation(part.x , part.y , i);
	util_ecart_angle(rob,robot[i]->angle,part,delta_gamma);
	if(fabs(*delta_gamma) > EPSIL_ALIGNEMENT)   //SOIT ROTATION DU ROBOT
	{
		if (robot[i]->color != RED)
			robot = robot_vrot(i,delta_gamma);
	}
	else //SOIT DESTRUCTION DE LA PARTICULE
	{
		robot = robot_activation_desactivation(i, DESACTIVATION);
		particule = particule_desintegration(k);
		if (robot[i]->color != RED)
			robot = robot_color(i , BLUE);
	}
}

void simulation_boucle_robot_libre (int i, S2D rob)
{
	if (robot[i]->actif)  //SI AUCUNE COLLISION DETECTEE
	{
		S2D part = {robot[i]->occup.x , robot[i]->occup.y};
		double distance = util_distance(rob , part);
		if(distance < R_PARTICULE_MIN) //LE ROBOT A SON BUT SE DESACTIVE
		{
			robot = robot_activation_desactivation(i,DESACTIVATION);
		}
		else
		{
			if(robot[i]->color != RED) //LE ROBOT SE DEPLACE
			{
				robot = robot_deplacement(rob, i);
				simulation_alignement(rob,i);
			}
		}
	}
}

void simulation_boucle_collision_robot(int i,int j, double* delta_gamma, S2D rob)
{
	S2D cible = {robot[j]->pos_x,robot[j]->pos_y}; //SI COLLISION ROBOT
	if (util_ecart_angle(rob, robot[i]->angle, cible , delta_gamma))
	{
		if (fabs(*delta_gamma) < (M_PI/2)) //TANT QUE ALIGNE TOURNE
		{
			if(robot[i]->color != RED)
				robot_manuel(NUL,VROT_MAX,i);
		}
		else //SI NON ALIGNE DEPLACEMENT EN LIGNE DROITE
		{
			if(robot[i]->color != RED)
			{
				robot_manuel(VTRAN_MAX,NUL,i);
				simulation_alignement(rob, i);
			}
		}
	}
}

