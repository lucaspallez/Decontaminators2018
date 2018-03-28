#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "error.h"
#include "utilitaire.h"
#include "simulation.h"
#include "tolerance.h"

static double nb_robots ,nb_particules; 

//ouvrir le fichier
int ouverture_fichier (const char *file_name)
{
	FILE *fichier_initial; 
	fichier_initial = fopen(file_name, "rb"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
		return EXIT_FAILURE;
	} 
	
	
	enum etats {NBR , DONNEES_R , NBP , DONNEES_P,FIN_R,FIN};
	int etat = NBR;
	int line_number = 0;
	int i;
	char fin[9] = {0};
	char tab[1000];
	double xr , yr , alpha;
	double xp , yp , r , t;
	char *debut;
	int token, j= 0;
	
	
	while (fgets(tab, 1000 ,fichier_initial)!= NULL )
	{
		printf( "%s " , tab); 
		line_number++;
		if((tab[0] =='#')||(tab[0] =='\n') ||(tab[2] =='#')) 
		continue;
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
			
			case DONNEES_R : debut = tab;
				
				
 					if (sscanf (debut ,  "%lf" "%lf" "%lf" , &xr,&yr,&alpha) != 3 )
						{
							printf( "%d " , sscanf (debut ,  "%lf" "%lf" "%lf" , &xr,&yr,&alpha));
							printf ( "%lf " "%lf " "%lf "  , xr,yr,alpha);
 							error_fin_liste_robots(line_number);
							return EXIT_FAILURE;
						}
						if (fabs(alpha)>M_PI)
						{
							error_invalid_robot_angle(alpha);
							return EXIT_FAILURE;
						}
					while (j != 3 || debut != NULL)
					{	
						debut = debut+1;
						if (*debut != '\n' || *debut != ' ' || *debut != '\t')
							j++; 
					}
					i++;
				
			
			if(i == nb_robots)
				etat = FIN_R;
			break;
			
			case FIN_R : if (sscanf (tab , "%s" , fin) != 1)
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
			debut = tab;			
			i=0;
			etat = DONNEES_P ;
			break;
			
			case DONNEES_P : debut = tab;
			while (debut)
				{
					if(sscanf (debut ,  "%lf" "%lf" "%lf" "%lf", &xp , &yp , &r ,&t) != 4)
					{
						error_fin_liste_robots(line_number);
						return EXIT_FAILURE;
					}
					++i;
					debut = debut+4; 
				}

			if(i == nb_particules)
				etat = FIN;
			break;
			
			case FIN : if(sscanf (tab , "%s" , fin)!= 1)
						{
							error_missing_fin_liste_robots(line_number);
							return EXIT_FAILURE;
						}
			break;
				
			
		}
		
	}
	error_no_error_in_this_file();
	return EXIT_SUCCESS;
}
