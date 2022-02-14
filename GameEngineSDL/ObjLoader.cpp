
#include "ObjLoader.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>  
#include <sstream>
using namespace std;

std::vector<std::string> split(std::string x, char token) {
	std::vector<std::string> strings;
	std::stringstream f(x);
	std::string s;
	while (getline(f, s, token)) {
		strings.push_back(s);
	}
	return strings;
}

Entity loadEntity(std::string path)
{
	std::stringstream ss;
	std::ifstream file;
	file.open(path.c_str());
	std::string line;
	std::string prefix = "";
	Entity obj = Entity();
	std::string s = "\n";
	int faceId = 0;
	int v = 0;
	int tempF;

	if (file.is_open()) {
		//printf("%s open!\n", path.c_str());
		while (getline(file, line)) {
			ss.clear();
			ss.str(line);
			ss >> prefix;
			if (prefix == "v") {
				Vertex v;
				//printf("This is a vertex\n");
				v.w = 1;
				ss >> v.x >> v.y >> v.z;
				//printf("%f, %f, %f\n", v.x, v.y, v.z);
				obj.vertexAmount++;
				obj.Vertices.push_back(v);
			}
			else if (prefix == "vt") {
				//printf("This is a texture\n");
				Texture t;
				ss >> t.u >> t.v;
				obj.Textures.push_back(t);
				obj.textureAmount++;
			}
			else if (prefix == "vn") {
				//printf("This is a vertex normal\n");
				Normal n;
				ss >> n.x >> n.y >> n.z;
				obj.Normals.push_back(n);
			}
			else if (prefix == "usemtl") {
				//todo
			}
			else if (prefix == "s") {
				//printf("This is a s\n");
			}
			else if (prefix == "f") {
				//std::printf("This is a face\n");
				Face f;
				f.capacity = 0;
				std::vector<std::string> aux = split(line, ' ');
				int counter = 0;
				
				while (ss >> tempF) {
					if (counter == 0) {
						f.Vertices.push_back(tempF);
						f.capacity++;
					}
					else if (counter == 1) {
						f.Textures.push_back(tempF);
					}
					else if (counter == 2) {
						f.Normals.push_back(tempF);
					}
					if (ss.peek() == '/')
					{
						++counter;
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' ')
					{
						++counter;
						ss.ignore(1, ' ');
					}

					//Reset the counter
					if (counter > 2)
						counter = 0;
				}
				/*for (int i = 0; i < f.capacity; i++)
					std::printf("face %d %d %d\n", f.Vertices[i], f.Textures[i], f.Normals[i]);*/
				obj.Faces.push_back(f);
				obj.faceAmount++;
			}
		}//std::printf("EOF\n");
	}
	else {
		std::printf("%s not found\n", path.c_str());
	}
	file.close();
	return obj;
}