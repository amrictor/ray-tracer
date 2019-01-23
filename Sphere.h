#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "Face.h"
#include "Material.h"
#include "Ray.h"

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Sphere
{
public:

    Vector3d center;
	double radius;
	std::string driverFile, programName;
    Material mat;
	Sphere(){};
	Sphere(std::string& driver, std::string& program, std::istringstream& driver_iss);
	~Sphere(){};
	
	Vector3d refractTRay(Vector3d direction, Vector3d normal, double eta1, double eta2);
	Ray refractExit(Vector3d direction, Vector3d point, double eta);
	void debug();
};
#endif
