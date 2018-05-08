#include <vector>
#include <iostream>
#include "ObjHandle.h"


ObjHandle::ObjHandle() : transform_type(0) {
	//std::cout << "new objhandler created" << std::endl;
}

//adds object to the scene
void ObjHandle::add(char* filename, Camera camera) {
	Object newobject;
	parser.read(filename, newobject); //read obj file
	fit(newobject); //fit object on screen
	center(newobject, camera); //center object on camera
	newobject.makeFaceNorms();
	objects.push_back(newobject);
}

std::vector<Object>& ObjHandle::getObjects() {
	return objects;
}

ObjHandle& ObjHandle::getInstance() {
  static ObjHandle instance;
  return instance;
}

void ObjHandle::draw() {
	for (Object obj : objects) {
		//obj.draw();
	}
}

int ObjHandle::getTransformType() {
	return transform_type;
}

void ObjHandle::setTransformType(int type) {
	transform_type = type;
	std::cout << "transform set to " << transform_type << std::endl;
}

//centers the object on a transform (usually the camera)
void ObjHandle::center(Object& obj, Camera& camera) {
	std::vector<Vertex> centerVer(obj.getVertexPos());
	//std::vector<Face> centerVerFaces(obj.getFaces());
	float xavg = 0.0, yavg = 0.0, zavg = 0.0, wavg = 0.0;
	
	//Transform center;
	for (auto& v: centerVer) {
		xavg += v.x;
		yavg += v.y;
		zavg += v.z;
		wavg += v.w;
	}
	Transform center(xavg / centerVer.size(), yavg / centerVer.size(), zavg / centerVer.size(), wavg / centerVer.size());
	obj.setCenter(center);
	
	obj.setPosition(matrix.translate(center, matrix.fscale(center, -1.0)));
	
	for (auto& t: obj.getTransformPos()) {
		std::cout << t << std::endl;
		t = matrix.translate(t, matrix.fscale(center, -1.0));
		t = matrix.translate(t, camera.getPosition());
		t = matrix.translate(t, Transform(0, 0, -350.0f, 1.0));
		std::cout << t << std::endl;
		
	}
	
	obj.setPosition(matrix.translate(obj.getPosition(), camera.getPosition()));
	obj.setPosition(matrix.translate(obj.getPosition(), Transform(0, 0, -350.0f, 1.0)));
	obj.printObj();
}

//fits camera within window, if does not fit, scale until largest dimension is 400 pixels
void ObjHandle::fit(Object& obj) {
	std::vector<Transform> transforms(obj.getTransformPos());
	//find largest dimension
	float xmin = transforms[0].getX();
	float xmax = transforms[0].getX();
	float ymin = transforms[0].getY();
	float ymax = transforms[0].getY();
	float zmin = transforms[0].getZ();
	float zmax = transforms[0].getZ();
	float largest_dim;
	float smallest_dim;
	for (auto& t : transforms) {
		if (t.getX() < xmin) {
			xmin = t.getX();
		}
		if (t.getX() > xmax) {
			xmax = t.getX();
		}
		if (t.getY() < ymin) {
			ymin = t.getY();
		}
		if (t.getY() > ymax) {
			ymax = t.getY();
		}	
		if (t.getZ() < zmin) {
			zmin = t.getZ();
		}
		if (t.getZ() > zmax) {
			zmax = t.getZ();
		}
	}
	if (xmax - xmin > ymax - ymin) {
		largest_dim = xmax - xmin;
	}
	else {largest_dim = ymax - ymin;}
	if (largest_dim < zmax - zmin) {
		largest_dim = zmax - zmin;
	}
	
	if (xmax - xmin < ymax - ymin) {
		smallest_dim = xmax - xmin;
	}
	else {smallest_dim = ymax - ymin;}
	if (smallest_dim > zmax - zmin) {
		smallest_dim = zmax - zmin;
	}
	//std::cout << "largest dim " << largest_dim << std::endl;
	//scale object if largest dimension is greater than 400 pixels
	if (largest_dim > 400) {
		for (auto& t: transforms) {
			t = matrix.translate(t, matrix.fscale(obj.getPosition(), -1.0));
			t = matrix.fscale(t, 400.0 / largest_dim);	
			t = matrix.translate(t, obj.getPosition());
			//std::cout << '(' << t.getX() << ", " << t.getY() << ", " << 
				//t.getZ() << ", " << t.getW() << ")" << std::endl;
		}
		
	}
	else if (largest_dim < 20) {
		for (auto& t: transforms) {
			t = matrix.translate(t, matrix.fscale(obj.getPosition(), -1.0));
			t = matrix.fscale(t, 100000.0 / largest_dim);	
			t = matrix.translate(t, obj.getPosition());
		}
	}

}


//THIS STUFF IS FOR THE Z-BUFFER
//create a new zbuffer
void ObjHandle::createZbuff(int x, int y) {
	xsize = x; ysize = y;
	//Zbuff.reserve(x);
	Zbuff.resize(x);
	for (auto& v : Zbuff) {
		v.resize(y);
	}
	float zmin = objects[0].getTransformPos()[0].getZ();
	//find intitial starting value of each zvalue
	for(auto& obj : objects) {
		std::vector<Transform> transforms(obj.getTransformPos());
		//zmin = transforms[0].getZ();
		float objmin = transforms[0].getZ();
		for (auto& t : transforms) {
			
			if (t.getZ() < objmin) {
				objmin = t.getZ();
				
			}			
		}
		if (zmin > objmin) {zmin = objmin;}
	}
	
	for (auto& xpos : Zbuff) {
		for (auto& ypos : xpos) {
			ypos = -10000000000000;
		}
	}
	//std::cout << zmin << std::endl;
}

//checks single pixel and z value, replace z value if necessary
bool ObjHandle::checkZbuff(int x, int y, float z) {
	if ((unsigned int)x >= Zbuff.capacity() || (unsigned int) y >= Zbuff[0].capacity()) {return false;}
	//std::cout << z << std::endl;
	if (z >= Zbuff[x][y] || fabs(z - Zbuff[x][y]) < 0.01) {
		if (z > Zbuff[x][y]) {
			Zbuff[x][y] = z;
		}
		return true;
	}
	else {/*std::cout << "z failed: " << z << " and " << Zbuff[x][y] << std::endl; */return false;}
}

std::vector<std::vector<float>>& ObjHandle::getZbuff() {
	return Zbuff;
	
}

//print entire z buffer
void ObjHandle::printZbuff() {
	for (auto& xpos : Zbuff) {
		for (auto& ypos : xpos) {
			if (ypos != -94) {
				//std::cout << ypos << std::endl;
			}
			
		}
	}
}

//THIS STUFF HANDLES TRANSFORMATIONS
//start transformation based on keyboard input
void ObjHandle::transform(int val) { //0 = a, 1 = d, 2 = s, 3 = w
	if (val == 0) {
		if (transform_type == 1) {
			Transform move(trans_speed, 0.0, 0.0, 0.0);		//translate in -x direction
			translate(matrix.fscale(move, -1.0));
		}
		else if (transform_type == 2) {
			float theta = -1.0 * rot_speed;					//rotate clockwise around x axis
			rotate(theta, 1);
		}
		else if (transform_type == 3) {
			//Transform scalar(1.0 - scale_speed, 1.0, 1.0, 1.0);		//decrease scale along x direction
			Transform scalar(1.0 - scale_speed, 1.0 - scale_speed, 1.0 - scale_speed, 1.0); //scale down
			scale(scalar);
		}
	}
	else if (val == 1) {
		if (transform_type == 1) {
			Transform move(trans_speed, 0.0, 0.0, 0.0);		//translate in +x direction
			translate(move);
		}
		else if (transform_type == 2) {
			float theta = rot_speed;
			rotate(theta, 1);								//rotate counterclockwise around x axis
		}
		else if (transform_type == 3) {
			//Transform scalar(1.0 + scale_speed, 1.0, 1.0, 1.0);		//increase scale along x direction
			Transform scalar(1.0 - scale_speed, 1.0 - scale_speed, 1.0 - scale_speed, 1.0); //scale down
			scale(scalar);
		}
	}
	else if (val == 2) {
		if (transform_type == 1) {
			Transform move(0.0, trans_speed, 0.0, 0.0);		//translate in -y direction
			translate(matrix.fscale(move, -1.0));
		}
		else if (transform_type == 2) {
			float theta = -1.0 * rot_speed;					//rotate clockwise around y axis
			rotate(theta, 0);
		}
		else if (transform_type == 3) {
			//Transform scalar(1.0, 1.0 - scale_speed, 1.0, 1.0);		//decrease scale along y direction
			Transform scalar(1.0 + scale_speed, 1.0 + scale_speed, 1.0 + scale_speed, 1.0); //scale up
			scale(scalar);			
		}
	}
	else if (val == 3) {
		if (transform_type == 1) {
			Transform move(0.0, trans_speed, 0.0, 0.0);		//translate in +y direction
			translate(move);
		}
		else if (transform_type == 2) {
			float theta = rot_speed;
			rotate(theta, 0);								//rotate counterclockwise around y axis
		}
		else if (transform_type == 3) {
			//Transform scalar(1.0, 1.0 + scale_speed, 1.0, 1.0);		//increase scale along y direction
			Transform scalar(1.0 + scale_speed, 1.0 + scale_speed, 1.0 + scale_speed, 1.0); //scale up
			scale(scalar);			
		}
	}
	createZbuff(500, 500);
}

//translate object
void ObjHandle::translate(Transform t) {
	for (auto& obj : objects) {
		for (auto& transform : obj.getTransformPos()) {
			transform = matrix.translate(transform, t);
		}
		
		//COMMENT THIS OUT TO ROTATE AROUND AXIS
		obj.setPosition(matrix.translate(obj.getPosition(), t));
	}
}

//scale object
void ObjHandle::scale(Transform t) {
	for (auto& obj : objects) {
		for (auto& transform : obj.getTransformPos()) {
			transform = matrix.translate(transform, matrix.fscale(obj.getPosition(), -1.0));
			transform = matrix.tscale(transform, t);
			transform = matrix.translate(transform, obj.getPosition());
		}
		obj.makeFaceNorms();
		//SCALING NORMALS IS TOO HARD!!!!!!!!!
		//for (auto& fnorm : obj.getFNorms()) {
			//transform = matrix.translate(transform, matrix.fscale(obj.getPosition(), -1.0));
			//fnorm = matrix.tscale(fnorm, t);
			//transform = matrix.translate(transform, obj.getPosition());
		//}
	}
}

//rotate object
void ObjHandle::rotate(float theta, int axis) { //0 = x, 1 = y, 2 = z
	if (axis == 0) {
		for (auto& obj : objects) {
			for (auto& transform : obj.getTransformPos()) {
				transform = matrix.translate(transform, matrix.fscale(obj.getPosition(), -1.0));		//rotate around x axis
				transform = matrix.rotatex(transform, theta);
				transform = matrix.translate(transform, obj.getPosition());
			}
			obj.makeFaceNorms();
		}
		
	}
	else if (axis == 1) {
		for (auto& obj : objects) {
			for (auto& transform : obj.getTransformPos()) {
				transform = matrix.translate(transform, matrix.fscale(obj.getPosition(), -1.0));		//rotate around y axis
				transform = matrix.rotatey(transform, theta);
				transform = matrix.translate(transform, obj.getPosition());
			}
			obj.makeFaceNorms();
		}
	}
	else if (axis == 2) {
		for (auto& obj : objects) {
			for (auto& transform : obj.getTransformPos()) {
				transform = matrix.translate(transform, matrix.fscale(obj.getPosition(), -1.0));		//rotate around z axis (not used)
				transform = matrix.rotatez(transform, theta);
				transform = matrix.translate(transform, obj.getPosition());
			}
			obj.makeFaceNorms();
		}
	}

}

//LIGHTING STUFF
//this function adds a light to the scene
void ObjHandle::addLight(const Transform& pos, const Transform& col, const Transform& am, const Transform& dif, const Transform& spec) {
	pointlights.push_back(PointLight(pos, col, am, dif, spec));
	
}

//get all the point lights from the scene
const std::vector<PointLight>& ObjHandle::getPointLights() const {
	return pointlights;
}
