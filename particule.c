#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "utilitaire.h"
#include "particule.h"
#include "constantes.h"

static double nb_particules;
static double *pointer_p;
static STR_PARTICULE **particule;
static int num = 1;

double * particule_lecture_fichier(const char *file_name)
{
	enum etats {SEARCH , NBP , DONNEES_P, FIN};
	int etat = SEARCH;
	int line_number = INITIALISATION;
	int compteur , k = INITIALISATION;
	char tab[MAX_LINE];
	double xp , yp , r , t;
	int token = INITIALISATION;
	char *ret ;
	ret = tab;
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
		if (*(tab+k)=='#'||*(tab+k)=='\n'||*(tab+k)=='\r')
			continue;
		ret = tab;
		k=INITIALISATION;
		switch (etat)
		{
			case SEARCH : 
				ret = strstr( tab , "FIN_LISTE");
				if (ret != NULL)
					etat = NBP;
				break;
											
			case NBP :
				if(sscanf(tab , "%lf" , &nb_particules)!= 1)
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
			if (nb_particules == 0)
			{
				etat = FIN;
				break;
			}			
			ret = tab;			
			compteur= INITIALISATION;
			token = INITIALISATION;
			etat = DONNEES_P ;
			break;
			case DONNEES_P : 
			while( *(tab + k) != '\n' && *(tab+k) != '\r')
			{
				if (sscanf(tab+k,"%lf %lf %lf %lf",&t,&r,&xp,&yp)
					!= NBR_COORDONNEES_P -1)
					 break;
				if (r > R_PARTICULE_MAX || r < R_PARTICULE_MIN ||
					fabs(xp) > DMAX || fabs(yp) > DMAX||
					t > E_PARTICULE_MAX || t < 0) 
				{
					error_invalid_particule_value(t , r , xp ,yp);
					return NULL;
				}
				if ( compteur <= nb_particules)
				{
					*(pointer_p+(compteur*NBR_COORDONNEES_P)) =t;
					*(pointer_p+((compteur*NBR_COORDONNEES_P)+1)) =r;
					*(pointer_p+((compteur*NBR_COORDONNEES_P)+2)) =xp;
					*(pointer_p+((compteur*NBR_COORDONNEES_P)+3)) =yp;
				}									
				compteur++;
				k = particule_avancement(k, tab);
			}
			if (token >= compteur)
			{
				error_fin_liste_particules(line_number);
				return NULL;
			}
			token = compteur;
			if ( compteur > nb_particules)
			{
				error_missing_fin_liste_particules(line_number+1);
				return NULL;
			}

			if(compteur == nb_particules)
				etat = FIN;
			break;
			case FIN : 
			if(sscanf(tab,"%lf %lf %lf %lf",&t,&r,&xp,&yp) == NBR_COORDONNEES_P -1)
			{
				error_missing_fin_liste_particules(line_number);
				return NULL;
			}
			break;
		}
	}
	if (particule_collision())
		return NULL;
	fclose(fichier_initial);
	return pointer_p;	
}

double particule_nombre_particules()
{
	return nb_particules;
}

int particule_avancement(int k, char * tab)
{
	int compteur= INITIALISATION;
	int token = INITIALISATION;
	
	while(compteur<=NBR_COORDONNEES_P-1&&*(tab+k)!='\n'&&*(tab+k)!='\r')
	{
		if (token != compteur || *(tab+k) == '.')
			while(*(tab+k)!='\t'&&*(tab+k)!=' '&& *(tab+k)!='\n'&&*(tab+k)!='\r')
				k++;
		token = compteur;
		if (*(tab+k) == '\t' || *(tab+k) == ' ')
			k++;
		else
		{
			if (compteur != NBR_COORDONNEES_P-1)
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

bool particule_collision()
{
	double x1, x2, y1, y2, r1 , r2;
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
			
			double dist_x = fabs(x2-x1);
			double dist_y = fabs (y2-y1);
			if ( sqrt( dist_x*dist_x + dist_y*dist_y) < r1+r2)
			{
				error_collision(PARTICULE_PARTICULE, j+1 , j+k+1);
				return 1;
			}
		}
	}
	return 0;
}

STR_PARTICULE** particule_donnees()
{
	particule = malloc(nb_particules*sizeof(STR_PARTICULE));
	for (int y = 0 ; y < nb_particules ; y++)
	{
		particule[y] = malloc(sizeof(STR_PARTICULE));
	}
	for (int z = 0 ; z< nb_particules ; z++)
	{
		particule[z]->energie = *(pointer_p+(z*NBR_COORDONNEES_P));
		particule[z]->rayon = *(pointer_p+((z*NBR_COORDONNEES_P)+1));
		particule[z]->pos_x = *(pointer_p+((z*NBR_COORDONNEES_P)+2));
		particule[z]->pos_y = *(pointer_p+((z*NBR_COORDONNEES_P)+3));
		particule[z]->num = num;
		num++;	
	}
	return particule;
}

STR_PARTICULE** particule_decomposition(int i)
{	
	if ((particule[i]->rayon)*R_PARTICULE_FACTOR > R_PARTICULE_MIN)
	{
		for (int j = 0 ; j < 4 ; j++)
		{
			int k=nb_particules;
			nb_particules++;
			particule = realloc(particule, nb_particules*sizeof(STR_PARTICULE));
			particule[k]=malloc(sizeof(STR_PARTICULE));
			particule[k]->rayon = (particule[i]->rayon)*R_PARTICULE_FACTOR;
			particule[k]->energie = particule[i]->energie*E_PARTICULE_FACTOR;
			if (j == 0 || j == 1)
				particule[k]->pos_x = (particule[i]->pos_x+particule[k]->rayon);
			else
				particule[k]->pos_x = (particule[i]->pos_x-particule[k]->rayon);
			if (j == 0 || j == 3)
				particule[k]->pos_y = (particule[i]->pos_y+particule[k]->rayon);
			else
				particule[k]->pos_y = (particule[i]->pos_y-particule[k]->rayon);
			particule[k]->num = num;
			num++;	
		}
		free(particule[i]);
		particule[i]=NULL;
		nb_particules--;
		particule = realloc(particule, nb_particules*sizeof(STR_PARTICULE));
		
	}
	return particule;
}


void particule_free_particules()
{
	for (int i=0 ; i< nb_particules ; i++)
	{
		free(particule[i]);
	}
	free(particule);
}

void particule_tri()
{
	STR_PARTICULE* token;
	int compteur;
	for(int k=0; k< nb_particules;k++)
	{
		token = particule[k];
		compteur = k;
		for(int i=k+1 ; i< nb_particules-k ; i++)
		{
			if (token->rayon < particule[i]->rayon)
			{
				token = particule[i];
				compteur++;
			}
		}
		particule[compteur]=particule[k];
		particule[k]=token;
		
	}
	for(int i=0;i< nb_particules;i++)
	{
		printf("%lf \n" , particule[i]->rayon);
	}
}

int particule_recherche (int k)
{
	for (int i = 0 ; i < nb_particules ; i++)
	{
		if (particule[i]->num==k)
		{
			return i;
		}
	}
	return -1;
}
