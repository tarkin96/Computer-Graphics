/*	This class handles all matrix and transform calculations.
 * 	Currently implements scale, rotate, translate, and matrix
 * 	multiplication. 
 * 
 */

#ifndef MATRIX_H
#define MATRIX_H
#include "Transform.h"
#include <cmath>
#include <stdlib.h>

class Matrix {
	public:
		Transform fscale(Transform, float);
		Transform tscale(Transform, Transform);
		Transform translate(Transform, Transform);
		Transform rotatex(Transform, float);
		Transform rotatey(Transform, float);
		Transform rotatez(Transform, float);
		Transform matrixmult(double**, Transform);
		
/*		Transform scale(Vertex, Transform);
		Transform translate(Vertex, Transform);
		Transform rotate(Vertex, Transform);*/
	private:
	
};
#endif
