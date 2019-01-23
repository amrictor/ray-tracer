#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Material
{
public:
	std::string name;
	Vector3d Ka, Kd, Ks, Kr, Ke, Ko, Ni, d;
	double Ns;
	double eta;
	int illumination;
	
	Material();
	Material(std::string& name);
	~Material();
	void setKo(std::istringstream& iss);
	void setKa(std::istringstream& iss);
	void setKd(std::istringstream& iss);
    void setKs(std::istringstream& iss);
    void setKr(std::istringstream& iss);
	void setKr();
	void setKe(std::istringstream& iss);
	void setEta(std::istringstream& iss);
	void setNs(std::istringstream& iss);
	void setNs(double phong);

	
	void debug();
};
#endif
