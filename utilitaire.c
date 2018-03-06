#include "utilitaire.h"
#include "constantes.h"
#include <stdbool.h> 
#include <math.h> 
#include <stdlib.h>

double util_distance (S2D a, S2D b)
{
	double m , p;
	double d;
	m = fabs( a.x - b.x);
	p = fabs( a.y - b.y);
	d = sqrt ( m*m + p*p );
	
	return d;
}


double  util_angle(S2D a, S2D b) 
{
	double m , p , d;
	m = fabs( a.x - b.x);
	p = fabs( a.y - b.y);
	if (m != 0)
		d = atan(p/m);
	else if (p != 0)
		d = M_PI/2;
	else
		d = 0;
		
	return d;
}


void  util_range_angle(double * p_angle) 
{
	while (*p_angle > M_PI)
			*p_angle = *p_angle - 2*M_PI;
	while (*p_angle <= M_PI)
			*p_angle = *p_angle + 2*M_PI;
}
		
bool  util_point_dehors(S2D a, double max)
{
	if ( fabs (a.x) > max || fabs (a.y) > max )
		return 1;
	else 
		return 0;
}
 
bool  util_alpha_dehors(double alpha)
{
	if ( fabs (alpha) > M_PI )
		return 1;
	else
		return 0;
} 

bool  util_point_dans_cercle(S2D a, C2D c)
{
	double distance;
	distance = util_distance ( a , c.centre);
	if (distance < c.rayon - EPSIL_ZERO)
		return 1;
	else
		return 0;
}

bool  util_collision_cercle(C2D a, C2D b, double * p_dist)
{
	*p_dist = util_distance (a.centre , b.centre);
	if (*p_dist <= a.rayon + b.rayon)
		return 1;
	else 
		return 0;
}

bool  util_deplacement(S2D p, double alpha, double dist); 

bool  util_ecart_angle(S2D a, double alpha, S2D b, double *p_ecart_angle); 
 
bool  util_alignement(S2D a, double alpha, S2D b); 

bool  util_inner_triangle(double la, double lb, double lc, double lb_new, 
      double * p_la_new);
