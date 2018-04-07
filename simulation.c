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
	int line_number = 0;
	int i;
	char tab[MAX_LINE];
	double xr , yr , alpha;
	double xp , yp , r , t;
	double token_xr = 25, token_yr = 25, token_alpha = 4;
	double token_xp = 25 , token_yp = 25 , token_r = 5 , token_t = 2;
	int token;
	char *ret ;
	ret = tab; 
	int c = 0;
	
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
							return EXIT_FAILURE;
						}
						token = nb_robots;
						if ( nb_robots != token)
						{
							error_invalid_nb_robots();
							return EXIT_FAILURE;
						} 	
			i=0;
			etat = DONNEES_R; 
			break;
			
			case DONNEES_R :
					
					
						 while( sscanf (ret , "%lf %lf %lf" , &xr,&yr,&alpha) == 3 )
						 {
							if ( fabs(alpha) > M_PI)
							{
								error_invalid_robot_angle(alpha);
								return EXIT_FAILURE;
							}
							if ( xr != token_xr || yr != token_yr || alpha != token_alpha)
							{
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
						 
			
			if (c >= i)
			{
				error_fin_liste_robots(line_number);
				return EXIT_FAILURE;
			}
			c=i;
		
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
			ret = tab;			
			i=0;
			c = 0;
			etat = DONNEES_P ;
			break;
			
			case DONNEES_P : 
			
						
				while( sscanf (ret ,  "%lf %lf %lf %lf", &t , &r , &xp ,&yp) == 4 )
				{
					if ( r > R_PARTICULE_MAX || r < R_PARTICULE_MIN || fabs(xp) > 20 || fabs(yp) > 20|| t > E_PARTICULE_MAX ) 
					{
						error_invalid_particule_value(t , r , xp ,yp);
						return EXIT_FAILURE;
					}
					if ( xp != token_xp || yp != token_yp || r != token_r || t != token_t)
					{
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
				if ( c >= i )
				{
					error_fin_liste_particules(line_number);
					return EXIT_FAILURE;
				}
				c = i;
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
		
	}
	error_no_error_in_this_file();
	return EXIT_SUCCESS;
}
