#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "utilitaire.h"
#include "robot.h"
#include "tolerance.h"
#include "constantes.h"

static double nb_robots = INITIALISATION;
static double *pointer_r; 

double * robot_lecture_fichier(const char *file_name)
{
	enum etats {NBR , DONNEES_R ,FIN_R, PARTICULE};
	int etat = NBR;
	int line_number = INITIALISATION;
	int i ,k = 0;
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
		printf( "%s " , tab);
		k=INITIALISATION;
		switch(etat)
		{
			case NBR : if (sscanf(tab , "%lf" , &nb_robots)!= 1)
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
				 if (sscanf(tab+ k , "%lf %lf %lf" , &xr,&yr,&alpha) != 3)
					break;
				 if ( fabs(alpha) > M_PI)
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
			if (sscanf (tab , "%lf %lf %lf" , &xr,&yr,&alpha) == NBR_COORDONNEES_R)
			{
				error_missing_fin_liste_robots(line_number);
				return NULL;
			}
			etat = PARTICULE;
			case PARTICULE : break;
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
	while(compteur<=NBR_COORDONNEES_R && *(tab+k)!='\n' && *(tab+k)!='\r')
	{
		if (token != compteur || *(tab+k) == '.')
			while (*(tab+k) != '\t' && *(tab+k) != ' ' && *(tab+k) != '\n' && *(tab+k) != '\r')
				k++;
		token = compteur;
		
		if (*(tab+k) == '\t' || *(tab+k) == ' ')
			k++;
		else
		{
			if (compteur != NBR_COORDONNEES_R)
			{
				k++;
				compteur++;
			}
			else
			{
				compteur++;
			}
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
			y1 = *(pointer_r + (j * NBR_COORDONNEES_R) + 1);
			x2 = *(pointer_r + NBR_COORDONNEES_R*(j+k));
			y2 = *(pointer_r + NBR_COORDONNEES_R*(j+k) +1 );
			
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


