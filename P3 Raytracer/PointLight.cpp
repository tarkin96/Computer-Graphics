#include "PointLight.h"
//main contructor
PointLight::PointLight(const Transform& pos, const Transform& col, const Transform& am, const Transform& dif, const Transform& spec) :
	position(pos), color(col), ambient(am), diffuse(dif), specular(spec)
{
	
}

//getters and setters
Transform PointLight::getPosition() const {
	return position;
}

Transform PointLight::getColor() const {
	return color;
}

Transform PointLight::getAmbient() const {return ambient;}
Transform PointLight::getDiffuse() const {return diffuse;}
Transform PointLight::getSpecular() const {return specular;}

void PointLight::setPosition(const Transform& pos) {
	position = pos;
}

void PointLight::setColor(const Transform& col) {
	color = col;
}

void PointLight::setAmbient(const Transform& am) {
	ambient = am;
}
void PointLight::setDiffuse(const Transform& dif) {
	diffuse = dif;
}
void PointLight::setSpecular(const Transform& spec) {
	specular = spec;
}
