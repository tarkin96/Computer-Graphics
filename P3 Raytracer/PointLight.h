#include "Transform.h"
//This class reprsents a point light for shadowing and phong lighting
class PointLight {
	public: 
		//main contructor
		PointLight(const Transform&, const Transform&, const Transform&, const Transform&, const Transform&);
		//getters and setters
		Transform getPosition() const;
		Transform getColor() const;
		Transform getAmbient() const;
		Transform getDiffuse() const;
		Transform getSpecular() const;
		void setPosition(const Transform&);
		void setColor(const Transform&);
		void setAmbient(const Transform&);
		void setDiffuse(const Transform&);
		void setSpecular(const Transform&);
		
	private:
		//constants for phong lighting
		Transform position;
		Transform color;
		Transform ambient;
		Transform diffuse;
		Transform specular;
};
