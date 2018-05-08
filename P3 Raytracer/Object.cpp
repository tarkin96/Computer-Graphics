#include "Object.h"

Object::Object() : position(0.0), scale(1.0), rotation(0.0) {
	//default constructor
}

std::vector<Vertex>& Object::getVertexPos() {
	return Vertices;
	
}

std::vector<Transform>& Object::getTransformPos() {
	return VPositions;
	
}

std::vector<Face>& Object::getFaces() {
		return Faces;
}

std::vector<VNorm>& Object::getVNorms() {
		return VNorms;
}

std::vector<Transform>& Object::getFNorms() {
		return FNorms;
}

Transform& Object::getPosition() {
		return position;
}

void Object::setPosition(Transform t) {
	position.setX(t.getX());
	position.setY(t.getY());
	position.setZ(t.getZ());
	position.setW(t.getW());
}

Transform& Object::getCenter() {
		return center;
}

void Object::setCenter(Transform t) {
	center.setX(t.getX());
	center.setY(t.getY());
	center.setZ(t.getZ());
	center.setW(t.getW());
}

//Print all the info about the object, used for testing
void Object::printObj() {
	std::vector<Vertex>::const_iterator Vptr = Vertices.begin();
	//std::cout << "im printing!!!" << std::endl;
	while (Vptr != Vertices.end()) {
		    std::cout << '(' << Vptr->x << ", " << Vptr->y << ", " << 
		    Vptr->z << ", " << Vptr->w << ")" << std::endl;
			++Vptr;
	}
	std::cout << "Showing transforms" << std::endl;
	std::vector<Transform>::const_iterator trptr = VPositions.begin();
	while (trptr != VPositions.end()) {
		    std::cout << '(' << trptr->getX() << ", " << trptr->getY() << ", " << 
		    trptr->getZ() << ", " << trptr->getW() << ")" << std::endl;
			++trptr;
	}
	std::vector<TexCoord>::const_iterator Tptr = TexCoords.begin();
	while (Tptr != TexCoords.end()) {
		    std::cout << '(' << Tptr->u << ", " << Tptr->v << ", " << 
		    Tptr->w << ")" << std::endl;
			++Tptr;
	}
	std::cout << "Showing VNorms" << std::endl;
	std::vector<VNorm>::const_iterator Nptr = VNorms.begin();
	while (Nptr != VNorms.end()) {
		    std::cout << '(' << Nptr->x << ", " << Nptr->y << ", " << 
		    Nptr->z << ")" << std::endl;
			++Nptr;
	}
	
	std::cout << "Showing Faces" << std::endl;
	std::vector<Face>::const_iterator Fptr = Faces.begin();
	while (Fptr != Faces.end()) {
		for (auto& set : Fptr->vals) {
			std::cout << set[0] << "/" << set[1] << "/" << 
				set[2] << " ";
		}
		/*for (int i = 0; i< 3; i++) {
			std::cout << Fptr->vals[i][0] << "/" << Fptr->vals[i][1] << "/" << 
		    Fptr->vals[i][2] << " ";
		}*/
		std::cout << std::endl;
		++Fptr;
	}
	
}

//add a vertex from the obj file to the object
void Object::addVertex(std::string vertex) {
	std::istringstream vertexstream(vertex);
	std::string vals;
	int i = 0;
	Vertex newvertex;
	Transform pos;
	while (vertexstream >> vals) {
		if (i == 0) {newvertex.x = atof(vals.c_str()); pos.setX(atof(vals.c_str()));}
		else if (i == 1) {newvertex.y = atof(vals.c_str()); pos.setY(atof(vals.c_str()));}
		else if (i == 2) {newvertex.z = atof(vals.c_str()); pos.setZ(atof(vals.c_str()));}
		else {newvertex.w = atof(vals.c_str()); pos.setW(atof(vals.c_str()));}
		//vals.str(std::string());
		//vals.clear();
		i = (i + 1) % 4;
	}
	Vertices.push_back(newvertex); //store original vertices
	VPositions.push_back(pos); //store vertices as temporary transforms
	//std::cout << pos << std::endl;
}

//add a texture coordinate from the obj file to the object, future proofing
void Object::addTexCoord(std::string texcoord) {
	std::istringstream texstream(texcoord);
	std::string vals;
	int i = 0;
	TexCoord newtex;
	while (texstream >> vals) {
		if (i == 0) {newtex.u = atoi(vals.c_str());}
		else if (i == 1) {newtex.v = atoi(vals.c_str());}
		else {newtex.w = atoi(vals.c_str());}
		//vals.str(std::string());
		//vals.clear();
		i = (i + 1) % 3;
	}
	TexCoords.push_back(newtex);
}

//add a vertex normal from obj file to object, originally used to calculate surface normals
void Object::addVNorm(std::string norm) {
	std::istringstream vnorm(norm);
	std::string vals;
	int i = 0;
	VNorm newnorm;
	while (vnorm >> vals) {
		//std::cout << vals << std::endl;
		if (i == 0) {newnorm.x = atof(vals.c_str());}
		else if (i == 1) {newnorm.y = atof(vals.c_str());}
		else if (i == 2) {newnorm.z = atof(vals.c_str());}
		//else {newvertex.w = atoi(vals.c_str());}
		//vals.str(std::string());
		//vals.clear();
		i = (i + 1) % 3;
	}
	VNorms.push_back(newnorm);
}

//add a face (polygon) from obj file to object
void Object::addFace(std::string face) {
	std::istringstream facef(face);
	std::string valset;
	std::stringstream slashsearch;
	std::string val;
	int i = 0;
	Face newface;
	while (facef >> valset) {
		//std::stringstream slashsearch;
		int j = 0;
		//std::cout << "valset: " << valset << std::endl;
		slashsearch << valset;
		newface.vals.push_back(std::vector<unsigned int>());
		//std::cout << "slashsearch: " << slashsearch.str() << std::endl;
		while (getline(slashsearch, val, '/')) {
			//std::cout << "i did it" << std::endl;
			newface.vals[i].push_back(atoi(val.c_str()));
			j++;
		}
		slashsearch.str(std::string());
		slashsearch.clear();
		i++;
	}
	//std::cout << "this is the face size: " << newface.vals.size() << std::endl;
	if (newface.vals.size() > 3) {
		//orderMesh(newface);
		triRemesh(newface);
	}
	else {
		Faces.push_back(newface); 
	}
	Polygon poly; //attempt to create a polygon (not implemented yet)
	//poly.makeEdgeTable(newface.vals, Vertices); // make edge table (not implemented yet)
	polygons.push_back(poly); //add polygon to the object (not implemented yet)
	
}

//use vertices to create surface normals for object
void Object::makeFaceNorms() {
	//std::cout << "Number of faces: " << Faces.size() << std::endl;
	FNorms.clear();
	std::cout << "printing fnorms" << std::endl;
	for(auto& face : Faces) {
		Transform facenorm;
		//create transforms for current edge of polygon
		Transform v1(VPositions[face.vals[0][0] - 1].getX() - VPositions[face.vals[1][0] - 1].getX(), VPositions[face.vals[0][0] - 1].getY() - VPositions[face.vals[1][0] - 1].getY(), VPositions[face.vals[0][0] - 1].getZ() - VPositions[face.vals[1][0] - 1].getZ());
		Transform v2(VPositions[face.vals[1][0] - 1].getX() - VPositions[face.vals[2][0] - 1].getX(), VPositions[face.vals[1][0] - 1].getY() - VPositions[face.vals[2][0] - 1].getY(), VPositions[face.vals[1][0] - 1].getZ() - VPositions[face.vals[2][0] - 1].getZ());
		//calculate vector cross product	
		facenorm.setX((v1.getY() * v2.getZ()) - (v1.getZ() * v2.getY()));
		facenorm.setY((v1.getZ() * v2.getX()) - (v1.getX() * v2.getZ()));
		facenorm.setZ((v1.getX() * v2.getY()) - (v1.getY() * v2.getX()));
		facenorm.setW(1.0); //set w as zero, because reasons
		
		//normalize cross product
		float dist = pow(facenorm.getX(), 2) + pow(facenorm.getY(), 2) + pow(facenorm.getZ(), 2);
		dist = pow(dist, 0.5);
		facenorm.setX(facenorm.getX() / dist);
		facenorm.setY(facenorm.getY() / dist);
		facenorm.setZ(facenorm.getZ() / dist);
		FNorms.push_back(facenorm);
		
		std::cout << facenorm << std::endl;
		//std::cout << "Face Norm : (" << facenorm.x << ", " << facenorm.y << ", " << 
		    //facenorm.z << ")" << std::endl;
	}
	
}

//old version of makign faces, defunct, saved in order to future-proof

/*void Object::makeFaceNorms() {
	std::cout << "Number of faces: " << Faces.size() << std::endl;
	for(auto& f : Faces) {
		Transform facenorm;
		for(auto& set : f.vals) {
			facenorm.setX(facenorm.getX() + VNorms[set[set.size() - 1] - 1].x);
			//std::cout << VNorms[set[set.size() - 1] - 1].x << std::endl;
			facenorm.setY(facenorm.getY() + VNorms[set[set.size() - 1] - 1].y);
			facenorm.setZ(facenorm.getZ() + VNorms[set[set.size() - 1] - 1].z);
		}
		facenorm.setX(facenorm.getX() / f.vals.size()); 
		facenorm.setY(facenorm.getY() / f.vals.size());
		facenorm.setZ(facenorm.getZ() / f.vals.size());
		facenorm.setW(1.0);
		
		FNorms.push_back(facenorm);
		//std::cout << "Face Norm : (" << facenorm.x << ", " << facenorm.y << ", " << 
		    //facenorm.z << ")" << std::endl;
	}
	
}*/


//MY REMESH CODE!!!!! (this is it for 20 points?)
void Object::triRemesh(const Face& complexPoly) {
	std::vector<std::vector<unsigned int>>::const_iterator Fptr = complexPoly.vals.begin();
	if (Fptr == complexPoly.vals.end()) {return;}
	Fptr++;
	while (Fptr+1 != complexPoly.vals.end()) {
		Face newface;
		newface.vals.push_back(complexPoly.vals[0]);
		newface.vals.push_back(*Fptr);
		Fptr++;
		newface.vals.push_back(*Fptr);
		//Fptr++;
		Faces.push_back(newface);
	}
}

