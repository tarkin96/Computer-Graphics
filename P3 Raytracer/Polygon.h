/*	This class represents an entire polygon and holds the edge table for
 * 	each polygon. This class is not used and is here only for future reference
 * 	and for if I have time to implement the edge tables.
 * 
 */

#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <list>
//represents a point (defunct)
struct Point {
	float x = 0.0, y = 0.0, z = 0.0, w = 1.0;
};

//represents the original positions of the object
struct Vertex {
	float x = 0.0, y = 0.0, z = 0.0, w = 1.0;
};

//tracks texture coordinates
struct TexCoord {
	float u = 0.0, v = 0.0, w = 0.0;
};

//vertex normals
struct VNorm {
	float x = 0.0, y = 0.0, z = 0.0;
};

//polygon and list of vertices in the polygon
struct Face {
	std::vector<std::vector<unsigned int>> vals;
	float r = 0.9, g = 0.1, b = 0.1;
	float shiny = 10; 
	float ambient = 0.6, diffuse = 0.2, specular = 0.4;
};

//values in an edge table, unimplemented, defunct with raytracer
struct EdgeInfo {
	int yup = 0, xlo = 0, inversem = 0;
};
//defunct with raytracer
class Polygon {
	public:
		void makeEdgeTable(std::vector<std::vector<unsigned int>>, std::vector<Vertex>);
	private:
		std::vector<std::list<EdgeInfo>> edgeTable;
};



#endif
