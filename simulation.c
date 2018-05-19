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

void simulation_boucle(double translat, double rotat)
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
	bool index;
	for(int i = 0; i < q ; i++)
	{
		if (((double) rand()/RAND_MAX) <= DECOMPOSITION_RATE)
		{
			if(particule[i])
			{
				particule = particule_decomposition(i);
				nb_particules = particule_nombre_particules();
			}

		}
		
		
		//nb_particules = particule_reallocation(0);
	}
	particule = particule_tri();
	nb_particules = particule_nombre_particules();
	//~ printf("%lf" , nb_particules);
	//~ printf("bouh \n");
	


// EDITION DE BUT
	for ( int i = 0 ; i< nb_particules ; i++)
	{
		token.indice = LIBRE;
			if(particule[i])
			{
					if(particule[i]->rayon > 0)
					{
						S2D part = {particule[i]->pos_x,particule[i]->pos_y};
						S2D rob = {0,0};
						double L;
						for (int k=0 ; k < nb_robots ; k++)
						{
							rob.x = robot[k]->pos_x;
							rob.y = robot[k]->pos_y;
							L = util_distance(part,rob);
							if(util_ecart_angle(rob,robot[k]->angle,part,delta_gamma))
							{
								temps_rot = robot_temps_rot_calcul(delta_gamma);
								vtran = robot_vtran(L);
								temps_tran= L / vtran ;
								temps_tot[k] = temps_tran+temps_rot;
							}
						}
						token.t = TEMPS_MAX;
						index = 0;
						for (int k = 0 ; k < nb_robots; k++)
						{
							if (particule[i]->pos_x == robot[k]->occup.x && particule[i]->pos_y == robot[k]->occup.y)
								index = 1;
						}
						
						if (!index)
						{
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
							if (token.indice != LIBRE)
							{
									particule = particule_ciblage_deciblage(i , ACTIVATION);
									printf(" %d rayon particule = %lf \n", token.indice ,particule[i]->rayon);
									robot = robot_activation_desactivation(token.indice, ACTIVATION);
									robot = robot_occupation(part.x , part.y, token.indice);
									rob.x = robot[token.indice]->pos_x;
									rob.y = robot[token.indice]->pos_y;
							}		
						}			
					
				}
			}
		
	}




	for (int i=0 ; i < nb_robots ; i++)
	{
		S2D rob = {0,0};
		S2D part= {0,0};
		rob.x = robot[i]->pos_x;
		rob.y = robot[i]->pos_y;
		robot[i]->vrot=0;
		robot[i]->vtran=0;
		int k = simulation_robot_colision_boucle(i, PARTICULE, DEBUT);
		int j = simulation_robot_colision_boucle(i, ROBOT ,DEBUT);
		
		if ( k != LIBRE)
		{
			robot = robot_activation_desactivation(i, ACTIVATION);
			particule = particule_ciblage_deciblage(k , ACTIVATION);
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
				robot = robot_activation_desactivation(i, DESACTIVATION);
				particule = particule_desintegration(k);
				if (robot[i]->color != RED)
					robot = robot_color(i , BLUE);
			}
		}
		else if (j == LIBRE)
		{
			if (robot[i]->actif)
			{
				S2D part = {robot[i]->occup.x , robot[i]->occup.y};
				double distance = util_distance(rob , part);
				if(distance < R_PARTICULE_MIN)
				{
					robot = robot_activation_desactivation(i,DESACTIVATION);
				}
				else
				{
					//~ printf ("deplacement \n");
					robot = robot_deplacement(rob, i);
					simulation_alignement(rob,i);
				}
			}
		}
		else
		{
			S2D cible = {robot[j]->pos_x,robot[j]->pos_y};
			if (util_ecart_angle(rob, robot[i]->angle, cible , delta_gamma))
			{
				if (fabs(*delta_gamma) < (M_PI/2))
				{
					robot_manuel(0,VROT_MAX,i);
				}
				else
				{
					robot_manuel(0.5,0,i);
					simulation_alignement(rob, i);
				}
			}
		}
	}
	
	//~ printf ("---- \n");
	for( int i =0 ; i< nb_robots ; i++)
	{
		//~ printf("%d \n" , i );
		//~ printf("but x = %lf \n" , robot[i]->occup.x);
		//~ printf("but y = %lf \n" , robot[i]->occup.y);
		
		
		
		//~ int j = simulation_robot_colision_boucle(i,ROBOT,FIN);
		//~ int k = simulation_robot_colision_boucle(i,PARTICULE,DEBUT);
		//~ if (j != LIBRE && k == LIBRE)
		//~ {	
			//~ S2D rob = {robot[i]->pos_x, robot[i]->pos_y};
			//~ robot_manuel(0,0,i);
			//~ simulation_alignement(rob, i);
		//~ }
	}
	simulation_manuel(translat,rotat);
	
	for (int i = 0 ; i < nb_particules; i++)
	{
		if(particule[i])
			printf("%lf  %d \n", particule[i]->rayon, particule[i]->ciblee);
	}
}

int simulation_robot_colision_boucle(int i, bool rp, bool etat)
{
	double x1, x2, y1, y2, r1;
 	double dist_x ,dist_y ;
 	if (rp)
 	{
		for (int k = 0 ; k < nb_particules ; k++)
		{
			if(particule[k])
			{
				x1 = robot[i]->pos_x;
				y1 = robot[i]->pos_y;
				r1 = particule[k]->rayon;
				x2 = particule[k]->pos_x;
				y2 = particule[k]->pos_y;
				
				dist_x = fabs(x2-x1);
				dist_y = fabs(y2-y1);
		
				if (etat == FIN)
				{
					if(sqrt( dist_x*dist_x + dist_y*dist_y) < r1+R_ROBOT)
						return k;
				}
				else
				{
					if(sqrt( dist_x*dist_x + dist_y*dist_y) <= r1+R_ROBOT)
						return k;
				}
			}
		}
	}
	else
	{
		for (int k = 0 ; k < nb_robots ; k++)
		{
			if (k != i)
			{
				{
					x1 = robot[i]->pos_x;
					y1 = robot[i]->pos_y;
					x2 = robot[k]->pos_x;
					y2 = robot[k]->pos_y;
					
					dist_x = x2-x1;
					dist_y = y2-y1;
					if( sqrt( dist_x*dist_x + dist_y*dist_y) < 2*R_ROBOT)
					{
						return k;
					}
				}
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

void simulation_alignement(S2D rob,int i)
{
	robot = robot_get_robots();
	int j;
	//~ k = simulation_robot_colision_boucle(i, PARTICULE , FIN);
	j = simulation_robot_colision_boucle(i, ROBOT, FIN);
	//~ printf("j= %d \n" , j);
		
	
	while (/*k != LIBRE ||*/ j!= LIBRE)
	{
		j = simulation_robot_colision_boucle(i, ROBOT, FIN);
		//~ printf("j= %d \n" , j);
		if (j!=LIBRE)
		{
			S2D token = {robot[i]->pos_x, robot[i]->pos_y};
			S2D cible = {robot[j]->pos_x, robot[j]->pos_y};
			S2D init;
			init = robot_alignement(rob,token,cible,ROBOT,R_ROBOT,robot[i]->angle);
			robot = robot_recul(init, i);
			//~ printf("%d \n", simulation_robot_colision_boucle(i, ROBOT, FIN));

			//~ printf("buh \n");
			//~ double alpha = (((double) rand()/RAND_MAX))*M_PI;
			//~ robot = robot_vrot(i, &alpha);	

							
		}
		//~ k = simulation_robot_colision_boucle(i, PARTICULE, FIN);
		//~ if (k!=LIBRE)
		//~ {
			//~ S2D token = {robot[i]->pos_x, robot[i]->pos_y};
			//~ S2D cible = {particule[k]->pos_x, particule[k]->pos_y};
			//~ S2D init;
			//~ init = robot_alignement(rob,token,cible,PARTICULE,particule[k]->rayon,robot[i]->angle);
			//~ robot = robot_recul(init, i);

		//~ }
		
	}
}

STR_ROBOT** simulation_manuel (double translat , double rotat)
{
	for (int i = 0; i< nb_robots ; i++)
	{
		if (robot[i]->color == RED)
		{
			S2D init = {robot[i]->pos_x,robot[i]->pos_y};
			robot = robot_manuel(translat, rotat , i);
			simulation_alignement(init, i);
		}
	}
	return robot;
}


