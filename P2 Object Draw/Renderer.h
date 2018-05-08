/*	This class handles all drawing capabilties for the program.
 * 	Given the object handler, it will loop through all objects and
 * 	draw them based on vertex information
 * 
 */


#include <vector>
#include <algorithm>
#include "ObjHandle.h"
#include "Camera.h"
#include "Matrix.h"
class Renderer {
		public: 
			void render(ObjHandle&, Camera) const;		//draws everything
			int getWireFrame();	
			void setWireFrame(int);
			void Orthonormal() const;					//not used
			void drawEdges(Face, Transform, std::vector<Transform>, ObjHandle&) const; 	//draws edges of objects (wireframe)
			void drawWireFrame(Face, std::vector<Transform>) const;						//DEFUNCT: used to draw wireframe, no longer used
			//void fillPolygon(Face, std::vector<Vertex>);
			void fillPolygon(Face, Transform, std::vector<Transform>, ObjHandle&) const;//fills polygons in an object
			float find_minVertex(Face, std::vector<Vertex>, int);						//not implemented
			int * sort_Intersections(int *);											//not implemented
			Matrix matrix;																//handles matrix and transform math
		private:
			int wireframe;//0=wireframe, 1=fill, tells whether to display only wireframe or to fill
	
};
