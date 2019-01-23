#include "Sphere.h"
#include "Ray.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <Eigen/Dense>

using namespace std;

using Eigen::Vector3d;

Sphere::Sphere(string& driver, string& program, istringstream& driver_iss) : driverFile(driver), programName(program) {
	driver_iss >> center[0] >> center[1] >> center[2] >> radius;
    mat.setKa(driver_iss);
    mat.setKd(driver_iss);
    mat.setKs(driver_iss);
    mat.setKr(driver_iss);
	mat.setKo(driver_iss);
	mat.setEta(driver_iss);
	mat.setNs(16.0);
};

Vector3d Sphere::refractTRay(Vector3d direction, Vector3d normal, double eta1, double eta2) {
        double etaR = eta1 / eta2;
        double wn = direction.dot(normal);
        double radSq = (etaR*etaR) * ((wn*wn) - 1) + 1;
        if (radSq < 0.0) return Vector3d(0.0,0.0,0.0);
        else {
           double b = (etaR * wn) - sqrt(radSq);
           return (-1*etaR) * direction + b * normal;
		}
}
Ray Sphere::refractExit(Vector3d direction, Vector3d point, double eta){
        Vector3d T1 = refractTRay(direction, (point - center).normalized(), 1.0, eta);
        if ((T1[0]+T1[1]+T1[2]) == 0.0) return Ray(false);

		Vector3d exit = point + 2 * (center - point).dot(T1) * T1;
		Vector3d Nin = (center - exit).normalized();
		Vector3d T2 = refractTRay(-1*T1, Nin, eta, 1.0);
		return Ray(exit, T2);
}

void Sphere::debug() {
	cout<<"center:\n"<<center<<'\n';
	cout<<"radius: "<<radius<<'\n';
    mat.debug();
}
