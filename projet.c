#include <stdio.h>
#include <string.h>
//#include "simulation.h"
//#include "graphic.h"

#define STRLEN_E			5
#define STRLEN_D			4
#define TEST_ERROR			0
#define TEST_DRAW			1

void error(const char * file_name)
{
	//ouverture_fichier(file_name);
	printf("error\n");

}

void draw(void)
{
	printf("draw\n");
}

void incorrect_argument(void)
{
	printf("fail\n");
	//AFFICHER FICHIER BLANC (VOIR TEST.X)
}




int main(int argc, char **argv)
{
	if(argc == 3)
	{
		const char * file_name = argv[2];
		const char test_e[STRLEN_E] = {'E','r','r','o','r'};
		const char test_d[STRLEN_D] = {'D','r','a','w'};
		
		int compteur = 0;
		for(int i=0; i<strlen(argv[1]);i++)
			if(strncmp(test_e[i],argv[1][i],strlen(argv[1])))
				compteur++;
		
		}
	else
		incorrect_argument();
	return 0;
}
