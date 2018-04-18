#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "utilitaire.h"
#include "robot.h"
#include "tolerance.h"
#include "constantes.h"

static double nb_robots = INITIALISATION;
static double *pointer_r; 


double * robot_lecture_fichier(const char *file_name)
{
	FILE *fichier_initial; 
	fichier_initial = fopen(file_name, "rt"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
		return NULL;
	} 
	
	
	enum etats {NBR , DONNEES_R ,FIN_R, PARTICULE};
	int etat = NBR;
	int line_number = INITIALISATION;
	int i;
	char tab[MAX_LINE];
	double x1, x2, y1, y2;
	double xr , yr , alpha;
	double token_xr = INIT_TOKEN_POS, token_yr = INIT_TOKEN_POS, token_alpha = INIT_TOKEN_ALPHA;
	int token = INITIALISATION;
	char *ret ;
	ret = tab;
	
	
	
	while (fgets(tab, MAX_LINE ,fichier_initial)!= NULL)
	{
		
		
		if((tab[0] =='#')||(tab[0] =='\n') || (tab[1] =='#')|| (tab[2] =='#' || (tab[4] =='#')))	
			continue;
		line_number++;
		ret = tab; 
		printf( "%s " , tab);
		 
		
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
						
			i= INITIALISATION;
			token = INITIALISATION;
			etat = DONNEES_R; 
			break;
			
			case DONNEES_R :  while( sscanf (ret , "%lf %lf %lf" , &xr,&yr,&alpha) == NBR_COORDONNEES_R )
							 {
								if ( fabs(alpha) > M_PI)
								{
									error_invalid_robot_angle(alpha);
									//~ free (pointer_r);
									return NULL;
								}
								if ( xr != token_xr || yr != token_yr || alpha != token_alpha)
								{
									if ( i <= nb_robots )
									{
										*(pointer_r + (i*NBR_COORDONNEES_R)) = xr;
										*(pointer_r + ((i*NBR_COORDONNEES_R) + 1)) = yr;
										*(pointer_r + ((i*NBR_COORDONNEES_R) + 2)) = alpha;
									}
									
									
									i++;
								}
								token_xr = xr;
								token_yr = yr;
								token_alpha = alpha;
								 
								ret = strchr ( ret , '\t');
								if (ret == NULL)
									break;
								ret=ret +1;
								 
							}
						 
			token_xr = INIT_TOKEN_POS;
			token_yr = INIT_TOKEN_POS;
			token_alpha = INIT_TOKEN_ALPHA;
			
			if (token >= i)
			{
				error_fin_liste_robots(line_number);
				//~ free (pointer_r);
				return NULL;
			}
			token=i;
		
			if ( i > nb_robots)
			{
				error_missing_fin_liste_robots(line_number +1);
				//free (pointer_r);
				return NULL;
			}
			
			if( i == nb_robots)
				etat = FIN_R;
			
			break;
			
			case FIN_R : if (sscanf (tab , "%lf %lf %lf" , &xr,&yr,&alpha) == NBR_COORDONNEES_R)
						{
							error_missing_fin_liste_robots(line_number);
							//~ free(pointer_r);
							return NULL;
						}
						etat = PARTICULE;
			
			case PARTICULE : break;
			
		}
	
	}
	
	for (int j = 0 ; j < nb_robots ; j++)
	{
		for (int k = 1 ; k < nb_robots - j ; k++)
		{

			x1 = *(pointer_r+ (j * NBR_COORDONNEES_R) );
			y1 = *(pointer_r + (j * NBR_COORDONNEES_R) + 1);
			x2 = *(pointer_r + NBR_COORDONNEES_R*(j+k));
			y2 = *(pointer_r + NBR_COORDONNEES_R*(j+k) +1 );
			
			if ( sqrt( fabs(x2-x1)*fabs(x2-x1) + fabs(y2-y1))*fabs(y2-y1) < 2*R_ROBOT)
			{

				error_collision(ROBOT_ROBOT, j+1 , j+k+1);
				//~ free (pointer_r);
				return NULL;
			}
		}
		
	}
		
	
	return pointer_r;
}

double robot_nombre_robots()
{
	return nb_robots;
}

