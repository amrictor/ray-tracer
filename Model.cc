#include "Model.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <Eigen/Dense>

using namespace std;


Matrix4d scaleMatrix(double& scale){
	Matrix4d S = Matrix4d::Identity();
	S(0,0) = scale;
	S(1,1) = scale;
	S(2,2) = scale;
	return S;
}

Matrix4d translationMatrix(double& tx, double& ty, double& tz){
	Matrix4d T = Matrix4d::Identity();
	T(0,3) = tx;
	T(1,3) = ty;
	T(2,3) = tz;
	return T;
}

Matrix4d rotationMatrix(double& wx, double& wy, double& wz, int& theta){
	Vector3d Wv = Vector3d(wx, wy, wz).normalized();
	
	Vector3d Mv;
	Mv << 0, 0, 0;
	if(Wv[0]==1.0 || Wv[0]==-1.0) Mv[1] = 1.0;
	else Mv[0] = 1.0;
	
	Vector3d Uv = Wv.cross(Mv);
	Uv = Uv.normalized();
	Vector3d V = Wv.cross(Uv);
	V = V.normalized();
	
	Matrix4d Rw; //order is u, v, w
	Rw <<	Uv[0], Uv[1], Uv[2], 0,
			 V[0],  V[1],  V[2], 0,
			Wv[0], Wv[1], Wv[2], 0,
			    0,     0,     0, 1;

	Matrix4d Rz = Matrix4d::Identity();
	Rz.row(0) << cos(theta*M_PI/180), -sin(theta*M_PI/180), 0, 0;
	Rz.row(1) << sin(theta*M_PI/180), cos(theta*M_PI/180), 0, 0;
	
	return Rw.transpose()*Rz*Rw;
}

Model::Model(string& driver, string& program, istringstream& driver_iss) : driverFile(driver), programName(program) {
	driver_iss >> wx >> wy >> wz >> theta >> scale >> tx >> ty >> tz >> shading >> obj;
	smooth = (shading=="smooth");
	
	ifstream in(obj);
	if(in.fail()){
		cerr << programName <<": Error with \"" << obj << "\": File not found.\n";
		exit(EXIT_FAILURE);
	}
	
	superMatrix = Matrix4d(translationMatrix(tx, ty, tz) * rotationMatrix(wx, wy, wz, theta) * scaleMatrix(scale));

	string line;
	while(getline(in, line)){
		int commentIndex = line.find_first_of('#');
		string no_comments = line;
		if(commentIndex!=-1) no_comments = line.substr(0,commentIndex);
		istringstream iss(no_comments);
		
		string s;
		iss>>s;
		if(s=="mtllib") {
			iss>>s;
			setMaterials(s);
		}
		if(s=="v") {
			Vector4d point(0, 0, 0, 1);
			iss >> point[0] >> point[1] >> point[2];
			vertexToFaces.push_back({});
			vertices.push_back(superMatrix * point);
		} 
		if(s=="vn") {
			Vector4d normal(0, 0, 0, 1);
			iss >> normal[0] >> normal[1] >> normal[2];
			normals.push_back(superMatrix * normal);
		} 
		if(s=="f") {
			Face face;
			for(int i = 0; i<3; i++){
				int vertexNum;
				iss>>vertexNum;
				vertexToFaces[vertexNum-1].push_back(faces.size()); //or mem address of face
				face[i] = vertices[vertexNum-1];
				face.vertices[i] = vertexNum -1;
				iss.ignore(1);
				if(iss.peek()!='/') iss>>vertexNum;
				iss.ignore(1);
				iss>>vertexNum;
			}
			face.setNormal();
			faces.push_back(face);
		}
	}
};
void Model::setMaterials(string& fileName){
	ifstream in(fileName);
	if(in.fail()){
		cerr << programName <<": Error with \"" << fileName << "\": File not found.\n";
		exit(EXIT_FAILURE);
	}
	string line;
	while(getline(in, line)){
		int commentIndex = line.find_first_of('#');
		string no_comments = line;
		if(commentIndex!=-1) no_comments = line.substr(0,commentIndex);
		istringstream iss(no_comments);
		
		string s;
		iss>>s;
		if(s=="newmtl") {
			iss>>s;
			materials.push_back(Material(s));
		}
		if(s=="Ka") materials[materials.size()-1].setKa(iss);
		if(s=="Kd") materials[materials.size()-1].setKd(iss);
		if(s=="Ks") materials[materials.size()-1].setKs(iss);
		if(s=="Ke") materials[materials.size()-1].setKe(iss);
		if(s=="Ns") materials[materials.size()-1].setNs(iss);
		
	}
	materials[materials.size()-1].setKr();
	
}
void Model::debug() {
	cout<<"T:\n"<<translationMatrix(tx, ty, tz)<<'\n';
	cout<<rotationMatrix(wx, wy, wz, theta)<<'\n';
	cout<<"S:\n"<<scaleMatrix(scale)<<'\n';
// 	cout<<"SUPER:\n"<<superMatrix<<'\n';
// 	cout<<"vertices: ";
// 	for(Vector4d v : vertices){
// 		cout << v << '\n';
// 	}
// 	for(Material m : materials){
// 		m.debug();
// 	}
	for(unsigned int i = 0; i < vertices.size(); i++) {
		cout << "Vertex " << i+1 <<":\n";
		cout << "Faces: ";
		for(int i : vertexToFaces[i]) cout << i << " ";
		cout << '\n';
	}
}
