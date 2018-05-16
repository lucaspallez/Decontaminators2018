#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "tolerance.h"
#include "utilitaire.h"

double util_distance (S2D a, S2D b)
{
	double delta_x , delta_y;
	double dist;
	delta_x = fabs(a.x - b.x);
	delta_y = fabs(a.y - b.y);
	dist = sqrt(delta_x*delta_x + delta_y*delta_y);
	
	return dist;
}


double  util_angle(S2D a, S2D b) 
{
	double delta_x , delta_y , angle;
	delta_x =  b.x - a.x;
	delta_y =  b.y - a.y;
		angle = atan2(delta_y,delta_x);
		if(angle == -M_PI)
			angle = M_PI;
	return angle;
}


void  util_range_angle(double * p_angle) 
{
	while (*p_angle > M_PI)
			*p_angle = *p_angle - 2*M_PI;
	while (*p_angle <= -M_PI)
			*p_angle = *p_angle + 2*M_PI;
}
		
bool  util_point_dehors(S2D a, double max)
{
	if (fabs(a.x) > max || fabs (a.y) > max )
		return 1;
	else 
		return 0;
}
 
bool  util_alpha_dehors(double alpha)
{
	if (fabs(alpha) > M_PI)
		return 1;
	else
		return 0;
} 

bool  util_point_dans_cercle(S2D a, C2D c)
{
	double distance;
	distance = util_distance (a , c.centre);
	if (distance < (c.rayon - EPSIL_ZERO))
		return 1;
	else
		return 0;
}

bool  util_collision_cercle(C2D a, C2D b, double * p_dist)
{
	*p_dist = util_distance (a.centre , b.centre);
	if (*p_dist < a.rayon + b.rayon)
		return 1;
	else 
		return 0;
}

S2D util_deplacement(S2D p, double alpha, double dist)
{
	p.x += dist*cos(alpha);
	p.y += dist*sin(alpha);
	return p;
}

bool util_ecart_angle(S2D a, double alpha, S2D b, double *p_ecart_angle)
{
	double dist = util_distance(a,b);
	double * alpha_p = &alpha;
	int token;
	util_range_angle(alpha_p);
	if(dist > EPSIL_ZERO)
	{
		*p_ecart_angle = util_angle(a,b) - alpha;
		if(fabs(*p_ecart_angle) > M_PI)
		{
			if (*p_ecart_angle > 0)
				*p_ecart_angle = 2*M_PI - *p_ecart_angle;
			else
				*p_ecart_angle = 2*M_PI + *p_ecart_angle;
		}
		util_range_angle(p_ecart_angle);
		return 1;
	}
	else
		return 0;
}

bool util_alignement(S2D a, double alpha, S2D b)
{
	if(util_distance(a,b) > EPSIL_ZERO && (fabs(util_angle(a,b) - alpha) < EPSIL_ALIGNEMENT))
		return 1;
	else
		return 0;
}

bool util_inner_triangle(double la, double lb, double lc, double lb_new, double * p_la_new)
{
		if(la > EPSIL_ZERO && lc > EPSIL_ZERO && lb > EPSIL_ZERO && lb_new <= lc && lb_new >= lb)
		{
			double cosbeta;
			double discriminant;
			cosbeta = (la*la + lc*lc - lb*lb) / (2*la*lc);
			discriminant = 4*(lc*lc*cosbeta*cosbeta - lc*lc + lb_new*lb_new);
			*p_la_new = (2*lc*cosbeta - sqrt(discriminant))/2;
			return 1;
		}
		else
		{
			return 0;
		}
}
