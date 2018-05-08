/*	This class represents the viewing camera. This holds the info about
 * 	the transform of the camera (origin), which is used when centering,
 * 	scaling, and rotating the object. 
 */

#ifndef CAMERA_H
#define CAMERA_H
#include "Object.h"

class Camera {
	public:
		Camera(Transform, int);
		//Camera(Point, int);
		Camera(float, int);
		Camera(float, float, int);
		Camera(float, float);
		Camera(float, float, float, int);
		Camera(float, float, float, float, int);
		Camera(float, float, float, int, const Transform&);
		const Transform& getPosition() const;
		void setPosition(const Transform&);
		const Transform& getRotation() const;
		void setRotation(const Transform&);		
		void center(Object);
	private:
		Transform position;		//transform holding the position of the camera
		Transform rotation;		//transform holding the current angle of the camera
		//view=0 -> orthonormal, view=1 -> perspective
		int view;				//keeps track of the type of view
};
#endif
