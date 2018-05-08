#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include "parser.h"
#include "ObjHandle.h"
//#include "Object.h"
#include "Camera.h"
#include "Renderer.h"

#define WIDTH 500
#define HEIGHT 500

int x_last,y_last;
//GLHandle GLHandler = GLHandle::getInstance();
ObjHandle& objhandler = ObjHandle::getInstance(); //create handler for all objects
Renderer renderer; //create renderer
Camera camera((float)WIDTH / 2, (float)HEIGHT / 2, 250.0, 0, Transform(0, 0, 0, 0)); //set up a camera
//Camera camera(0.0, 0.0, -250.0, 0, Transform(0, 0, 0, 0)); //set up a camera
/***************************************************************************/

void init_window()
                 /* Clear the image area, and set up the coordinate system */
{
	
        					       /* Clear the window */
        glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
        glOrtho(0,500,0,500,-1.0,1.0);
}

/***************************************************************************/
//THIS HAS BEEN MOVED TO THE RENDERER.CPP FILE!!!!!!!!!!!!!
/*void write_pixel(int x, int y, double intensity)
                                          //Turn on the pixel found at x,y 
{

        glColor3f (intensity, intensity, intensity);                 
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();	
}*/

//***************************************************************************/

void display ( void )   // Create The Display Function
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen 

  //write_pixel(x_last,y_last,1.0);//<-you can get rid of this call if you like
  // CALL YOUR CODE HERE
	
	renderer.render(camera);//renderer handles drawing, so tell it to draw the object
	//objhandler.draw();

  glutSwapBuffers();                                      // Draw Frame Buffer 
}

/***************************************************************************/
void mouse(int /*button*/, int /*state*/, int x, int y)
{
/* This function I finessed a bit, the value of the printed x,y should
   match the screen, also it remembers where the old value was to avoid multiple
   readings from the same mouse click.  This can cause problems when trying to
   start a line or curve where the last one ended */
        static int oldx = 0;
        static int oldy = 0;
	int mag;
	
	y *= -1;  //align y with mouse
	y += 500; //ignore 
	mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
	if (mag > 20) {
		printf(" x,y is (%d,%d)\n", x,y);
	}
	oldx = x;
	oldy = y;
	x_last = x;
	y_last = y;
}
 
/***************************************************************************/
void keyboard ( unsigned char key, int /*x*/, int /*y*/ )  // Create Keyboard Function
{

	switch ( key ) {
		case 'a':
			objhandler.transform(0);
			break;
		case 'd':
			objhandler.transform(1);
			break;
		case 's':
			objhandler.transform(2);
			break;
		case 'w':
			//switch between wireframe and polygon-fill
			if (objhandler.getTransformType() == 0) {
				renderer.setWireFrame((renderer.getWireFrame() + 1) % 2);
			}
			objhandler.transform(3);
			break;
		case 't':
			//set to translate, otherwise disable translation
			if (objhandler.getTransformType() != 1) {
				objhandler.setTransformType(1);
			}
			else {objhandler.setTransformType(0);}
			break;
		case 'r':
			//set to rotate, otherwise disable rotation
			if (objhandler.getTransformType() != 2) {
				objhandler.setTransformType(2);
			}
			else {objhandler.setTransformType(0);}
			break;	
		case 'e':
			//set to scale, otherwise disable scaling
			if (objhandler.getTransformType() != 3) {
				objhandler.setTransformType(3);
			}
			else {objhandler.setTransformType(0);}
			break;
		case 'x':
			renderer.setSmoothing();
			break;
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
   
   //check if a file was passed in
   	if (argc == 1) {
		std::cout << "Please give a file to read from in the argument." << std::endl;
		return 0;
	}
	
	objhandler.add(argv[1], camera); //add object to scene
	objhandler.createZbuff(WIDTH, HEIGHT); //create a zbuffer
	//add a light above and to the right of the camera
	objhandler.addLight(Transform(camera.getPosition().getX() + 500, camera.getPosition().getY() + 200, camera.getPosition().getZ()),
		Transform(0.9, 0.1, 0.2), 
		Transform(255, 255, 0), 
		Transform(255, 255, 0), 
		Transform(255, 255, 255));
	//add a light at the camera	
	objhandler.addLight(Transform(camera.getPosition().getX(), camera.getPosition().getY(), camera.getPosition().getZ()),
		Transform(0.9, 0.1, 0.2), 
		Transform(255, 0, 0), 
		Transform(255, 0, 0), 
		Transform(255, 255, 255));
	
	
	glutInit            ( &argc, argv ); 
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
	glutInitWindowSize  ( WIDTH, HEIGHT ); 
	glutCreateWindow    ( "Computer Graphics" ); 
	glutDisplayFunc     ( display );  
	glutIdleFunc	    ( display );
	glutMouseFunc       ( mouse );
	glutKeyboardFunc    ( keyboard );
  					      
        init_window();				             //create_window
						       		
	glutMainLoop        ( );                 // Initialize The Main Loop

}
