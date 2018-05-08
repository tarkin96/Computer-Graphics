/*	This class handles the parsing of obj files into objects. It reads the file
 * 	token by token until it finds an important token. It then gets the 
 * 	line from the file and adds info to the object.
 * 
 */

#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
void Parser::read(char* filename, Object& object) {
	std::string token;
	std::string garbage; //holds information that is not implemented
	std::ifstream objfile(filename);
	while (objfile >> token) { //loop through whole file
		if (token.compare("#") == 0) { //if comment
			std::string comment;
			getline(objfile, comment); 
		}
		else if (token.compare("g") == 0) { //if group name
			getline(objfile, garbage);
			//std::cout << garbage << std::endl;
		}
		else if (token.compare("v") == 0) { //if vertex position
			std::string vertex;
			getline(objfile, vertex);
			//std::cout << vertex << std::endl;
			object.addVertex(vertex);
			
		}
		else if (token.compare("vt") == 0) { //if texture coordinate
			std::string texcoord;
			getline(objfile, texcoord);
			//std::cout << texcoord << std::endl;
			object.addTexCoord(texcoord);
		}
		else if (token.compare("vn") == 0) { //if vertex normal
			//getline(objfile, garbage); 
			std::string norm;
			getline(objfile, norm);
			//std::cout << norm << std::endl;
			object.addVNorm(norm);
		}
		else if (token.compare("f") == 0) { //if face
			//getline(objfile, garbage); 
			std::string face;
			getline(objfile, face);
			//std::cout << face << std::endl;
			object.addFace(face);
		}
		else if (token.compare("s") == 0) { //if surface
			getline(objfile, garbage);
			//std::cout << garbage << std::endl;
		}
		else if (token.compare("mtllib") == 0) { //if library
			getline(objfile, garbage);
			//std::cout << garbage << std::endl;
		}
		else if (token.compare("usemtl") == 0) { //if use library
			getline(objfile, garbage);
			//std::cout << garbage << std::endl;
		}
		else { 
			std::cout << "read something weird" << std::endl; //if it sees somethign unexpected
			//getline(objfile, garbage);
			//ignore this stuff
		}
		//std::cout << token << std::endl;
	}
	object.makeFaceNorms(); //create the initial surface normals from the vertex positions
	//object.printObj();
}
