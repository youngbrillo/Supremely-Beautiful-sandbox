#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	//bone indexes to influence this vertex
	int		m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
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
public:
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<vTexture>		textures;

private: //render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
public:
	std::string directory;
};

