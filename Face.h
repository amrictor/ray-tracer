#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED

#include <Eigen/Dense>
#include <vector>

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Face
{
public:
	int vertices[3];
	Vector4d sides[3];
	Vector3d normal;
	Face(){};
	~Face(){};
	
	void setSide(int& index, Vector4d& value);
	void setNormal();
	Vector4d& operator[](int index);
};
#endif
