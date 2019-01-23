#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Sphere.h"

using namespace std;
// using Eigen::Vector3i;
using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::VectorXf;
using Eigen::Vector4d;

//FREE FUNCTIONS

bool fexists(string& filename) {
	ifstream ifile(filename);
	return !ifile.fail();
}

//FILE READER
Camera readDriver(string& programName, string& fileName, string& outputFile, vector<Model>& models, vector<Light>& lights, vector<Sphere>& spheres){
    ifstream in(fileName);
    if(in.fail()){
        cerr << programName <<": Error with \"" << fileName << "\": File not found.\n";
        exit(EXIT_FAILURE);
    }
    Camera cam = Camera(programName, fileName, outputFile, models, lights, spheres);
    string line;
    while(getline(in, line)){
		int commentIndex = line.find_first_of('#');
		if(commentIndex!=-1) line = line.substr(0,commentIndex);
		istringstream iss(line);
		string type;
		iss>>type;
		if(type=="recursionLevel") cam.setRecursion(iss);
		if(type=="eye") cam.setEye(iss);
		if(type=="look") cam.setLook(iss);
		if(type=="up") cam.setUp(iss);
		if(type=="d") cam.setFocalLength(iss);
		if(type=="bounds") cam.setBounds(iss);
		if(type=="res") cam.setResolution(iss);
		if(type=="ambient") cam.setAmbient(iss);
		if(type=="light") lights.push_back(Light(fileName, programName, iss));
		if(type=="model") models.push_back(Model(fileName, programName, iss));
		if(type=="sphere") spheres.push_back(Sphere(fileName, programName, iss));
	}
	cam.setVectors();
	return cam;
}



//MAIN
int main(int argc, char *argv[]){
    if(argc!=3) {
        cerr<< "usage: " << argv[0] << " [driverFileName] [outputFileName]\n";
        exit(EXIT_FAILURE);
    }
    string s0 = argv[0], s1 = argv[1], s2 = argv[2];
	vector<Model> models;
	vector<Light> lights;
	vector<Sphere> spheres;
	Camera cam = readDriver(s0, s1, s2, models, lights, spheres);
	cam.writePPM();
}


