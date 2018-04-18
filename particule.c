#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "utilitaire.h"
#include "particule.h"
#include "tolerance.h"
#include "constantes.h"

static double nb_particules;
static double *pointer_p;


double * particule_lecture_fichier(const char *file_name)
{
	
	FILE *fichier_initial; 
	fichier_initial = fopen(file_name, "rt"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
		return NULL;
	}
	
	enum etats {SEARCH , NBP , DONNEES_P, FIN};
	int etat = SEARCH;
	int line_number = INITIALISATION;
	int i ;
	char tab[MAX_LINE];
	double x1, x2, y1, y2, r1 , r2;
	double xp , yp , r , t;
	double token_xp = INIT_TOKEN_POS , token_yp = INIT_TOKEN_POS , token_r = INIT_TOKEN_R , token_t = INIT_TOKEN_T;
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
		
		switch (etat)
		{
			case SEARCH : ret = strchr( tab , 'F');
						if (ret != NULL)
							etat = NBP;
						break;
							
			case NBP : if(sscanf(tab , "%lf" , &nb_particules)!= 1)
						{
							error_invalid_particule();
							return NULL;
						}
						token = nb_particules;
						if (nb_particules != token)
						{
							 error_invalid_nb_particules();
							 return NULL;
						}
			pointer_p = malloc( nb_particules * sizeof(STR_PARTICULE)); 			
			ret = tab;			
			i= INITIALISATION;
			token = INITIALISATION;
			etat = DONNEES_P ;
			break;
			
			case DONNEES_P : while( sscanf (ret ,  "%lf %lf %lf %lf", &t , &r , &xp ,&yp) == NBR_COORDONNEES_P )
							{
								if ( r > R_PARTICULE_MAX || r < R_PARTICULE_MIN || fabs(xp) > DMAX || fabs(yp) > DMAX|| t > E_PARTICULE_MAX || t < 0) 
								{
									error_invalid_particule_value(t , r , xp ,yp);
									//~ free(pointer_p);
									return NULL;
									
								}
								if ( xp != token_xp || yp != token_yp || r != token_r || t != token_t)
								{
									if ( i <= nb_particules)
									{
										*(pointer_p + (i*NBR_COORDONNEES_P)) = t;
										*(pointer_p + ((i*NBR_COORDONNEES_P) + 1)) = r;
										*(pointer_p + ((i*NBR_COORDONNEES_P) + 2)) = xp;
										*(pointer_p + ((i*NBR_COORDONNEES_P) + 3)) = yp;
									}
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
				//~ free(pointer_p);
				return NULL;
			}
			token = i;
			if ( i > nb_particules)
			{
				error_missing_fin_liste_particules(line_number + 1);
				//~ free(pointer_p);
				return NULL;
			}

			if(i == nb_particules)
				etat = FIN;
			break;
			
			case FIN : if (sscanf(tab , "%lf %lf %lf %lf" , &t ,&r ,&xp, &yp) == NBR_COORDONNEES_P )
						{
							error_missing_fin_liste_particules(line_number);
							//~ free(pointer_p);
							return NULL;
						}
			break;
				
			
		}
	}
	
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

			if ( sqrt( a*a + b*b) < r1+r2)
			{
				error_collision(PARTICULE_PARTICULE, j+1 , j+k+1);
				//~ free(pointer_p);
				return NULL;
			}
		}
	}
	return pointer_p;		
		
}

double particule_nombre_particules()
{
	return nb_particules;
}
