#include "Camera.h"

/*Camera::Camera(Point pos, int v) : position(pos), view(v) {
	
}*/

//constructors for camera
Camera::Camera(float pos, int v) : position(pos), rotation(0.0), view(v) {}

Camera::Camera(float pos, float rot) : position(pos), rotation(rot), view(0) {}

Camera::Camera(float pos, float rot, int v) : position(pos), rotation(rot), view(v) {}

Camera::Camera(float posx, float posy, float rot, int v) : position(posx, posy), rotation(rot), view(v) {}

Camera::Camera(float posx, float posy, float posz, float rot, int v) : position(posx, posy, posz), rotation(rot), view(v) {}

Camera::Camera(float posx, float posy, float posz, int v, const Transform& rotate) :
	position(posx, posy, posz), rotation(rotate), view(v) {}

//getters and setters for camera
const Transform& Camera::getPosition() const {
	return position;
}

void Camera::setPosition(const Transform& pos){
	position = pos;
}

const Transform& Camera::getRotation() const {
	return rotation;
}

void Camera::setRotation(const Transform& rot) {
	rotation = rot;
}
