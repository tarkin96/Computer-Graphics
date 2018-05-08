#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "Curve.h"

//does factorial calculations
int factorial (int num) {
	int total = 1;
	if (num == 0 || num == 1) {return total;}
	for (int i = num; i > 1; i--) {
		total = total * i;
	}
	return total;
}

//insitialize control points and bezier curve points
Curve::Curve()
{
	init_points[0].x = -1; init_points[0].y = -1; 
	init_points[1].x = -1; init_points[1].y = -1; 
	init_points[2].x = -1; init_points[2].y = -1; 
	init_points[3].x = -1; init_points[3].y = -1; 
	
	curve_points = (float**)malloc(sizeof(float*) * (num_points + 2));
	for (int i = 0; i < num_points + 2; i++) {
		curve_points[i] = (float*)malloc(sizeof(float) * 2);
	}
}

Curve::~Curve() {
	for (int n = 0; n < num_points + 2; n++) {
		free(curve_points[n]);
	}
	free(curve_points);
}

//returns the desired control point
Point Curve::getPoint(int index) {
	Point rtnpoint;
	if (index < 4) {
		rtnpoint.x = init_points[index].x;
		rtnpoint.y = init_points[index].y;
		return rtnpoint;
	}
	else {
		rtnpoint.x = -1;
		rtnpoint.y = -1;
		std::cout << "point does not exist" << std::endl; return rtnpoint; }
}

//sets the desired control point
void Curve::setPoint(int index, int newx, int newy) {
	if (index < 4) {
		init_points[index].x = newx;
		init_points[index].y = newy;
	}
	else {
		std::cout << "point does not exist" << std::endl;
	}
}

//get and set the state
int Curve::getState() {
	return state;
}

void Curve::setState(int num) {
	state = num;
}

//get and set the currently selected control point
int Curve::getpoint_selected () {
	return point_selected;
}

void Curve::setpoint_selected (int val) {
	point_selected = val;
}

//get and set goal control point during animation
Point Curve::getnew_init_point() {
	return new_init_point;
}

void Curve::setnew_init_point(int x, int y) {
	new_init_point.x = x; 
	new_init_point.y = y;
}

//get and set old control point during animation
Point Curve::getold_init_point() {
	return old_init_point;
}

void Curve::setold_init_point(Point point) {
	old_init_point.x = point.x; 
	old_init_point.y = point.y;
}

//gets and sets desired bexier curve point
int Curve::getCurvePoint(int point, int dim) {
	return curve_points[point][dim];
}

void Curve::generateBernstein() {
	for (int i = 0; i < 4; i++) {
		ci[i] = (double)factorial(3) / (double)(factorial(i) * factorial(3 - i));
	}
}

//calculates every point on the bezier curve
void Curve::makeCurvePoints() {
	double dt = 1.0 / num_points; //increment for u
	//set the endpoints of curve
	curve_points[0][0] = init_points[0].x; curve_points[0][1] = init_points[0].y;
	curve_points[num_points+1][0] = init_points[3].x; curve_points[num_points+1][1] = init_points[3].y;
	for (int n = 1; n < num_points + 2; n++) {
		double u = dt * n;
		curve_points[n][0] = 0;
		curve_points[n][1] = 0;
		//calculate each point based on location of control points
		for (int i = 0; i < 4; i++) {
			curve_points[n][0] += ci[i] * pow(u, i) * pow((1-u), (3 - i)) * init_points[i].x;
			curve_points[n][1] += ci[i] * pow(u, i) * pow((1-u), (3 - i)) * init_points[i].y;
		}
		
	}
}

//select a control point to animate
int Curve::selectControlPoint(int x, int y) {
	for (int i = 0; i < 4; i++) {
		//if control point is within 3 pixels of selection
		if(abs(x - init_points[i].x) <= 5 && abs(y - init_points[i].y) <= 5 && point_selected == -1) {
			point_selected = i;
			std::cout << "Selected control point at: (" << init_points[i].x << ", " << init_points[i].y << ")" << std::endl;
			return i;
		}
	}
	return -1;
}

bool Curve::animation(Point oldpoint, Point goalpoint) {
	//get current time to compare to start of animation
	gettimeofday(&stop, NULL);

	//perform one frame of animation with microsecond precision
	double starttime = start.tv_sec + (start.tv_usec / pow(10, 6));
	double stoptime = stop.tv_sec + (stop.tv_usec / pow(10, 6));
	if (stoptime - starttime > anim_time) {
		point_selected = -1;
		//stop animation
		return false;
	}
	else {
		//perform single step of animation
		double distancex = ((double)(goalpoint.x - oldpoint.x)) * ((stoptime - starttime) / anim_time);
		double distancey = ((double)(goalpoint.y - oldpoint.y)) * ((stoptime - starttime) / anim_time);
		
		init_points[point_selected].x = round(oldpoint.x + distancex);
		init_points[point_selected].y = round(oldpoint.y + distancey);
		//continue animation
		return true;
	}
}


