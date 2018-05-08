/*	This class handles all matrix and transform calculations.
 * 	Currently implements scale, rotate, translate, and matrix
 * 	multiplication. 
 * 
 */

#ifndef MATRIX_H
#define MATRIX_H
#include "Transform.h"
//#include <cmath>
#include <stdlib.h>

class Matrix {
	public:
		Transform fscale(Transform, float) const;
		Transform tscale(Transform, Transform) const;
		Transform translate(Transform, Transform) const;
		Transform rotatex(Transform, float) const;
		Transform rotatey(Transform, float) const;
		Transform rotatez(Transform, float) const;
		Transform matrixmult(double**, Transform) const;
		
		//DETERMINANT STUFF
		float threeDet(const Transform&, const Transform&, const Transform&) const;
		Transform normalize(const Transform&) const;
		float dotProd(const Transform&, const Transform&) const;
		
/*		Transform scale(Vertex, Transform);
		Transform translate(Vertex, Transform);
		Transform rotate(Vertex, Transform);*/
	private:
	
};
#endif
