#pragma once
#include <vector>
#include <string>
#include <fstream>
using namespace std;

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
	int capacity;
	Color color;
	std::vector<int> Vertices;
	std::vector<int> Normals;
	std::vector<int> Textures;
};
struct Edge {
	int a;
	int b;
};
class Entity {
public:
	int EdgeAmount;
	int vertexAmount;
	int faceAmount;
	int textureAmount;
	vector<Vertex> Vertices;
	vector<Texture> Textures;
	vector<Normal> Normals;
	vector<Face> Faces;
	vector<Edge> Edges;
};

Entity loadEntity(std::string path);