#include "Ray.h"
#include <iostream>
#include <sstream>
#include <Eigen/Dense>

using namespace std;

Ray::Ray(Vector3d& origin, Vector3d& direction) : origin(origin), direction(direction), real(true){
};
Ray::Ray(bool fake) : real(false){
};
Ray::~Ray(){};

void Ray::debug(){
	cout << "origin: (" << origin[0] << ", " << origin[1] << ", " << origin[2] << ")\n";
	cout << "direction: (" << direction[0] << ", " << direction[1] << ", " << direction[2] << ")\n";
}
