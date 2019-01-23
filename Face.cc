#include "Face.h"
#include <iostream>
#include <sstream>
#include <Eigen/Dense>

using namespace std;

Vector3d dehomogenize(Vector4d v) {
	return Vector3d(v[0]/v[3], v[1]/v[3], v[2]/v[3]);
}
Vector4d homogenize(Vector3d v) {
	return Vector4d(v[0], v[1], v[2], 1);
}

void Face::setSide(int& index, Vector4d& value) {
	sides[index]=value;
}
void Face::setNormal() {
	Vector3d e0 = (dehomogenize(sides[1])-dehomogenize(sides[0])); 
	Vector3d e1 = (dehomogenize(sides[2])-dehomogenize(sides[1]));
	normal = e0.cross(e1).normalized();
}
Vector4d& Face::operator[](int index) { 
	return sides[index]; 
} 

