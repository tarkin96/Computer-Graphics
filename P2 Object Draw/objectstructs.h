//These structs represent individual parts of objects

//vertex position
struct Vertex {
	float vals[4] = {0.0, 0.0, 0.0, 1.0};
};
//texture coordinate
struct TexCoord {
	float vals[3] = {0.0, 0.0, 0.0};
};
//vertex normal
struct VNorm {
	float vals[3] = {0.0, 0.0, 0.0};
};
//face
struct Face {
	unsigned int vals[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
};
