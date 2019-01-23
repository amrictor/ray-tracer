#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Ray 
{
public:
	std::string driverFile, programName;
	Vector3d origin;
	Vector3d direction;
	Vector3d bestPoint;
	Vector3d bestSphere;
	bool real;
	
	Ray(Vector3d& origin, Vector3d& direction);
	Ray(bool fake);
	~Ray();
	void debug();
};
#endif
