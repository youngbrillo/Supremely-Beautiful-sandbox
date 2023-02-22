#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoords;
};

struct vTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<vTexture> textures);
	void Draw(Shader* shader);
	std::string directory;
public:
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<vTexture>		textures;

private: //render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

