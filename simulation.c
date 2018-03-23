#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "utilitaire.h"
#include "simulation.h"
#include "tolerance.h"

//ouvrir le fichier
void ouverture_fichier (const char file_name[])
{
	FILE *fichier_initial; 
	fichier_initial = fopen("file_name", "rb"); 
	if (fichier_initial == NULL) 
	{
		error_file_missing (file_name);
	} 
	
	
	enum Etats {NBR , DONNEES_R , NBP , DONNEES_P};
	int etat = NBR;
	int nb_robots ,nb_particules, i;
	char tab[1000], 
	char donnees_R [3]
	char donnees_P[4]
	
	
	while (fgets(tab, 1000 ,fichier_initial)!= NULL )
	{
		if((tab[0]=='#')||(tab[0]=='\n')) 
		continue;
		switch(etat)
		{
			case NBR : sscanf(tab , "%d" , &nb_robots); i=0;
			etat = DONNEES_R; break;
			case DONNEES_R : sscanf (tab ,  "%d" "%d" "%d" , &donnees_R);
			++i;  
			if(i == nb_robots)
				etat = NBP;
			break;
			case NBP : sscanf(tab , "%d" , &nb_particules); i=0;
			etat = DONNEES_P ; break;
			case DONNEES_R : sscanf (tab ,  "%d" "%d" "%d" "%d", &donnees_P);
			++i;  break
				
			
		}
		
	}
}



   
