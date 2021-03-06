/*	This class is used to handle all the objects in the scene. It does all 
* 	z buffer calculations and handles "events" for transforming the object.
* 	This class is implemented as a singleton.
*/

#ifndef OBJECTHANDLE_H
#define OBJECTHANDLE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
//#include "Object.h"
#include "parser.h"
#include "Matrix.h"
#include "Camera.h"
#include "PointLight.h"

class ObjHandle {
	public:
		static ObjHandle& getInstance();
		int getTransformType();
		void setTransformType(int);
		ObjHandle();
		void add(char*, Camera);
		//void keyboard ( unsigned char, int, int );
		std::vector<Object>& getObjects();
		//void Init(int, char**);
		void center(Object&, Camera&); 	//centers object on camera
		void fit(Object&);					//scales object to fit in 500x500 window
		void draw();						//not used currently
		void createZbuff(int, int);			//create/recreate zbuffer
		bool checkZbuff(int, int, float);	//checks if pixel is shown and modifies zbuffer
		std::vector<std::vector<float>>& getZbuff();
		void printZbuff();					//prints all values in zbuffer, for testing
		void transform(int);				//starts transforming						
		void translate(Transform);			//translate object
		void scale(Transform);				//scale object
		void rotate(float, int);			//rotate object
		float trans_speed = 5.0;			//translate by 5 pixels
		float rot_speed = 0.2;				//rotate by .2 radians
		float scale_speed = 0.05;			//scale to 95% of current size
		int getXSize() const {return xsize;}
		int getYSize() const {return ysize;}
		
		bool checkBaryCentric(const Face&, const Transform&, const Transform&, const std::vector<Transform>&) const; 	//calculate barycentric coordinates and check if pixel is visible
		const Transform createRay(int, int, int, const Transform&) const;		//creates a ray between two transforms
		//const Transform normalize(const Transform&) const;
		
		//LIGHTING STUFF
		void addLight(const Transform&, const Transform&, const Transform&, const Transform&, const Transform&); //add a light to the scene
		const std::vector<PointLight>& getPointLights() const;		//get all lights from scene
		
	private:
		int transform_type; //0 = none, 1 = translate, 2 = rotate, 3 = scale
		std::vector<Object> objects;
		Parser parser;
		Point origin;
		Matrix matrix;
		int xsize, ysize;
		std::vector<std::vector<float>> Zbuff;
		
		//LIGHTING STUFF
		std::vector<PointLight> pointlights;
};
#endif
