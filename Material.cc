#include "Material.h"
#include <iostream>
#include <sstream>
#include <Eigen/Dense>

using namespace std;

Material::Material() {};
Material::Material(string& name) : name(name) {};
Material::~Material(){};


void Material::setKo(istringstream& iss) {
	iss >> Ko[0] >> Ko[1] >> Ko[2];
}
void Material::setKa(istringstream& iss) {
	iss >> Ka[0] >> Ka[1] >> Ka[2];
}
void Material::setKd(istringstream& iss) {
	iss >> Kd[0] >> Kd[1] >> Kd[2];
}
void Material::setKs(istringstream& iss) {
	iss >> Ks[0] >> Ks[1] >> Ks[2];
}
void Material::setKr(istringstream& iss) {
	iss >> Kr[0] >> Kr[1] >> Kr[2];
}
void Material::setKr() {
	Kr << 1, 1, 1;
}
void Material::setKe(istringstream& iss) {
	iss >> Ke[0] >> Ke[1] >> Ke[2];
}
void Material::setEta(istringstream& iss) {
	iss >> eta;
}
void Material::setNs(istringstream& iss) {
	iss >> Ns;
}
void Material::setNs(double phong) {
	Ns = phong;
}
void Material::debug() {
	cout << "name: " << name << "\n";
	cout << "Ka: (" << Ka[0] << ", " << Ka[1] << ", " << Ka[2] << ")\n";
	cout << "Kd: (" << Kd[0] << ", " << Kd[1] << ", " << Kd[2] << ")\n";
    cout << "Ks: (" << Ks[0] << ", " << Ks[1] << ", " << Ks[2] << ")\n";
    cout << "Kr: (" << Kr[0] << ", " << Kr[1] << ", " << Kr[2] << ")\n";
	cout << "Ke: (" << Ke[0] << ", " << Ke[1] << ", " << Ke[2] << ")\n";
	cout << "Ko: (" << Ko[0] << ", " << Ko[1] << ", " << Ko[2] << ")\n";
	cout << "Eta: " << eta << "\n";
    cout << "Ns: " << Ns << "\n";
	
}
