
#include "ObjLoader.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>  
#include <sstream>

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
	if (file.is_open()) {
		printf("%s open!\n", path.c_str());
		while (getline(file, line)) {
			ss.clear();
			ss.str(line);
			ss >> prefix;
			if (prefix == "v") {
				Vertex v;
				printf("This is a vertex\n");
				std::string x, y, z;
				ss >> v.x >> v.y >> v.z;
				printf("%f, %f, %f\n", v.x, v.y, v.z);
				obj.vertexAmount++;
				obj.vertices.push_back(v);
			}
			else if (prefix == "vt") {
				printf("This is a texture\n");
			}
			else if (prefix == "vn") {
				printf("This is a vertex normal\n");
			}
			else if (prefix == "usemtl") {
			}
			else if (prefix == "s") {
				printf("This is a s\n");
			}
			else if (prefix == "f") {
				printf("This is a face\n");
				Face f;
				std::vector<std::string> aux = split(line, ' ');
				
				for (int i = 0; i < aux.size(); i++) {
					int v = 0;
					int n = 0;
					int t = 0;
					std::vector<std::string> aux2 = split(aux[i], '/');
					for (int j = 0; j < aux2.size(); j++) {
							std::cout << aux2[j] << ' ';
							printf("j filho de uma puta %d\n", j);
					}
					std::cout << '\n';

					printf("%d %d %d\n", v, n, t);
				}
			}
		}printf("EOF\n");
	}
	else {
		printf("%s not found\n", path.c_str());
	}
	file.close();
	return obj;
}