#include "Renderer.h"

//used with line drawing
void write_pixel(int x, int y, double intensity)
 //Turn on the pixel found at x,y 
{

        glColor3f (intensity, intensity, intensity);               
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();	
}

//used for raytracing to control color of pixels
void write_pixel(int x, int y, double r, double g, double b)
 //Turn on the pixel found at x,y with certain color
{

        glColor3f (r, g, b);               
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();	
}

//the main rendering loop that draws each object in the objhandler
void Renderer::render(const Camera& camera) const {
	ObjHandle& objhandler = ObjHandle::getInstance();
	for (auto& o : objhandler.getObjects()) {
		std::vector<Vertex> vertices(o.getVertexPos());
		std::vector<Transform> transforms(o.getTransformPos());
		std::vector<Transform> fnorms(o.getFNorms());
		std::vector<VNorm> vnorms(o.getVNorms());
		int i = 0;
		if (!raytrace) {
			for(auto& face: o.getFaces()) { //for each polygon
				if (wireframe == 1) { //check if renderer should fill or draw in wireframe
					fillPolygon(face, fnorms[i], transforms);
					drawEdges(face, fnorms[i], transforms);
				}
				else {
					//drawWireFrame(face, transforms);
					drawEdges(face, fnorms[i], transforms);
				}
				i++;
			}
		}
		else {
			//RAYTRACING STARTS HERE
			rayTrace(o.getFaces(), camera, transforms, fnorms);	
		}
	}
	//objhandler.printZbuff();
	
	
}

void Renderer::setWireFrame(int val) {//0=wireframe, 1=fill
	wireframe = val;	
}

//This is called when user presses x, changes 
void Renderer::setSmoothing() {
		if (smoothing) {smoothing = false;}
		else {smoothing = true;}
		std::cout << "smoothing is: " << smoothing << std::endl;
}

int Renderer::getWireFrame() {return wireframe;}

//DEFUNCT, drew in wireframe mode, kept for future reference
void Renderer::drawWireFrame(Face face, std::vector<Transform> transforms) const {
	int xchange = 0, ychange = 0;
	for (unsigned int i = 0; i < face.vals.size(); i++) {
		double current = 0.0;
		//write_pixel(round(vertices[face.vals[i][0] - 1].x), round(vertices[face.vals[i][0] - 1].y),1.0);
		int dx = round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX() - transforms[face.vals[i][0] - 1].getX());
		int dy = round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY() - transforms[face.vals[i][0] - 1].getY());
		//check if if x or y is the default increment by 1
		if (dx > 0) {xchange = 1;} else {xchange = -1;}
		if (dy > 0) {ychange = 1;} else {ychange = -1;}
		//if (dz > 0) {zchange = 1;} else {zchange = -1;}
		if (fabs(dx) > 0 && fabs(dy) == 0) {
			//float z = round(transforms[face.vals[i][0] - 1].getZ());
			for (int pix = round(transforms[face.vals[i][0] - 1].getX()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()); pix += xchange) {
				write_pixel(pix, round(transforms[face.vals[i][0] - 1].getY()), 0.4);
			}
		}
		else if (fabs(dy) > 0 && fabs(dx) == 0) {
			for (int pix = round(transforms[face.vals[i][0] - 1].getY()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY()); pix += ychange) {
				write_pixel(round(transforms[face.vals[i][0] - 1].getX()), pix ,0.4);
			}
		}
		else if (fabs(dx) >= fabs(dy)) { //decrement x by 1, y by dy/dx
			double increment = (double)abs(dy) / (double)abs(dx);
			current = transforms[face.vals[i][0] - 1].getY();
			for (int pix = round(transforms[face.vals[i][0] - 1].getX()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()); pix += xchange) {
				if (ychange == 1) {
					current += increment;
				}
				else {
					current -= increment;
				}
				write_pixel(pix,round(current) , 0.4);
			}
		}
		//IMPLEMENT ABILITY TO HANDLE DY AND DX OF 0
		else if (fabs(dx) < fabs(dy)) { //decrement y by 1, x by dx/dy
			double increment = (double)abs(dx) / (double)abs(dy);
			current = transforms[face.vals[i][0] - 1].getX();
			for (int pix = round(transforms[face.vals[i][0] - 1].getY()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY()); pix += ychange) {
				if (xchange == 1) {
					current += increment;
				}
				else {
					current -= increment;
				}
				write_pixel(round(current), pix , 0.4);
			}
		}
	}
	
}

//draws the edges of the object, draws wireframe when fill is not active
void Renderer::drawEdges(const Face& face, const Transform& fnorm, const std::vector<Transform>& transforms) const {
	ObjHandle& objhandler = ObjHandle::getInstance();
	int xchange = 0, ychange = 0, zchange = 0;
	for (unsigned int i = 0; i < face.vals.size(); i++) { //for each vertices in the polygon
		float current = 0.0;
		//write_pixel(round(vertices[face.vals[i][0] - 1].x), round(vertices[face.vals[i][0] - 1].y),1.0);
		float dx = transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX() - transforms[face.vals[i][0] - 1].getX();		//get change of x,y,z at each vertex
		float dy = transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY() - transforms[face.vals[i][0] - 1].getY();
		float dz = transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getZ() - transforms[face.vals[i][0] - 1].getZ();
		//calculate constant in plane function
		float d = -1.0 * ((fnorm.getX() * transforms[face.vals[0][0] - 1].getX()) + (fnorm.getY() * transforms[face.vals[0][0] - 1].getY()) + (fnorm.getZ() * transforms[face.vals[0][0] - 1].getZ()));
		//check if if x,y, and z is the default increment
		if (dx > 0) {xchange = 1;} else {xchange = -1;}
		if (dy > 0) {ychange = 1;} else {ychange = -1;}
		if (dz > 0) {zchange = 1;} else {zchange = -1;}
		if (fabs(dx) > 0 && fabs(dy) == 0) { //if horizontal line
			float z/* = transforms[face.vals[i][0] - 1].getZ()*/;
			//float zincrement = fabs(dz / dx);
			//std::cout << "dx changes" << std::endl;
			float y = round(transforms[face.vals[i][0] - 1].getY());
			for (int pix = round(transforms[face.vals[i][0] - 1].getX()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()); pix += xchange) {
				z = ((pix * (-1.0 * fnorm.getX())) + (y * (-1.0 * fnorm.getY())) - d) / fnorm.getZ(); //get z value for pixel
				if (objhandler.checkZbuff(pix, y, z)) {	//chekc zbuffer
					write_pixel(pix, y, 0.4);
					
				}
				//if (zchange == 1) {z += zincrement;}
				//else {z -= zincrement;}
			}
		}
		else if (fabs(dy) > 0 && fabs(dx) == 0) {	//if vertical line
			float z/* = transforms[face.vals[i][0] - 1].getZ()*/;
			//float zincrement = fabs(dz / dy);
			//std::cout << "dy changes" << std::endl;
			float x = round(transforms[face.vals[i][0] - 1].getX());
			for (int pix = round(transforms[face.vals[i][0] - 1].getY()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY()); pix += ychange) {
				z = ((x * (-1.0 * fnorm.getX())) + (pix * (-1.0 * fnorm.getY())) - d) / fnorm.getZ(); //find z value
				if (objhandler.checkZbuff(x, pix, z)) { //check zbuffer
					write_pixel(x, pix, 0.4);
				}
				//if (zchange == 1) {z += zincrement;}
				//else {z -= zincrement;}
			}
		}
		else if (fabs(dx) >= fabs(dy)) { //x of line grows faster
			float z/* = transforms[face.vals[i][0] - 1].getZ()*/;
			float increment = fabs(dy / dx);
			//float zincrement = fabs(dz / dx);
			//get slope and yintercept to create line equation
			float slope = dy / dx;
			float yintercept = transforms[face.vals[i][0] - 1].getY() - (transforms[face.vals[i][0] - 1].getX() * slope);
			current = (round(transforms[face.vals[i][0] - 1].getX()) * slope) + yintercept;
			//std::cout << "dy is greater than dx" << std::endl;
			for (int pix = round(transforms[face.vals[i][0] - 1].getX()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()); pix += xchange) {
				z = ((pix * (-1.0 * fnorm.getX())) + (round(current) * (-1.0 * fnorm.getY())) - d) / fnorm.getZ(); //get z value
				if (objhandler.checkZbuff(pix, round(current), z)) { //check zbuffer
					write_pixel(pix,round(current) , 0.4);
				}
				if (ychange == 1) {
					current += increment;
				}
				else {
					current -= increment;
				}
				//if (zchange == 1) {z += zincrement;}
				//else {z -= zincrement;}

			}
		}
		else if (fabs(dx) < fabs(dy)) { //y of line grows faster
			float z/* = transforms[face.vals[i][0] - 1].getZ()*/;
			float increment = fabs(dx / dy);
			//float zincrement = fabs(dz / dy);
			//calculate slope and yintercept to get line equation
			float slope = dy / dx;
			float yintercept = transforms[face.vals[i][0] - 1].getY() - (transforms[face.vals[i][0] - 1].getX() * slope);
			current = (round(transforms[face.vals[i][0] - 1].getY()) - yintercept) / slope;
			//std::cout << "dx is greater than dy" << std::endl;
			for (int pix = round(transforms[face.vals[i][0] - 1].getY()); pix != round(transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY()); pix += ychange) {
				z = ((round(current) * (-1.0 * fnorm.getX())) + (pix * (-1.0 * fnorm.getY())) - d) / fnorm.getZ(); //get zvalue
				if (objhandler.checkZbuff(round(current), pix, z)) { //check zbuffer
					write_pixel(round(current), pix, 0.4);
				}
				if (xchange == 1) {
					current += increment;
				}
				else {
					current -= increment;
				}
				//if (zchange == 1) {z += zincrement;}
				//else {z -= zincrement;}

			}
		}
	}
	
}

//this fills in the polygons with a white color
void Renderer::fillPolygon(const Face& face, const Transform& fnorm, const std::vector<Transform>& transforms) const {
	ObjHandle& objhandler = ObjHandle::getInstance();
	//find min x, max x, min y, max y, min z, max z
	float xmin = transforms[face.vals[0][0] - 1].getX();
	float xmax = transforms[face.vals[0][0] - 1].getX();
	float ymin = transforms[face.vals[0][0] - 1].getY();
	float ymax = transforms[face.vals[0][0] - 1].getY();
	float zmin = transforms[face.vals[0][0] - 1].getZ();
	float zmax = transforms[face.vals[0][0] - 1].getZ();
	for (unsigned int i = 1; i < face.vals.size(); i++) {
		if (transforms[face.vals[i][0] - 1].getX() < xmin) {
			xmin = transforms[face.vals[i][0] - 1].getX();
		}
		if (transforms[face.vals[i][0] - 1].getX() > xmax) {
			xmax = transforms[face.vals[i][0] - 1].getX();
		}
		if (transforms[face.vals[i][0] - 1].getY() < ymin) {
			ymin = transforms[face.vals[i][0] - 1].getY();
		}
		if (transforms[face.vals[i][0] - 1].getY() > ymax) {
			ymax = transforms[face.vals[i][0] - 1].getY();
		}	
		if (transforms[face.vals[i][0] - 1].getZ() < zmin) {
			zmin = transforms[face.vals[i][0] - 1].getZ();
		}
		if (transforms[face.vals[i][0] - 1].getZ() > zmax) {
			zmax = transforms[face.vals[i][0] - 1].getZ();
		}
	}
	
	if (ymax > 500) {ymax = 500;}
	if (ymin < 0) {ymin = 0;}
	//std::cout << xmin << ", " << xmax << ", " << ymin << ", " << ymax << ", " << zmin << ", " << zmax << std::endl;
	for(int y = round(ymin) + 1; y < round(ymax); y++) { //for every y value in the object
		std::vector<int> intersections; //create a vector to store the x values of each possible intersection
		int intersection;
		float fintersection;
		for (unsigned int i = 0; i < face.vals.size(); i++) { //for each vertices in the polygon
			float dx = transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX() - transforms[face.vals[i][0] - 1].getX();
			float dy = transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY() - transforms[face.vals[i][0] - 1].getY();
			//float dz = transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getZ() - transforms[face.vals[i][0] - 1].getZ();
			if (dy == 0) { //if horizontal line
				//do nothing
				
			}	
			else if (dx != 0) { //if both x and y change
				//get line equation info
				float slope = dy / dx;
				float yintercept = transforms[face.vals[i][0] - 1].getY() - (transforms[face.vals[i][0] - 1].getX() * slope);
				//calculate intersection on line in float and integer form
				fintersection = (y - yintercept) / slope;
				intersection = round(fintersection);
				//std::cout << intersection << std::endl;
				
				//check if intersection is at a vertex
				if (fintersection == transforms[face.vals[i][0] - 1].getX()) { //if intersection occurs at vertex
					float otherdy = transforms[face.vals[i][0] - 1].getY() - transforms[face.vals[(i-1) % face.vals.size()][0] - 1].getY();
					if (dy * otherdy >= 0.0) { //check if change in y changes sign at intersection
						if (fintersection >= xmin && fintersection <= xmax) { //is x value in range of polygon?
							intersections.push_back(intersection); 
						}
					}					
				}
				
				//DEFUNCT: kept for future reference
				/*else if ( fintersection == transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()) {
					//float otherdx = vertices[face.vals[(i+1) % face.vals.size()][0] - 1].x - vertices[face.vals[(i+2) % face.vals.size()][0] - 1].x;
					float otherdy = transforms[face.vals[(i+2) % face.vals.size()][0] - 1].getY() - transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY();
					if (dy * otherdy >= 0.0) {
						if (fintersection >= xmin && fintersection <= xmax) {std::cout << "entered here2" << std::endl;
							intersections.push_back(intersection);
							//intersections.push_back(intersection);
						}
					}

				}*/
				
				else { //if intersection does not occur at vertex
					if (dx > 0.0) { //if x increases on line
						if (fintersection > transforms[face.vals[i][0] - 1].getX() && fintersection < transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()) {
							intersections.push_back(intersection);
							//std::cout << "entered here2" << std::endl;
						}
					}
					else if (dx < 0.0) { //if x decreases on line
						if (fintersection < transforms[face.vals[i][0] - 1].getX() && fintersection > transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getX()) {
							intersections.push_back(intersection);
							//std::cout << "entered here3" << std::endl;
						}
					}
				}
				//std::cout << "intersect: " << fintersection << std::endl;
			}
			else if (dx == 0) { // if line is vertical
				//if (transforms[face.vals[i][0] - 1].getX() >= xmin && transforms[face.vals[i][0] - 1].getX() <= xmax) { //if line is in bounds of polygon, useless?
					if (dy > 0.0) { //if x increases on line
						if (transforms[face.vals[i][0] - 1].getY() <= y && transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY() >= y) { //if y is within range of line
							intersections.push_back(round(transforms[face.vals[i][0] - 1].getX()));
						}
					}
					else if (dy < 0.0) { //if x descreases on line
						if (transforms[face.vals[i][0] - 1].getY() >= y && transforms[face.vals[(i+1) % face.vals.size()][0] - 1].getY() <= y) { //if y is within range of line
							intersections.push_back(round(transforms[face.vals[i][0] - 1].getX()));
						}
					}
					
					
				//}
			}
			
			//else {intersections[i] = round(vertices[face.vals[i][0] - 1].x);}
		}
		//calculate constant in plane equation
		float d = -1.0 * ((fnorm.getX() * transforms[face.vals[0][0] - 1].getX()) + (fnorm.getY() * transforms[face.vals[0][0] - 1].getY()) + (fnorm.getZ() * transforms[face.vals[0][0] - 1].getZ()));
		//make sure intersectiosn occurred
		if (intersections.size() != 0) {
			std::sort(intersections.begin(), intersections.end()); //sort the intersections by smallest x value
			if (intersections[0] < 500 && intersections[intersections.size() - 1] > 0) { //cull if intersections are off screen
				if (intersections[0] < 0) {intersections[0] = 0;} //ensure draw starts at x >= 0
				if (intersections[intersections.size() - 1] > 500) {  //ensure draw ends at x <= 500
					intersections[intersections.size() - 1] = 500;
					}
				int intersect = 1;
				//float zincrement = (dz / dx);
				if (fnorm.getZ() != 0) { 	//make sure plane is visible from front
					for (int x = intersections[0] + 1; x < intersections[intersections.size() - 1]; x++) {
						float z;
						z = ((x * (-1.0 * fnorm.getX())) + (y * (-1.0 * fnorm.getY())) - d) / fnorm.getZ(); //calculate z value
						//std::cout << z << std::endl;
						if (intersect % 2 == 1) { //check if pixel is between intersections
							if (objhandler.checkZbuff(x, y, z)) { //check z buffer
								write_pixel(x, y ,1.0);
							}	
						}
						if (x >= intersections[intersect]) { //check if intersection is encountered, if so, turn off pixels until next intersect is found
							intersect++;
						}
					}
				}
			}
		}
	}
	
}

//DEFUNCT: kept for future reference
/*float Renderer::find_minVertex(Face face, std::vector<Vertex> vertices, int dim) {
	float min = 0.0;
	for (auto& v : face.vals) {
		
	}
	return 0.0;
}
//DEFUNCT: kept for future reference
int * sort_Intersections(int * intersections) {
	return intersections;
}*/




//NEW RAYTRACING STUFF!!!!!!!!
void Renderer::rayTrace(const std::vector<Face>& polygons, const Camera& camera, const std::vector<Transform>& transforms, const std::vector<Transform>& fnorms) const {
	std::vector<float> detT_values; //vector to keep track of det(t) values
	int i = 0;
	for (auto& polygon : polygons) {
		//calculate det(t) value for polygon, speeds up program
		Transform eb(transforms[polygon.vals[1][0] - 1]);
		Transform ec(transforms[polygon.vals[2][0] - 1]);
		Transform origin(transforms[polygon.vals[0][0] - 1]);
		
		eb = matrix.translate(eb, matrix.fscale(origin, -1.0f));
		ec = matrix.translate(ec, matrix.fscale(origin, -1.0f));
		
		float t = matrix.threeDet(matrix.fscale(eb, -1.0f), matrix.fscale(ec, -1.0f), 
			matrix.translate(origin, matrix.fscale(camera.getPosition(), -1.0f)));
			
		detT_values.push_back(t);

		ObjHandle& objhandler = ObjHandle::getInstance();
		//for every pixel
		for (int x = 0; x < objhandler.getXSize(); x++) {
			for (int y = 0; y < objhandler.getYSize(); y++) {
				Transform ray(createRay(x, y, -200.0, camera.getPosition()));	//shoot ray to pixel plane
				//get barycentric values
				std::pair<std::pair<float, int>, std::pair<float, float>> baryresults = checkBaryCentric(polygon, ray, camera.getPosition(), transforms);
				//calculate xyz coordinate of intersection with polygon
				Transform polygonInter(camera.getPosition().getX() + (ray.getX() * baryresults.first.first),
					camera.getPosition().getY() + (ray.getY() * baryresults.first.first),
					camera.getPosition().getZ() + (ray.getZ() * baryresults.first.first));
				//fill the polygon
				if (baryresults.first.second == 1) {
					if (objhandler.checkZbuff(x, y, -baryresults.first.first)) { //check z buffer
						Transform totalcolor; float numLights = 0; //create color that will hold average color of lights
						for (PointLight light : objhandler.getPointLights()) {
							numLights++;
							//create ray between polygon and light
							Transform lightray(createRay(light.getPosition().getX(), light.getPosition().getY(), light.getPosition().getZ(), polygonInter));
							lightray = normalize(lightray);
							if (!inShadow(light.getPosition(), lightray, polygonInter, polygon, polygons, transforms)) { //no shadow on object
								if (smoothing) { //if smoothign is enabled
									//calculate new normal from phong smoothing and light at pixel
									Transform phongNorm(phongSmooth(baryresults.second.first, baryresults.second.second, polygon, polygons, fnorms));
									Transform color(matrix.fscale(phongShade(light, lightray, polygon, phongNorm, matrix.fscale(ray, -1.0f)), 1.0/255.0));
									totalcolor = matrix.translate(totalcolor, color);
								}
								else {
									//calculate color without phong smoothing
									Transform color(matrix.fscale(phongShade(light, lightray, polygon, fnorms[i], matrix.fscale(ray, -1.0f)), 1.0/255.0));
									totalcolor = matrix.translate(totalcolor, color);
								}
							}
							else {
								if (smoothing) {
									//calculate new normal from phong smoothing and light at pixel
									Transform phongNorm(phongSmooth(baryresults.second.first, baryresults.second.second, polygon, polygons, fnorms));
									Transform color(matrix.fscale(phongShade(light, lightray, polygon, phongNorm, matrix.fscale(ray, -1.0f)), 1.0/255.0));
									totalcolor = matrix.translate(totalcolor, color);
								}
								else {
									//calculate color without phong smoothing
									Transform color(matrix.fscale(phongShade(light, lightray, polygon, fnorms[i], matrix.fscale(ray, -1.0f)), 1.0/255.0));
									totalcolor = matrix.translate(totalcolor, matrix.fscale(color, 0.7));
								}
							}
						}
						//calculate average color of all lighting on pixel
						totalcolor = matrix.fscale(totalcolor, 1.0/numLights);
						write_pixel(x, y, totalcolor.getX(), totalcolor.getY(), totalcolor.getZ());		
					}
				}
				//draw an edge for testing
				else if (baryresults.first.second == 0) {
					if (objhandler.checkZbuff(x, y, -baryresults.first.first)) {
						for (PointLight light : objhandler.getPointLights()) {
							write_pixel(x, y, 0.3, 0.5, 0.9);
						}
					}
				}
			}
		}
		i++;		
	}
}
		
//this handles barycentric coordinate intersections
std::pair<std::pair<float, int>, std::pair<float, float>> Renderer::checkBaryCentric(const Face& polygon, const Transform& ray, const Transform& camera, const std::vector<Transform>& transforms) const {
	//std::pair<float, int> ret;
	//calculate beta, gamma, alpha, detA, and t value for intersection
	Transform origin(transforms[polygon.vals[0][0] - 1]);
	Transform eb(transforms[polygon.vals[1][0] - 1]);
	Transform ec(transforms[polygon.vals[2][0] - 1]);
	
	eb = matrix.translate(eb, matrix.fscale(origin, -1.0f));
	ec = matrix.translate(ec, matrix.fscale(origin, -1.0f));
	
	float detA = matrix.threeDet(matrix.fscale(eb, -1.0f), matrix.fscale(ec, -1.0f), ray);
	float t = matrix.threeDet(matrix.fscale(eb, -1.0f), matrix.fscale(ec, -1.0f), matrix.translate(origin, matrix.fscale(camera, -1.0f))) / detA;
	float beta = matrix.threeDet(matrix.translate(origin, matrix.fscale(camera, -1.0f)), matrix.fscale(ec, -1.0f), ray) / detA;
	float gamma = matrix.threeDet(matrix.fscale(eb, -1.0f), matrix.translate(origin, matrix.fscale(camera, -1.0f)), ray) / detA;
	
	//check if camera can see intersection
	if (t > 0) {
		if (beta >= 0 && gamma >= 0) {
			if (beta + gamma < 1) {
				//if (beta < 0.02 || gamma < 0.02) {return std::make_pair(t, 0);} //draw edge      //uncomment these in order to draw edges around triangles
				//else if (beta + gamma > 0.98) {return std::make_pair(t, 0);} //draw edge
				return std::make_pair(std::make_pair(t, 1), std::make_pair(beta, gamma));//fill the polygon
			}
		}
	}
	return std::make_pair(std::make_pair(t, -1), std::make_pair(beta, gamma)); //draw nothing
}

//creates a ray between two transforms
const Transform Renderer::createRay(float x, float y, float z, const Transform& camera) const {
	Transform ray(x - camera.getX(), y - camera.getY(), z - camera.getZ(), 1.0);
	ray = normalize(ray);
	return ray;
}

//normalizes a transform or vector
const Transform Renderer::normalize(const Transform& transform) const{
	float dist = pow(transform.getX(), 2) + pow(transform.getY(), 2) + pow(transform.getZ(), 2);
	dist = pow(dist, 0.5);
	Transform newtransform(transform.getX() / dist, transform.getY() / dist, transform.getZ() / dist, 1.0);
	return newtransform;
}

//LIGHTING STUFF
//checks if a pixel is in shadow by testing for self-shadowing from other polygons in an object
bool Renderer::inShadow(const Transform& light, const Transform& lightRay, const Transform& polygonPos, const Face& polygon, const std::vector<Face>& polygons, const std::vector<Transform>& transforms) const {
	//calculate t value
	float t = 0.0;
	if (lightRay.getX() != 0.0) {
		t = (light.getX() - polygonPos.getX()) / lightRay.getX();
	}
	else if (lightRay.getY() != 0.0) {
		t = (light.getY() - polygonPos.getY()) / lightRay.getY();
	}
	else if (lightRay.getZ() != 0.0) {
		t = (light.getZ() - polygonPos.getZ()) / lightRay.getZ();
	}
	
	//check intersections of light with other polygons
	for (auto& otherPoly : polygons) {
		//don't check polygon against itself
		if (&otherPoly != &polygon) {
			//calculate intersection with other polygon
			std::pair<std::pair<float, int>, std::pair<float, float>> baryresults = checkBaryCentric(otherPoly, lightRay, polygonPos, transforms);
			if (baryresults.first.second != -1) {
				if (baryresults.first.first > 0) {
					if (baryresults.first.first < t) {
						//std::cout << "in shadow" << std::endl;
						return true;
					}
				}
			}
		}
	}
	//std::cout << "not in shadow" << std::endl;
	return false;
}

//this function calculates the color of a pixel using phong lighting method
Transform Renderer::phongShade(const PointLight& light, const Transform& lightRay, const Face& polygon, const Transform& fnorm, const Transform& viewer) const {
	Transform phong;
	Transform reflector(matrix.translate(matrix.fscale(fnorm, matrix.dotProd(fnorm, lightRay) * 2), matrix.fscale(lightRay, -1.0)));
		
	phong.setX((polygon.diffuse * light.getDiffuse().getX() * matrix.dotProd(lightRay, fnorm)) +
		(polygon.specular * light.getSpecular().getX() * pow(matrix.dotProd(viewer, reflector), polygon.shiny)) +
		(polygon.ambient * light.getAmbient().getX()));
	phong.setY((polygon.diffuse * light.getDiffuse().getY() * matrix.dotProd(lightRay, fnorm)) +
		(polygon.specular * light.getSpecular().getY() * pow(matrix.dotProd(viewer, reflector), polygon.shiny)) +
		(polygon.ambient * light.getAmbient().getY()));
	phong.setZ((polygon.diffuse * light.getDiffuse().getZ() * matrix.dotProd(lightRay, fnorm)) +
		(polygon.specular * light.getSpecular().getZ() * pow(matrix.dotProd(viewer, reflector), polygon.shiny)) +
		(polygon.ambient * light.getAmbient().getZ()));
	
	return phong;
	
}

//this function calculates the color of a pixel using phong smoothing method
Transform Renderer::phongSmooth(float beta, float gamma, const Face& polygon, const std::vector<Face>& faces, const std::vector<Transform>& fnorms) const {
	
	Transform tempvnorms[3] = {Transform(0, 0, 0), Transform(0, 0, 0), Transform(0, 0, 0)};
	
	//loop through all other polygons to determine new vertex normals
	for (auto& temp : tempvnorms) {
		int i = 0;
		for (auto& set : polygon.vals) {
			for (auto& face : faces) {
				int f = 0;
				for (auto& otherset : face.vals) {
					if (set[0] == otherset[0]) {
						i++;
						temp.setX(temp.getX() + fnorms[f].getX());
						temp.setY(temp.getY() + fnorms[f].getY());
						temp.setZ(temp.getZ() + fnorms[f].getZ());
					}
					f++;
				}
			}
		}
		temp = matrix.fscale(temp, 1.0/i);
	}
	//std::cout << tempvnorms[1] << std::endl;
	
	
	//VNorm origin(vnorms[polygon.vals[0][2] - 1]);
	//VNorm eb(vnorms[polygon.vals[1][2] - 1]);
	//VNorm ec(vnorms[polygon.vals[2][2] - 1]);
	
	VNorm newVNorm; 
	newVNorm.x = (((tempvnorms[1].getX() - tempvnorms[0].getX()) * beta) + tempvnorms[0].getX()) + (((tempvnorms[2].getX() - tempvnorms[0].getX()) * gamma) + tempvnorms[0].getX());
	newVNorm.y = (((tempvnorms[1].getY() - tempvnorms[0].getY()) * beta) + tempvnorms[0].getY()) + (((tempvnorms[2].getY() - tempvnorms[0].getY()) * gamma) + tempvnorms[0].getY());
	newVNorm.z = (((tempvnorms[1].getZ() - tempvnorms[0].getZ()) * beta) + tempvnorms[0].getZ()) + (((tempvnorms[2].getZ() - tempvnorms[0].getZ()) * gamma) + tempvnorms[0].getZ());
	
	return normalize(Transform(newVNorm.x, newVNorm.y, newVNorm.z));
}
