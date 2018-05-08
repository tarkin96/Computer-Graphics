/*	This class handles all drawing capabilties for the program.
 * 	Given the object handler, it will loop through all objects and
 * 	draw them based on vertex information
 * 
 */


#include <vector>
#include <algorithm>
#include <utility>
#include "ObjHandle.h"
#include "Camera.h"
#include "Matrix.h"
class Renderer {
		public: 
			void render(const Camera&) const;		//draws everything
			int getWireFrame();	
			void setWireFrame(int);
			void setSmoothing();					//swaps smoothing technique
			void Orthonormal() const;					//not used
			void drawEdges(const Face&, const Transform&, const std::vector<Transform>&) const; 	//draws edges of objects (wireframe)
			void drawWireFrame(Face, std::vector<Transform>) const;						//DEFUNCT: used to draw wireframe, no longer used
			//void fillPolygon(Face, std::vector<Vertex>);
			void fillPolygon(const Face&, const Transform&, const std::vector<Transform>&) const;//fills polygons in an object
			
			//NEW RAYTRACE STUFF
			void rayTrace(const std::vector<Face>&, const Camera&, const std::vector<Transform>&, const std::vector<Transform>&) const;
			std::pair<std::pair<float, int>, std::pair<float, float>> checkBaryCentric(const Face&, const Transform&, const Transform&, const std::vector<Transform>&) const;
			const Transform createRay(float, float, float, const Transform&) const;
			const Transform normalize(const Transform&) const;
			
			//LIGHTING STUFF
			bool inShadow(const Transform&, const Transform&, const Transform&, const Face&, const std::vector<Face>&, const std::vector<Transform>&) const;
			Transform phongShade(const PointLight&, const Transform&, const Face&, const Transform&, const Transform&) const;
			Transform phongSmooth(float, float, const Face&, const std::vector<Face>&, const std::vector<Transform>&) const;
			
			float find_minVertex(Face, std::vector<Vertex>, int);						//not implemented
			int * sort_Intersections(int *);											//not implemented
			Matrix matrix;																//handles matrix and transform math
		private:
			int wireframe;//0=wireframe, 1=fill, tells whether to display only wireframe or to fill
			bool raytrace = true; //set to true when using the ray tracing algorithm
			bool smoothing = false; //tells whether to use phong smoothing or not
};
