#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Light 
{
public:
	std::string driverFile, programName;
	Vector3d position;
	Vector3d color;
	int w;
	
	Light(std::string& driver, std::string& program, std::istringstream& driver_iss);
	~Light();
	
	void debug();
};
#endif
