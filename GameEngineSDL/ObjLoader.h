#pragma once
#include <vector>
#include <string>
#include <fstream>

struct Color {
	int red;
	int green;
	int blue;
	int alpha;
};
struct Vertex {
	int index;
	float x;
	float y;
	float z;
	float w;
};
struct Texture {
	float u;
	float v;
};
struct Normal {
	float x;
	float y;
	float z;
};
struct Face {
	int id;
	Color color;
	std::vector<int> vertices;
	std::vector<int> normals;
	std::vector<int> textures;
};
class Entity {
public:
	int EdgeAmount;
	int vertexAmount;
	int faceAmount;
	std::vector<Vertex> vertices;
};

Entity loadEntity(std::string path);