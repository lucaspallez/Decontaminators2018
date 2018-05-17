#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "utilitaire.h"
#include "robot.h"
#include "constantes.h"

static double nb_robots = INITIALISATION;
static double *pointer_r; 
static STR_ROBOT **robot;

double * robot_lecture_fichier(const char *file_name)
{
	enum etats {NBR , DONNEES_R ,FIN_R, PARTICULE};
	int etat = NBR;
	int line_number = INITIALISATION;
	int i ,k = INITIALISATION;
	char tab[MAX_LINE];
	double xr , yr , alpha;
	int token = INITIALISATION;
	
	FILE *fichier_initial; 
	fichier_initial = fopen(file_name, "rt"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
		return NULL;
	} 
	while (fgets(tab, MAX_LINE ,fichier_initial)!= NULL)
	{
		k=INITIALISATION;
		line_number++;
		while (*(tab+k) == ' ' || *(tab+k) == '\t')
			k++;
		if (*(tab+k) == '#' || *(tab+k) == '\n' || *(tab+k) == '\r')
			continue;	
		k=INITIALISATION;
		switch(etat)
		{
			case NBR : 
				if (sscanf(tab , "%lf" , &nb_robots)!= 1)
				{
					error_invalid_robot();
					return NULL;
				}
				token = nb_robots;
				if ( nb_robots != token)
				{
					error_invalid_nb_robots();
					return NULL;
				}
				pointer_r = malloc( nb_robots * sizeof(STR_ROBOT)); 	
				if ( nb_robots == 0 )
				{
					etat = PARTICULE;
					break;
				}					
				i= INITIALISATION;
				token = INITIALISATION;
				etat = DONNEES_R; 
				break;
			
			case DONNEES_R :  
				while( *(tab + k) != '\n' && *(tab+k) != '\r')
				{
					if(sscanf(tab+k,"%lf %lf %lf",&xr,&yr,&alpha)!= NBR_COORDONNEES_R-3)
						break;
					if(fabs(alpha) > M_PI)
					{
						error_invalid_robot_angle(alpha);
						return NULL;
					}	
					if ( i <= nb_robots )
					{
						*(pointer_r + (i*NBR_COORDONNEES_R)) = xr;
						*(pointer_r + ((i*NBR_COORDONNEES_R) + 1)) = yr;
						*(pointer_r + ((i*NBR_COORDONNEES_R) + 2)) = alpha;
					}									
					i++;
					k = robot_avancement(k, tab);		
				}
				if (token >= i)
				{
					error_fin_liste_robots(line_number);
					return NULL;
				}
				token=i;
				if ( i > nb_robots)
				{
					error_missing_fin_liste_robots(line_number +1);
					return NULL;
				}
				if( i == nb_robots)
					etat = FIN_R;
				break;
			
			case FIN_R : 
				if(sscanf(tab,"%lf %lf %lf",&xr,&yr,&alpha) == NBR_COORDONNEES_R-3)
				{
					error_missing_fin_liste_robots(line_number);
					return NULL;
				}
				etat = PARTICULE;
			
			case PARTICULE : 
				break;
		}
	}
	if (robot_collision())
		return NULL;
	fclose(fichier_initial);
	return pointer_r;
}

double robot_nombre_robots()
{
	return nb_robots;
}

int robot_avancement(int k, char *tab)
{
	int compteur=INITIALISATION;
	int token = INITIALISATION;
	while(compteur<=NBR_COORDONNEES_R-3&&*(tab+k)!='\n'&&*(tab+k)!='\r')
	{
		if(token != compteur || *(tab+k) == '.')
			while (*(tab+k)!='\t'&&*(tab+k)!=' '&&*(tab+k)!='\n' &&*(tab+k)!='\r')
				k++;
		token = compteur;
		if(*(tab+k) == '\t' || *(tab+k) == ' ')
			k++;
		else
		{
			if (compteur != NBR_COORDONNEES_R-3)
			{
				k++;
				compteur++;
			}
			else
				compteur++;
		}
	}
	k--;
	return k;
}

bool robot_collision()
{
	double x1, x2, y1, y2;
	for (int j = 0 ; j < nb_robots ; j++)
	{
		for (int k = 1 ; k < nb_robots - j ; k++)
		{
			double dist_x;
			double dist_y;

			x1 = *(pointer_r+ (j * NBR_COORDONNEES_R) );
			y1 = *(pointer_r+ (j* NBR_COORDONNEES_R) + 1);
			x2 = *(pointer_r+ NBR_COORDONNEES_R*(j+k));
			y2 = *(pointer_r+ NBR_COORDONNEES_R*(j+k) + 1);
			
			dist_x = fabs(x2-x1);
			dist_y = fabs(y2-y1);
			if ( sqrt( dist_x*dist_x + dist_y*dist_y) < 2*R_ROBOT)
			{
				error_collision(ROBOT_ROBOT, j+1 , j+k+1);
				return 1;
			}
		}
	}
	return 0;
}

STR_ROBOT** robot_donnees()
{
	robot = malloc(nb_robots*sizeof(STR_ROBOT));
	for (int y = 0 ; y < nb_robots ; y++)
	{
		robot[y] = malloc(sizeof(STR_ROBOT));
	}
	for (int z = 0 ; z< nb_robots ; z++)
	{
		robot[z]->pos_x = *(pointer_r+(z*NBR_COORDONNEES_R));
		robot[z]->pos_y = *(pointer_r+((z*NBR_COORDONNEES_R)+1));
		robot[z]->angle = *(pointer_r+((z*NBR_COORDONNEES_R)+2));
		robot[z]->actif = 0;
	}
	
	return robot;
}

void robot_free_robots()
{
	for (int i=0 ; i< nb_robots ; i++)
	{
		free(robot[i]);
	}
	free(robot);
}

STR_ROBOT** robot_vrot(int i, double*angle)
{
	double vrot = *(angle) / DELTA_T;
	if (fabs(vrot) > VROT_MAX)
	{			
		if(vrot > 0)
			vrot = VROT_MAX;
		else
			vrot = -VROT_MAX;
	}
	robot[i]->angle = robot[i]->angle + vrot*DELTA_T;
	
	return robot;
}

double robot_temps_rot_calcul(double *angle)
{
	double vrot = *(angle) / DELTA_T;
	if (fabs(vrot) > VROT_MAX)
	{			
		if(vrot > 0)
			vrot = VROT_MAX;
		else
			vrot = -VROT_MAX;
	}
	double temps;
	if(vrot != 0)
		temps = fabs(*(angle)/(vrot));
	else
		temps = 0;
	return temps;
}

double robot_vtran(double L)
{
	double vtran;
	vtran = L/DELTA_T;
	if( fabs(vtran) > VTRAN_MAX)
	{
		if (vtran > 0)
			vtran = VTRAN_MAX;
		else
			vtran = -VTRAN_MAX;
	}
	return vtran;
}

STR_ROBOT** robot_deplacement(S2D rob, int i)
{
	S2D part;
	double * delta_gamma = NULL;
	double ecart_angle;
	delta_gamma = &ecart_angle;
	double vtran;
	part.x = robot[i]->occup.x;  
	part.y = robot[i]->occup.y;
	double L = 0;
	L = util_distance(rob, part);
	vtran = robot_vtran(L);
	util_ecart_angle(rob,robot[i]->angle,part,delta_gamma);
	
	if(fabs(*delta_gamma) > EPSIL_ZERO)
	{
			if(fabs(*(delta_gamma)) > M_PI/2)
			{
				robot_vrot(i,delta_gamma);
			}
			else
			{
				robot_vrot(i,delta_gamma);
				rob = util_deplacement(rob, robot[i]->angle, vtran*DELTA_T);
				robot[i]->pos_x=rob.x;
				robot[i]->pos_y=rob.y;	
			}
		
	}
	else
	{
		rob = util_deplacement(rob, robot[i]->angle, vtran*DELTA_T);
		robot[i]->pos_x=rob.x;
		robot[i]->pos_y=rob.y;
	}
	return robot;
}

STR_ROBOT ** robot_activation_desactivation(int i, bool a)
{
	if (a)
		robot[i]->actif = 1;
	else
		robot[i]->actif = 0;
	return robot;
}

STR_ROBOT ** robot_occupation(double x , double y , int i)
{
	robot[i]->occup.x = x;
	robot[i]->occup.y = y;
	return robot;
}

STR_ROBOT** robot_color(int i)
{
	robot[i]->color = BLUE;
	return robot;
}

