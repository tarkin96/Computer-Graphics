/*	This class represents a single object. It holds all the information
 * 	found in the obj file and the temporary transform values, which
 * 	change when you apply a transformation. 
 */
 #ifndef OBJECT_H
#define OBJECT_H
#include <stdio.h>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
//#include "Drawer.h"
#include "Polygon.h"
#include "Transform.h"



class Object {
		public: 
			Object();
			//void read(char *); defunct
			void addVertex(std::string);	// for adding parts to the
			void addTexCoord(std::string);  // object
			void addVNorm(std::string);     //
			void addFace(std::string);      //
			void draw();
			void printObj();                //prints object info
			void makeFaceNorms();           //calculate the surface normals
			void setPosition(Transform);
			Transform& getPosition();
			void setCenter(Transform);
			Transform& getCenter();
			std::vector<Vertex>& getVertexPos();	
			std::vector<Transform>& getTransformPos();	
			std::vector<Face>& getFaces();	
			std::vector<VNorm>& getVNorms();	
			std::vector<Transform>& getFNorms();	
		private:
			std::vector<Vertex> Vertices;	//holds all original vertex positions for object
			std::vector<Transform> VPositions;	//holds temporary transform positions for object
			std::vector<TexCoord> TexCoords;	//hold the texture coordinates for object (not used)
			std::vector<VNorm> VNorms;	//holds vertex normals for object (I NO LONGER USE THESE)
			std::vector<Transform> FNorms;	//holds the surface normals calculated using vertex positions
			std::vector<Face> Faces;	//holds face information for object
			std::vector<Polygon> polygons;      //not currently used
			Transform position;	//current average position of object
			Transform center;	//center of object
			Transform scale;	//current scale
			Transform rotation;	//current rotation
			
			//std::vector<std::list<EdgeInfo>> edgeTable;
			//Drawer drawer;
};
#endif
