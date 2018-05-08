/*	This class represents the current transformation info of an object
 * 	or entity. Matrix and vector operations may be performed on these.
 * 
 * 
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H
class Transform {
	public:
		Transform();
		Transform(float);
		Transform(float, float);
		Transform(float, float, float);
		Transform(float, float, float, float);
		float getX() const; float getY() const; float getZ() const; float getW() const;
		void setX(float); void setY(float); void setZ(float); void setW(float);
		Transform& operator=(const Transform&);
	private:
		float x, y, z, w;
};
#endif
