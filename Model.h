#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <Eigen/Dense>
#include <vector>
#include <map>
#include "Face.h"
#include "Material.h"

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Model //contains already transformed vertices
{
public:
	Matrix4d superMatrix;
	std::vector<Face> faces;
	std::vector<Vector4d> vertices;
	std::vector<Vector4d> normals;
	std::vector<Material> materials;
	std::vector<std::vector<int>> vertexToFaces; 
	double wx, wy, wz, scale, tx, ty, tz;
	int theta;
	std::string shading, obj;
	std::string driverFile, programName;
	bool smooth;
	
	Model(std::string& driver, std::string& program, std::istringstream& driver_iss);
	~Model(){};
	
	void setMaterials(std::string& fileName);
	void debug();
};
#endif
