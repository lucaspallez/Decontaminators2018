#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "error.h"
#include "utilitaire.h"
#include "simulation.h"
#include "tolerance.h"

static int nb_robots ,nb_particules; 

//ouvrir le fichier
int ouverture_fichier (const char file_name[])
{
	FILE *fichier_initial; 
	fichier_initial = fopen("file_name", "rb"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
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
	
	
	while (fgets(tab, 1000 ,fichier_initial)!= NULL )
	{
		line_number++;
		if((tab[0]=='#')||(tab[0]=='\n')) 
		continue;
		switch(etat)
		{
			case NBR : if (sscanf(tab , "%d" , &nb_robots)!= 1)
						{
							error_invalid_robot();
							return EXIT_FAILURE;
						}
						if (nb_robots%1 != 0)
						{
							error_invalid_nb_robots();
							return EXIT_FAILURE;
						} 	
			i=0;
			etat = DONNEES_R; 
			break;
			
			case DONNEES_R : debut = tab;
				while(debut)
				{
					if (sscanf (debut ,  "%lf" "%lf" "%lf" , &xr,&yr,&alpha) != 3 )
						{
							error_fin_liste_robots(line_number);
							return EXIT_FAILURE;
						}
						if (fabs(alpha)>M_PI)
						{
							error_invalid_robot_angle(alpha);
							return EXIT_FAILURE;
						}
					debut=debut+3;
					i++;
				}
			
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
			
			case NBP : if(sscanf(tab , "%d" , &nb_particules)!= 1)
						{
							error_invalid_particule();
							return EXIT_FAILURE;
						}
						if (nb_particules%1 != 0)
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
