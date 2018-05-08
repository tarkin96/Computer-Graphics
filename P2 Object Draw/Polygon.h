/*	This class represents an entire polygon and holds the edge table for
 * 	each polygon. This class is not used and is here only for future reference
 * 	and for if I have time to implement the edge tables.
 * 
 */

#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <list>
struct Point {
	float x = 0.0, y = 0.0, z = 0.0, w = 1.0;
};

struct Vertex {
	float x = 0.0, y = 0.0, z = 0.0, w = 1.0;
};

struct TexCoord {
	float u = 0.0, v = 0.0, w = 0.0;
};

struct VNorm {
	float x = 0.0, y = 0.0, z = 0.0;
};

struct Face {
	std::vector<std::vector<unsigned int>> vals;
};

struct EdgeInfo {
	int yup = 0, xlo = 0, inversem = 0;
};

class Polygon {
	public:
		void makeEdgeTable(std::vector<std::vector<unsigned int>>, std::vector<Vertex>);
	private:
		std::vector<std::list<EdgeInfo>> edgeTable;
};



#endif
