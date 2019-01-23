#include "Camera.h"
#include "Face.h"
#include "Light.h"
#include "Model.h"
#include "Material.h"
#include "Sphere.h"
#include "Ray.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>

using namespace std;

Camera::Camera(string driver, string program, string output, vector<Model>& models, vector<Light>& lights, vector<Sphere>& spheres) : driverFile(driver), programName(program), outputFile(output), recursionDepth(0), models(models), lights(lights), spheres(spheres) {};

void Camera::setRecursion(istringstream& driver_iss) {
	driver_iss >> recursionDepth;
}
void Camera::setEye(istringstream& driver_iss) {
	driver_iss >> eye[0] >> eye[1] >> eye[2];
}
void Camera::setLook(istringstream& driver_iss) {
	driver_iss >> look[0] >> look[1] >> look[2];
}
void Camera::setUp(istringstream& driver_iss) {
	driver_iss >> up[0] >> up[1] >> up[2];
}
void Camera::setFocalLength(istringstream& driver_iss) {
	driver_iss >> focalLength;
}
void Camera::setBounds(istringstream& driver_iss) {
	driver_iss >> left >> bottom >> right >> top;
}
void Camera::setResolution(istringstream& driver_iss) {
	driver_iss >> hRes >> vRes;
}
void Camera::setAmbient(istringstream& driver_iss) {
	driver_iss >> ambient[0] >> ambient[1] >> ambient[2];
}
void Camera::setVectors() {
	Wv = (eye - look).normalized();
	Mv << 0, 0, 0;
	if(Wv[0]==1.0 || Wv[0]==-1.0) Mv[1] = 1.0;
	else Mv[0] = 1.0;
	
	Uv = Wv.cross(up).normalized();
	Vv = Wv.cross(Uv).normalized();
}
double Camera::rayIntersect(Ray& ray, Material& mat, Vector3d& normal, bool& model, Sphere& sphere) {
//	Vector3d direction = (destination - origin).normalized();
	double min = -1.0;
	bool smooth = true;
	for(Model m: models) {
		for(Face f : m.faces){
			
			Vector3d betaGammaT = triangleIntersection(f, ray);
			double beta = betaGammaT[0];
			double gamma = betaGammaT[1];
			double t = betaGammaT[2];
			//also get beta and gamma
			
			if(t<=.0000000001) continue; //check self intersection
			if(t!=-1.0) { //check for intersection
				if((t < min || min == -1.0)/* && t > .00001*/) { //new min t
					min = t;
					mat = m.materials[0];
					if(smooth) {
						Vector3d averageNorms[3];
						for (int vertex = 0; vertex < 3; vertex++) {
							averageNorms[vertex] = Vector3d(0, 0, 0);
							vector<int> adjacentFaces = m.vertexToFaces[f.vertices[vertex]];

							for(unsigned int fIndex = 0; fIndex < adjacentFaces.size(); fIndex++) {
								Face adjFace = m.faces[adjacentFaces[fIndex]];
								if(f.normal.dot(adjFace.normal) > .92718385456) { //cos(0) is 1!
									
									averageNorms[vertex] += adjFace.normal;

								}
							}
						}

						
						normal = ((1 - beta - gamma) * averageNorms[0] + beta*averageNorms[1] + gamma*averageNorms[2]).normalized();

					}

					else {
						normal = f.normal;
					}
					model = true;
				}
			}
		}
	}
	for(Sphere s: spheres) {
		double distanceToClosest = (s.center - ray.origin).dot(ray.direction);
		double distanceToCenter = (s.center - ray.origin).norm();
		double distanceFromCenterToClosest = sqrt(distanceToCenter*distanceToCenter - distanceToClosest*distanceToClosest);
		double distanceFromClosestToEdgeSqd = s.radius*s.radius - distanceFromCenterToClosest*distanceFromCenterToClosest;
		if(distanceFromClosestToEdgeSqd >= 0)  {
			double t = distanceToClosest - sqrt(distanceFromClosestToEdgeSqd);
			if(t<=.0000000001) continue;
			if(t < min || min == -1.0) {
					min = t;
					mat = s.mat;
					normal = ((ray.origin + t * ray.direction) - s.center).normalized();
					if(normal.dot(ray.direction)>0) normal *= -1;
					model=false;
					sphere = s;
			}
		}
	}

	return min;
}

Vector3d Camera::triangleIntersection(Face& face, Ray& ray) {
	Matrix3d M;
	M << 	(face[0][0] - face[1][0]), (face[0][0] - face[2][0]), ray.direction[0],
			(face[0][1] - face[1][1]), (face[0][1] - face[2][1]), ray.direction[1],
			(face[0][2] - face[1][2]), (face[0][2] - face[2][2]), ray.direction[2];
			
	Vector3d Y;
	Y <<	(face[0][0] - ray.origin[0]), (face[0][1] - ray.origin[1]), (face[0][2] - ray.origin[2]);
	
	Vector3d betaGammaT = M.partialPivLu().solve(Y);
	if(!(betaGammaT[0]>=0 && betaGammaT[1]>=0 && (betaGammaT[0] + betaGammaT[1])<=1 && betaGammaT[2]>0)) {
		betaGammaT[2] = -1.0;
	}
	return betaGammaT;
}
	
Vector3d Camera::pixelToPoint(int i, int j) {
	double px = ((double)i)/(hRes-1) * (left-right) + right;
	double py = ((double)j)/(vRes-1) * (bottom-top) + top;
	return eye + (-1*focalLength*Wv) + (px * Uv) + (py * Vv);
}

void Camera::pt_illum(Vector3d& point, Ray &ray, Vector3d& normal, Material& mat, Vector3d& illum, Vector3d& atten, bool& model) {
	Vector3d color = ambient.cwiseProduct(mat.Ka);
	for(Light l : lights){
		Vector3d directionToLight = (l.position - point).normalized();
		Ray rayToLight = Ray(point, directionToLight);
		Material trashM;
		Vector3d trashV;
		bool trashB;
		Sphere trashS;
		if(rayIntersect(rayToLight, trashM, trashV, trashB, trashS) != -1.0) continue; 
		
		if(directionToLight.dot(normal) < 0 && model) normal*=-1;
		double NdotL = (normal.dot(directionToLight) < 0) ? 0 : normal.dot(directionToLight);

		if(NdotL > 0.0) {
			color += (mat.Kd.cwiseProduct(l.color)*(NdotL)); //diffuse
			
			Vector3d toC = (ray.origin - point).normalized();
			Vector3d spR = ((2 * (NdotL) * normal) - directionToLight).normalized();
			double CdotR = (toC.dot(spR) < 0) ? 0 : toC.dot(spR);
			
			if(CdotR > 0.0) {
				color += (mat.Ks.cwiseProduct(l.color) * pow(CdotR, mat.Ns)); //specular
			}
			
		}
	}
	for (int i = 0; i<3; i++) illum[i] += atten[i] * color[i];
}

void Camera::ray_trace(Ray ray, Vector3d& illum, Vector3d atten, int level) {
	Material mat;
	Vector3d normal;
	bool model;
	Sphere s;
	double t = rayIntersect(ray, mat, normal, model, s);
	
	
	if(t != -1.0) {  //if there is an intersection
		Vector3d surfacePoint = ray.origin + t * ray.direction;
		pt_illum(surfacePoint, ray, normal, mat, illum, atten, model);


		if (level > 0) {
			Vector3d inverseDirection = -1 * ray.direction;
			Vector3d refR = 2 * normal.dot(inverseDirection) * normal - inverseDirection;
			ray_trace(Ray(surfacePoint, refR), illum, mat.Kr.cwiseProduct(atten), level-1);
		}

		if (level > 0 && (mat.Ko[0]+mat.Ko[1]+mat.Ko[2]) < 3.0 && !model) {

			Vector3d thru = Vector3d(0,0,0);
			Ray fraR = s.refractExit(-1 * ray.direction, surfacePoint, mat.eta);
			if(fraR.real) {
				ray_trace(fraR, thru, mat.Kr.cwiseProduct(atten), (level - 1));
                for (int i = 0; i<3; i++) illum[i] += atten[i] * (1.0 - mat.Ko[i]) * thru[i];
			}
		}
		return;
	}

}

void Camera::writePPM() {
	ofstream out(outputFile);
	out << "P3\n#Rendered by Abigail Rictor\n" << hRes << " " << vRes << '\n' << 255 << '\n';

	for (int j = hRes-1; j >=0; j--) {
		cout<< 100*(double(hRes-1-j)/double(hRes-1))<<"%\n";
		for(int i = 0; i < vRes; i++) {
		
 			Vector3d point = pixelToPoint(i, j);
 			Vector3d direction = (point - eye).normalized();
			Ray ray = Ray(point, direction);
			
			Vector3d illum = Vector3d(0, 0, 0);
			Vector3d atten = Vector3d(1.0, 1.0, 1.0);
			ray_trace(ray, illum, atten, recursionDepth);
			
			out<<int(illum[0]*255)<<" "<<int(illum[1]*255)<<" "<<int(illum[2]*255)<<" ";
		}
		out << '\n';
	}		
}



void Camera::debug(){
	cout << "eye: (" << eye[0] << ", " << eye[1] << ", " << eye[2] << ")\n";
	cout << "look: (" << look[0] << ", " << look[1] << ", " << look[2] << ")\n";
	cout << "up: (" << up[0] << ", " << up[1] << ", " << up[2] << ")\n";
	cout << "focal length: " << focalLength << '\n';
	cout << "bounds: " << left << " " << bottom << " " << right << " " << top << '\n';
	cout << "resolution: " << hRes << " " << vRes << '\n';
	cout << "ambient: (" << ambient[0] << ", " << ambient[1] << ", " << ambient[2] << ")\n";
	cout << "WV: " << Wv << '\n';
	cout << "UV: " << Uv << '\n';
	cout << "VV: " << Vv << '\n';
}



