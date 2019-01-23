#include "Light.h"
#include <iostream>
#include <sstream>
#include <Eigen/Dense>

using namespace std;

Light::Light(string& driver, string& program, istringstream& driver_iss) : driverFile(driver), programName(program) {
		driver_iss >> position[0] >> position[1] >> position[2] >> w >> color[0] >> color[1] >> color[2];
		if(w==0)
			position = Vector3d(0, 0, 0) + (10000000000000 * position);
};
Light::~Light(){};
	
void Light::debug(){
	cout << "position: (" << position[0] << ", " << position[1] << ", " << position[2] << ")\n";
	cout << "w: " << w << '\n';
	cout << "color: (" << color[0] << ", " << color[1] << ", " << color[2] << ")\n";
}
