#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <Eigen/Dense>
#include <vector>
#include "Face.h"
#include "Model.h"
#include "Light.h"
#include "Sphere.h"
#include "Ray.h"

using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

class Camera 
{
public:
	Camera(std::string driver, std::string program, std::string output, std::vector<Model>& models, std::vector<Light>& lights, std::vector<Sphere>& spheres);
	~Camera(){};
	
	std::vector<Vector4d> normals;
	Vector3d Wv, Mv, Uv, Vv;
	std::string driverFile, programName, outputFile;
	double eyeX, eyeY, eyeZ, lX, lY, lZ, uX, uY, uZ, focalLength, left, bottom, right, top;
	Vector3d eye, look, up;
	int hRes, vRes, recursionDepth;
	Vector3d ambient;
	
	void setEye(std::istringstream& driver_iss);
	void setRecursion(std::istringstream& driver_iss);
	void setLook(std::istringstream& driver_iss);
	void setUp(std::istringstream& driver_iss);
	void setFocalLength(std::istringstream& driver_iss);
	void setBounds(std::istringstream& driver_iss);
	void setResolution(std::istringstream& driver_iss);
	void setAmbient(std::istringstream& driver_iss);
	void setVectors();
	void writePPM();
	void debug();
private:
	std::vector<Model>& models;
	std::vector<Light>& lights;
	std::vector<Sphere>& spheres;
	Vector3d pixelToPoint(int i, int j);
	Vector3d triangleIntersection(Face& face, Ray& ray);
	double rayIntersect(Ray& ray, Material& mat, Vector3d& normal, bool& model, Sphere& sphere);
	void pt_illum(Vector3d& point, Ray &ray, Vector3d& normal, Material& mat, Vector3d& illum, Vector3d& atten, bool& model);
	void ray_trace(Ray ray, Vector3d& illum, Vector3d attenuation, int level);
};
#endif
