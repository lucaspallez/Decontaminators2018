#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "utilitaire.h"
#include "simulation.h"
#include "tolerance.h"
#include "constantes.h"

static double nb_robots ,nb_particules; 


int ouverture_fichier (const char *file_name)
{
	FILE *fichier_initial; 
	fichier_initial = fopen(file_name, "rt"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
		return EXIT_FAILURE;
	} 
	
	
	enum etats {NBR , DONNEES_R , NBP , DONNEES_P,FIN_R,FIN};
	int etat = NBR;
	int line_number = INITIALISATION;
	int i;
	char tab[MAX_LINE];
	double x1, x2, y1, y2, r1 , r2;
	double xr , yr , alpha;
	double xp , yp , r , t;
	double token_xr = INIT_TOKEN_POS, token_yr = INIT_TOKEN_POS, token_alpha = INIT_TOKEN_ALPHA;
	double token_xp = INIT_TOKEN_POS , token_yp = INIT_TOKEN_POS , token_r = INIT_TOKEN_R , token_t = INIT_TOKEN_T;
	int token = INITIALISATION;
	char *ret ;
	ret = tab;
	
	double *pointer_r; 
	double *pointer_p;
	
	while (fgets(tab, MAX_LINE ,fichier_initial)!= NULL)
	{
		printf( "%s " , tab);
		
		if((tab[0] =='#')||(tab[0] =='\n') || (tab[1] =='#')|| (tab[2] =='#' || (tab[4] =='#')))	
			continue;
		line_number++;
		ret = tab; 
		 
		
		switch(etat)
		{
			case NBR : if (sscanf(tab , "%lf" , &nb_robots)!= 1)
						{
							error_invalid_robot();
							return EXIT_FAILURE;
						}
						token = nb_robots;
			if ( nb_robots != token)
			{
				error_invalid_nb_robots();
				return EXIT_FAILURE;
			}
			
			pointer_r = malloc( nb_robots * sizeof(STR_ROBOT)); 	
						
			i= INITIALISATION;
			token = INITIALISATION;
			etat = DONNEES_R; 
			break;
			
			case DONNEES_R :
					
					
						 while( sscanf (ret , "%lf %lf %lf" , &xr,&yr,&alpha) == NBR_COORDONNEES_R )
						 {
							if ( fabs(alpha) > M_PI)
							{
								error_invalid_robot_angle(alpha);
								return EXIT_FAILURE;
							}
							if ( xr != token_xr || yr != token_yr || alpha != token_alpha)
							{
								*(pointer_r + (i*NBR_COORDONNEES_R)) = xr;
								*(pointer_r + ((i*NBR_COORDONNEES_R) + 1)) = yr;
								*(pointer_r + ((i*NBR_COORDONNEES_R) + 2)) = alpha;
								
								
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
				return EXIT_FAILURE;
			}
			token=i;
		
			if ( i > nb_robots)
			{
				error_missing_fin_liste_robots(line_number);
				return EXIT_FAILURE;
			}
			
			if( i == nb_robots)
				etat = FIN_R;
			
			break;
			
			case FIN_R : if (*tab != 'F')
						{	
							error_missing_fin_liste_robots(line_number);
							return EXIT_FAILURE;
						}
			
			etat = NBP;
			break;
			
			case NBP : if(sscanf(tab , "%lf" , &nb_particules)!= 1)
						{
							error_invalid_particule();
							return EXIT_FAILURE;
						}
						token = nb_particules;
						if (nb_particules != token)
						{
							 error_invalid_nb_particules();
							 return EXIT_FAILURE;
						}
			pointer_p = malloc( nb_particules * sizeof(STR_PARTICULE)); 			
			ret = tab;			
			i= INITIALISATION;
			token = INITIALISATION;
			etat = DONNEES_P ;
			break;
			
			case DONNEES_P : 
			
						
				while( sscanf (ret ,  "%lf %lf %lf %lf", &t , &r , &xp ,&yp) == NBR_COORDONNEES_P )
				{
					if ( r > R_PARTICULE_MAX || r < R_PARTICULE_MIN || fabs(xp) > DMAX || fabs(yp) > DMAX|| t > E_PARTICULE_MAX ) 
					{
						error_invalid_particule_value(t , r , xp ,yp);
						return EXIT_FAILURE;
					}
					if ( xp != token_xp || yp != token_yp || r != token_r || t != token_t)
					{
						*(pointer_p + (i*NBR_COORDONNEES_P)) = t;
						*(pointer_p + ((i*NBR_COORDONNEES_P) + 1)) = r;
						*(pointer_p + ((i*NBR_COORDONNEES_P) + 2)) = xp;
						*(pointer_p + ((i*NBR_COORDONNEES_P) + 3)) = yp;
						
						i++;
					}
					token_xp = xp;
					token_yp = yp;
					token_r = r;
					token_t = t;
					
					ret = strchr ( ret , '\t');
					if (ret == NULL)
						break;
						
					ret=ret +1;
				 }
				 
				token_xp = INIT_TOKEN_POS;
				token_yp = INIT_TOKEN_POS;
				token_r = INIT_TOKEN_R; 
				token_t = INIT_TOKEN_T;

				if ( token >= i )
				{
					error_fin_liste_particules(line_number);
					return EXIT_FAILURE;
				}
				token = i;
				if ( i > nb_particules)
				{
					error_missing_fin_liste_particules(line_number);
					return EXIT_FAILURE;
				}
				
					
			

			if(i == nb_particules)
				etat = FIN;
			break;
			
			case FIN : if( *tab != 'F')
						{
							error_missing_fin_liste_particules(line_number);
							return EXIT_FAILURE;
						}
			break;
				
			
		}
		//~ for (int j = 0 ; j< nb_particules * 4; j++)
		//~ {
			//~ printf ( "%d \n" , *(pointer_p + j));
		//~ }
	}
	
	// Robot _ Robot
	
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
				return EXIT_FAILURE;
			}
		}
	}
	
	
	//Particule_ Particule 
	
	for (int j = 0 ; j < nb_particules ; j++)
	{
		for (int k = 1 ; k < nb_particules - j ; k++)
		{
			r1 = *(pointer_p + (j * NBR_COORDONNEES_P) + 1);
			r2 = *(pointer_p + NBR_COORDONNEES_P*(j+k) + 1);
			x1 = *(pointer_p + (j * NBR_COORDONNEES_P) + 2);
			y1 = *(pointer_p + (j * NBR_COORDONNEES_P) + 3);
			x2 = *(pointer_p + NBR_COORDONNEES_P*(j+k) + 2);
			y2 = *(pointer_p + NBR_COORDONNEES_P*(j+k) + 3);
			
			double a = fabs(x2-x1);
			double b = fabs (y2-y1);
			//~ printf( "%lf %lf %lf %lf \n" , x1 , y1 , x2 , y2);
			//~ printf( "%lf \n" , sqrt( a*a + b*b));
			if ( sqrt( a*a + b*b) < r1+r2)
			{
				error_collision(PARTICULE_PARTICULE, j+1 , j+k+1);
				return EXIT_FAILURE;
			}
		}
	}
	
	//Robot_Particule
	
	for (int j = 0 ; j < nb_particules ; j++)
	{
		for (int k = 0 ; k < nb_particules ; k++)
		{
			x1 = *(pointer_r + (j * NBR_COORDONNEES_R));
			y1 = *(pointer_r + (j * NBR_COORDONNEES_R) + 1);
			r1 = *(pointer_p + (NBR_COORDONNEES_P*k) + 1);
			x2 = *(pointer_p + (NBR_COORDONNEES_P*k) + 2);
			y2 = *(pointer_p + (NBR_COORDONNEES_P*k) + 3);
			
			if ( sqrt( fabs(x2-x1)*fabs(x2-x1) + fabs(y2-y1))*fabs(y2-y1) < r1+R_ROBOT)
			{
				error_collision(ROBOT_PARTICULE, j+1 , k+1);
				return EXIT_FAILURE;
			}
		}
	}
	
	
	error_no_error_in_this_file();
	return EXIT_SUCCESS;
}
