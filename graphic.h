/*------------------------------------------------------------------------*/
/*                          fichier en-tete (graphic.h)                   */
/*------------------------------------------------------------------------*/

#ifndef GRAPHIC_H
#define GRAPHIC_H

#define GRAPHIC_EMPTY  0
#define GRAPHIC_FILLED 1


/*=================================================================*/
/* Fonctions pour afficher un segment, un rectangle ou un cercle.  */
/*								   */
/* Pour le segment, on fournit les coordonnnees des deux extremites*/
/*                                                                 */
/* Pour le rectangle, on fournit les coordonnees de son centre et  */
/* de sa taille (width, height).                                   */
/*                                                                 */
/* Pour le cercle, on fournit les coordonnees de son centre et son */
/* rayon.                                                          */
/*                                                                 */
/* Le rectangle et le cercle peuvent etre creux ou pleins (filled) */
/* suivant la valeur du dernier argument (GRAPHIC_FILLED ou bien   */
/* GRAPHIC_EMPTY).                                              */
/*=================================================================*/

void       graphic_draw_segment (float x1,
                                 float y1,
                                 float x2, 
                                 float y2);

void       graphic_draw_rectangle (float xc,
			           float yc, 
			           float width,
                                   float height,
                                   int   filled);

void       graphic_draw_circle (float xc,
                                float yc,
			        float r,
                                int   filled);

void       graphic_set_color3f(float r,
                             float g, 
                             float b);

void       graphic_set_color3fv(float color[3]);


void       graphic_set_line_width(float width);

#endif
