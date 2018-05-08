/* Wade King
 * CPSC 4050
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include "Curve.h"

#define WIDTH 500
#define HEIGHT 500

int x_last,y_last;
int did_calc = 0; //tracks if the calculations for the bezier curve have been done
bool do_anim = false; //tracks whether an animation should be performed
Curve newcurve; //Class representing all the info for a bezier curve

//initializes the window
void init_window()
/* Clear the image area, and set up the coordinate system */
{

/* Clear the window */
        glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
        glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
}

//turns on a single pixel on the screen
void write_pixel(int x, int y, double intensity)
                                         /* Turn on the pixel found at x,y */
{

        glColor3f (intensity, intensity, intensity);                 
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();	
}

//manages all displayed items each frame
void display ( void )  //called every frame
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen 

    //writes each of the control points to the screen each frame
	write_pixel(newcurve.getPoint(0).x, newcurve.getPoint(0).y,1.0);
	write_pixel(newcurve.getPoint(1).x, newcurve.getPoint(1).y,1.0);
	write_pixel(newcurve.getPoint(2).x, newcurve.getPoint(2).y,1.0);
	write_pixel(newcurve.getPoint(3).x, newcurve.getPoint(3).y,1.0);
	
	//perform the animation for the bezier curve
	if (do_anim == true) {
		do_anim = newcurve.animation(newcurve.getold_init_point(), newcurve.getnew_init_point());
		did_calc = 0;
	}
	
	if (newcurve.getState() == 4) { //checks if 4 control points have been selected
		if (did_calc == 0) { //checks if calculation for the bezier curve is required
			//get Bernstein constants and calculate points on the curve
			newcurve.generateBernstein();
			newcurve.makeCurvePoints();
			did_calc = 1;
		}
		int xchange = 0, ychange = 0;
		//connect each point on the curve with a line
		for (int i = 0; i < newcurve.num_points; i++) {
			double current = 0.0;
			write_pixel(round(newcurve.getCurvePoint(i, 0)), round(newcurve.getCurvePoint(i, 1)),1.0);
			int dx = round(newcurve.getCurvePoint(i + 1, 0) - newcurve.getCurvePoint(i, 0));
			int dy = round(newcurve.getCurvePoint(i + 1, 1) - newcurve.getCurvePoint(i, 1));
			//check if if x or y is the default increment by 1
			if (dx > 0) {xchange = 1;} else {xchange = -1;}
			if (dy > 0) {ychange = 1;} else {ychange = -1;}
			if (fabs(dx) > fabs(dy)) { //decrement x by 1, y by dy/dx
				double increment = (double)abs(dy) / (double)abs(dx);
				current = newcurve.getCurvePoint(i, 1);
				for (int pix = round(newcurve.getCurvePoint(i, 0)); pix != round(newcurve.getCurvePoint(i + 1, 0)); pix += xchange) {
					if (ychange == 1) {
						current += increment;
					}
					else {
						current -= increment;
					}
					write_pixel(pix,round(current) ,1.0);
				}
			}
			else { //decrement y by 1, x by dx/dy
				double increment = (double)abs(dx) / (double)abs(dy);
				current = newcurve.getCurvePoint(i, 0);
				for (int pix = round(newcurve.getCurvePoint(i, 1)); pix != round(newcurve.getCurvePoint(i + 1, 1)); pix += ychange) {
					if (xchange == 1) {
						current += increment;
					}
					else {
						current -= increment;
					}
					write_pixel(round(current), pix ,1.0);
				}
			}
		}
	}


	glutSwapBuffers();                                      // Draw Frame Buffer 
}

/***************************************************************************/
void mouse(int, int, int x, int y)
{
	static int point_count = 0;
	static int oldx = 0;
	static int oldy = 0;
	int mag;
	if (point_count < 4) {	//get new control points at start
		y *= -1;  //align y with mouse
		y += 500; //ignore 
		mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
		if (mag > 20) {
			newcurve.setPoint(point_count, x, y);
			point_count++;
			newcurve.setState(point_count);
			oldx = x;
			oldy = y;
		}
		else {oldx = 0; oldy = 0;}
	}
	else { //do this after 4 control points are found
		y *= -1;  //align y with mouse
		y += 500; //ignore 
		mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
		if (mag > 20) {
			if (newcurve.selectControlPoint(x, y) >= 0) {
				/*this statement allows one to select a control point
				without activating an animation*/
				
			}
			else if (newcurve.selectControlPoint(x, y) == -1 && newcurve.getpoint_selected() != -1 && do_anim == false){
				/*do this if there is no point currently selected or 
				 there is no animation currently playing*/
				newcurve.setnew_init_point(x, y); //change the selected control point
				//set a reference for where the old control point was
				newcurve.setold_init_point(newcurve.getPoint(newcurve.getpoint_selected())); 
				std::cout << "Moved control point to: (" << newcurve.getnew_init_point().x << ", " << newcurve.getnew_init_point().y << ")" << std::endl;
				did_calc = 0; //get new bezier calculations
				do_anim = true; //start the animation and get its start time
				gettimeofday(&(newcurve.start), NULL); 
			}
			oldx = x;
			oldy = y;
		}
		else { oldx = 0; oldy = 0;}
	}
	
}
 
/***************************************************************************/
void keyboard ( unsigned char key, int, int )  // Create Keyboard Function
{

	switch ( key ) {
		case 27:              // When Escape Is Pressed...
			exit ( 0 );   // Exit The Program
			break;        
			case '1':             // stub for new screen
				printf("New screen\n");
			break;
		default:       
			break;
	}
}
/***************************************************************************/

int main (int argc, char *argv[])
{
/* This main function sets up the main loop of the program and continues the
   loop until the end of the data is reached.  Then the window can be closed
   using the escape key.						  */
	//Curve newCurve();
	
	glutInit            ( &argc, argv ); 
		glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
	glutInitWindowSize  ( 500,500 ); 
	glutCreateWindow    ( "Computer Graphics" ); 
	glutDisplayFunc     ( display );  
	glutIdleFunc	    ( display );
	glutMouseFunc       ( mouse );
	glutKeyboardFunc    ( keyboard );
								  
		init_window();				             //create_window
									
	glutMainLoop        ( );                 // Initialize The Main Loop
}


