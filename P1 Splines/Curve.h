#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>

#include <GL/glut.h> 

/* a struct representing a single point on the curve*/
struct Point {
	int x;
	int y;
	
};
 /* a class reprsenting functionality for a single curve*/
class Curve {
	public:
		//constructors and destructors
		Curve();
		~Curve();
		//getters and setters
		Point getPoint(int);
		void setPoint(int, int, int);
		int getState();
		void setState(int);
		int getpoint_selected();
		void setpoint_selected(int);
		Point getnew_init_point();
		void setnew_init_point(int, int);
		Point getold_init_point();
		void setold_init_point(Point);
		int getCurvePoint(int, int);
		//other functions
		void generateBernstein();
		void makeCurvePoints();
		void drawCurve();
		int selectControlPoint(int, int); 
		bool animation(Point, Point);
		int num_points = 2000; //number of points on the bezier curve
		struct timeval stop, start; //tracks time for animation
	private:
		int state; //current state of the program (number of control points)
		int point_selected = -1; //tracks point selected for moving
		Point init_points[4]; //the control points
		Point new_init_point;
		Point old_init_point;
		double ci[4]; //Bernstein constants
		float **curve_points; //every point on the curve
		double anim_time = 2.0; //time for each animation

		
};

