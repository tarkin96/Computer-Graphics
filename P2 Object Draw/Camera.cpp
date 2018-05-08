#include "Camera.h"

/*Camera::Camera(Point pos, int v) : position(pos), view(v) {
	
}*/

Camera::Camera(float pos, int v) : position(pos), rotation(0.0), view(v) {}

Camera::Camera(float pos, float rot) : position(pos), rotation(rot), view(0) {}

Camera::Camera(float pos, float rot, int v) : position(pos), rotation(rot), view(v) {}

Camera::Camera(float posx, float posy, float rot, int v) : position(posx, posy), rotation(rot), view(v) {}

Camera::Camera(float posx, float posy, float posz, float rot, int v) : position(posx, posy, posz), rotation(rot), view(v) {}

Transform Camera::getPosition() {
	return position;
}
