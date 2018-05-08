#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Transform.h"

Transform::Transform() : x(0.0), y(0.0), z(0.0), w(1.0) {}

Transform::Transform(float val) : x(val), y(val), z(val), w(1.0) {}

Transform::Transform(float xval, float yval) : x(xval), y(yval), z(0.0), w(1.0) {}

Transform::Transform(float xval, float yval, float zval) : x(xval), y(yval), z(zval), w(1.0) {}

Transform::Transform(float xval, float yval, float zval, float wval) : x(xval), y(yval), z(zval), w(wval) {}

//assignment operator for transforms
Transform& Transform::operator=(const Transform& rhs) {
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	//std::cout << "did it!!!!" << std::endl;
	return *this;
	
}

//getters and setters
float Transform::getX() const {return x;}
float Transform::getY() const {return y;}
float Transform::getZ() const {return z;}
float Transform::getW() const {return w;}

void Transform::setX(float val) {x = val;} 
void Transform::setY(float val) {y = val;}
void Transform::setZ(float val) {z = val;} 
void Transform::setW(float val) {w = val;}
