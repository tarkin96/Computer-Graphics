#include "Matrix.h"

//scale by a float value in all dimensions, returns the new transform
Transform Matrix::fscale(Transform lhs, float scalar) const {
	Transform ret;
	ret.setX(lhs.getX() * scalar);
	ret.setY(lhs.getY() * scalar);
	ret.setZ(lhs.getZ() * scalar);
	ret.setW(lhs.getW() * scalar);
	//lhs.setX(lhs.getX() * scalar);
	//lhs.setX(lhs.getX() * scalar);*/
	return ret;
}

//scale a transforms by another transform, returns the new transform
Transform Matrix::tscale(Transform lhs, Transform scalar) const {
	Transform ret;
	ret.setX(lhs.getX() * scalar.getX());
	ret.setY(lhs.getY() * scalar.getY());
	ret.setZ(lhs.getZ() * scalar.getZ());
	ret.setW(lhs.getW() * scalar.getW());
	//lhs.setX(lhs.getX() * scalar);
	//lhs.setX(lhs.getX() * scalar);*/
	return ret;
}

//translate a transform, returns the new transform
Transform Matrix::translate(Transform lhs, Transform rhs) const {
	Transform ret;
	ret.setX(lhs.getX() + rhs.getX());
	ret.setY(lhs.getY() + rhs.getY());
	ret.setZ(lhs.getZ() + rhs.getZ());
	ret.setW(lhs.getW() + rhs.getW());
	return ret;
}

//rotate around x axis, returns the new transform
Transform Matrix::rotatex(Transform lhs, float theta) const {
	Transform ret;
	double** matrix = (double**)malloc(4 * sizeof(double*)); // malloc an array representing a matrix
	for (int i = 0; i < 4; i++) {
		matrix[i] = (double*)malloc(4 * sizeof(double));
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = 0.0;
		}
	}
	matrix[0][0] = 1.0; 
	matrix[1][1] = cos(theta); matrix[1][2] = -1.0 * sin(theta);				//create matrix for rotation
	matrix[2][1] = sin(theta); matrix[2][2] = cos(theta); matrix[3][3] = 1.0;
	ret = matrixmult(matrix, lhs);
	for(int i = 0; i < 4; i++) {							//free the matrix array
			free(matrix[i]);
	}
	free(matrix);
	
	return ret;
}

//rotate around y axis, returns the new trasnform
Transform Matrix::rotatey(Transform lhs, float theta) const {
	Transform ret;
	double** matrix = (double**)malloc(4 * sizeof(double*));
	for (int i = 0; i < 4; i++) {
		matrix[i] = (double*)malloc(4 * sizeof(double));
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = 0.0;
		}
	}
	matrix[0][0] = cos(theta); matrix[0][2] = sin(theta); 
	matrix[1][1] = 1.0; matrix[2][0] = -1.0 * sin(theta);
	matrix[2][2] = cos(theta); matrix[3][3] = 1.0;
	ret = matrixmult(matrix, lhs);
	for(int i = 0; i < 4; i++) {
			free(matrix[i]);
	}
	free(matrix);
	return ret;
}

//rotate around z axis, returns the new transform
Transform Matrix::rotatez(Transform lhs, float theta) const {
	Transform ret;
	double** matrix = (double**)malloc(16 * sizeof(double));
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = 0.0;
		}
	}
	matrix[0][0] = cos(theta); matrix[0][1] = -1.0 * sin(theta); 
	matrix[1][0] = sin(theta); matrix[1][1] = cos(theta);
	matrix[3][3] = 1.0; matrix[2][2] = 1.0;
	ret = matrixmult(matrix, lhs);
	for(int i = 0; i < 4; i++) {
			free(matrix[i]);
	}
	free(matrix);
	return ret;
	
}

//perform matrix multiplication, for rotations, returns the new transform
Transform Matrix::matrixmult(double** matrix, Transform transform) const {
	Transform ret;
	//for (int i = 0; i < 4; i++) {
		ret.setX((transform.getX() * matrix[0][0]) + (transform.getY() * matrix[0][1]) + (transform.getZ() * matrix[0][2]) + (transform.getW() * matrix[0][3]));
		ret.setY((transform.getX() * matrix[1][0]) + (transform.getY() * matrix[1][1]) + (transform.getZ() * matrix[1][2]) + (transform.getW() * matrix[1][3]));
		ret.setZ((transform.getX() * matrix[2][0]) + (transform.getY() * matrix[2][1]) + (transform.getZ() * matrix[2][2]) + (transform.getW() * matrix[2][3]));
		ret.setW((transform.getX() * matrix[3][0]) + (transform.getY() * matrix[3][1]) + (transform.getZ() * matrix[3][2]) + (transform.getW() * matrix[3][3]));
	//}
	return ret;
}

//DETERMINANT STUFF!!!!!

/* I may try to implement a complete determinant function later on*/
//calculates the determinant of a 3x3 maxtrix (or 3 transforms)
float Matrix::threeDet(const Transform& first, const Transform& second, const Transform& third) const {
	float det = (first.getX() * second.getY() * third.getZ()) +
		(second.getX() * third.getY() * first.getZ()) +
		(third.getX() * first.getY() * second.getZ()) - 
		(third.getX() * second.getY() * first.getZ()) -
		(second.getX() * first.getY() * third.getZ()) -
		(first.getX() * third.getY() * second.getZ());
	
	//std::cout << det << std::endl;
	return det;
}

//normalizes a transform or vector
Transform Matrix::normalize(const Transform& transform) const {
	float dist = pow(transform.getX(), 2) + pow(transform.getY(), 2) + pow(transform.getZ(), 2);
	dist = pow(dist, 0.5);
	Transform newtransform(transform.getX() / dist, transform.getY() / dist, transform.getZ() / dist, 1.0);
	return newtransform;
}

//calculates the dot product of two transforms
float Matrix::dotProd(const Transform& left, const Transform& right) const {
	return ((left.getX() * right.getX()) + (left.getY() * right.getY()) + (left.getZ() * right.getZ()));
}
