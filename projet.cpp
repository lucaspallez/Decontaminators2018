extern "C" 
{
	#include <stdio.h>
	#include <string.h>
	#include <GL/glut.h>
	#include "simulation.h"
	#include "graphic.h"
}
#include <GL/glui.h>

#define WINDOW_SIZE			500
#define STRLEN_E			5
#define STRLEN_D			4
#define TEST_ERROR			0
#define TEST_DRAW			1
#define STR_MAX				64
#define PRTCL_E				0
#define PRTCL_R				1
#define PRTCL_XC			2
#define PRTCL_YC			3
#define RBT_XC				0
#define RBT_YC				1
#define RBT_ANGLE			2
#define LU					1
#define NON_LU				0


namespace
{
	char save_name[STR_MAX] = "save.txt";
	char file_name[STR_MAX] = "";
	int main_window, turn = 0, recording, control_type, control, fichier_lu = NON_LU;
	int nb_robots, nb_particules;
	double rate = 0., v_trans = 0., v_rot = 0.;
	STR_ROBOT **robot;
	STR_PARTICULE ** particule;
	
	GLfloat aspect_ratio;
	GLUI_Button *exit_button, *open_button, *save_button;
	GLUI_Button *sim_start_button, *sim_step_button;
	GLUI_Panel *open_panel, *save_panel, *sim_panel, *record_panel;
	GLUI_Panel *control_mode_panel, *robot_control_panel;
	GLUI_EditText *open_field, *save_field;
	GLUI_Checkbox *record_box;
	GLUI_StaticText *record_text_rate, *record_text_turn;
	GLUI_StaticText *robot_text_trans, *robot_text_rot;
	GLUI_RadioGroup *control_type_group;
}

void ouverture_fichier(void)
{
	simulation_ouverture_fichier(file_name);
	nb_robots = simulation_get_nb_robots();
	nb_particules = simulation_get_nb_particules();
	robot = simulation_get_robots();
	particule = simulation_get_particules();
	fichier_lu = LU;
}

void affichage(void)
{
	GLfloat gauche = -DMAX, droite = DMAX, bas = -DMAX, haut = DMAX;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	if(aspect_ratio <= 1.)
		glOrtho (gauche, droite, bas/aspect_ratio, haut/aspect_ratio, -1.0, 1.0);
	else
		glOrtho (gauche*aspect_ratio, droite*aspect_ratio, bas, haut, -1.0, 1.0);
	if(fichier_lu == LU)
	{
		//ROBOTS
		for(int i = 0; i < nb_robots; i++)
		{
			float black[3] = {0.0,0.0,0.0};
			float red[3] = {1.0,0.0,0.0};
			double x_c = robot[i]->pos_x;
			double y_c = robot[i]->pos_y;
			double angle = robot[i]->angle;
			graphic_set_color3fv(black);
			graphic_set_line_width(2.0);
			graphic_draw_circle (x_c, y_c, R_ROBOT, GRAPHIC_EMPTY);
			graphic_draw_segment(x_c, y_c, x_c + R_ROBOT*cos(angle),
								 y_c + R_ROBOT*sin(angle));
			graphic_set_color3fv(red);
			graphic_draw_rectangle(x_c, y_c, 0.15, 0.15, GRAPHIC_FILLED);
		}
		
		//PARTICULES
		for(int i = 0; i < nb_particules; i++)
		{
			double x_c = particule[i]->pos_x;
			double y_c = particule[i]->pos_y;
			double radius = particule[i]->rayon;
			float gray[3] = {0.7, 0.7,0.7};
			graphic_set_color3fv(gray);
			graphic_draw_circle(x_c, y_c, radius, GRAPHIC_FILLED);
		}
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	aspect_ratio = (GLfloat) w / (GLfloat) h;
	glutPostRedisplay();
}

void idle(void)
{
	if(glutGetWindow() != main_window)
		glutSetWindow(main_window);
	glutPostRedisplay();
}

void open_CB(int control)
{
	ouverture_fichier();
}

void save_CB(int control)
{
	FILE *save_ptr = fopen(save_name, "w");
	fprintf(save_ptr, "%d\n", nb_robots);
	for(int i = 0; i<nb_robots; i++)
	{
		fprintf(save_ptr, "%lf %lf %lf\n", robot[i]->pos_x, 
				robot[i]->pos_y, robot[i]->angle);
	}
	fprintf(save_ptr, "FIN_LISTE\n\n%d\n", nb_particules);
	for(int i = 0; i<nb_particules; i++)
	{
		fprintf(save_ptr,"%lf %lf %lf %lf\n", particule[i]->energie,
				particule[i]->rayon, 
				particule[i]->pos_x,
				particule[i]->pos_y);
	}
	fprintf(save_ptr, "FIN_LISTE\n");
	fclose(save_ptr);
}

void start_CB(int control)
{
	//START SIMULATION
}

void step_CB(int control)
{
	//NEXT STEP
}

void recording_CB(int control)
{
	if(recording == 1)
	{
		//RECORD
	}
}

void control_type_CB(int control)
{
	if(control_type_group->get_int_val() == 0)
	{
		//AUTOMATIC MODE
	}
	else if(control_type_group->get_int_val() == 1)
	{
		//MANUAL MODE
	}
}

void display_init(void)
{
	//GLUT init
	char * argv[1] = {(char*)" "};
	int argc = 1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_SIZE,WINDOW_SIZE);
	aspect_ratio = (GLfloat) 250/ (GLfloat) 250;
	main_window = glutCreateWindow("Decontaminators Display");
	
	glClearColor(1.0,1.0,1.0,0.0);
	
	glutDisplayFunc(affichage);
	glutReshapeFunc(reshape);
	GLUI_Master.set_glutIdleFunc(idle);
}

void gui_init(void)
{
	GLUI *glui = GLUI_Master.create_glui("Decontaminators - Control");
	
	//FIRST COLUMN
	open_panel = glui->add_panel("Opening");
	open_field = glui->add_edittext_to_panel(open_panel,"File name:",
											 GLUI_EDITTEXT_TEXT, file_name);
	open_field->set_text(file_name);
	
	open_button = glui->add_button_to_panel(open_panel,"Open",control,open_CB);
	
	save_panel = glui->add_panel("Saving");
	save_field = glui->add_edittext_to_panel(save_panel,"File name:", 
											 GLUI_EDITTEXT_TEXT, save_name);
	save_field->set_text("save.txt");
	
	save_button = glui->add_button_to_panel(save_panel,"Save",control,save_CB);
	
	glui->add_column();	//SECOND COLUMN

	sim_panel = glui->add_panel("Simulation");
	sim_start_button = glui->add_button_to_panel(sim_panel,"Start",control,start_CB);
	sim_step_button = glui->add_button_to_panel(sim_panel,"Step",control,step_CB);
	
	record_panel = glui->add_panel("Recording");
	record_box = glui->add_checkbox_to_panel(record_panel, "Record",
											 &recording,recording, recording_CB);
	
	record_text_rate = glui->add_statictext_to_panel(record_panel, "Rate: ");
	char str_rate[STR_MAX];
	sprintf(str_rate, "Rate: %.3lf", rate);
	record_text_rate->set_text(str_rate);
	
	record_text_turn = glui->add_statictext_to_panel(record_panel, "Turn: ");
	char str_turn[STR_MAX];
	sprintf(str_turn, "Turn: %d", turn);
	record_text_turn->set_text(str_turn);
	
	glui->add_column(); //THIRD COLUMN
	
	control_mode_panel = glui->add_panel("Control mode");
	control_type_group = glui->add_radiogroup_to_panel(control_mode_panel,
													   &control_type,
													   control, control_type_CB);
	glui->add_radiobutton_to_group(control_type_group, "Automatic");
	glui->add_radiobutton_to_group(control_type_group, "Manual");
	
	robot_control_panel = glui->add_panel("Robot Control");
	
	robot_text_trans = glui->add_statictext_to_panel(robot_control_panel,
													 "Translation: ");
	char str_trans[STR_MAX];
	sprintf(str_trans, "Translation: %.3lf", v_trans);
	robot_text_trans->set_text(str_trans);
	
	robot_text_rot = glui->add_statictext_to_panel(robot_control_panel, "Rotation: ");
	char str_rot[STR_MAX];
	sprintf(str_rot, "Rotation:     %.3lf", v_rot);
	robot_text_rot->set_text(str_rot);
	
	exit_button = glui->add_button("Exit", 0, (GLUI_Update_CB)exit);
	glui->set_main_gfx_window(main_window);
}

void draw(void)
{
	ouverture_fichier();
	display_init();
	gui_init();
	glutMainLoop();
}

void error(void)
{
	simulation_ouverture_fichier(file_name);
}

void no_argument(void)
{
	display_init();
	gui_init();
	glutMainLoop();
}

int main(int argc, char **argv)
{
	if(argc == 3)
	{
		strncpy(file_name, argv[2], strlen(argv[2]));
		const char test_e[STRLEN_E] = {'E','r','r','o','r'};
		const char test_d[STRLEN_D] = {'D','r','a','w'};
		
			if(strncmp(test_e,argv[1],strlen(argv[1])) == 0
			   && strlen(argv[1]) == STRLEN_E)
				error();
			else if(strncmp(test_d,argv[1],strlen(argv[1])) == 0 
					&& strlen(argv[1]) == STRLEN_D)				
				draw();
			else
				printf("Invalid use of this program!\nSyntax:"
					   " ./projet.x [Error|Draw, name_file]\n");
		}
	else if(argc == 1)
		no_argument();
	else
		printf("Invalid use of this program!\nSyntax:"
			   "./projet.x [Error|Draw, name_file]\n");
	return 0;
}