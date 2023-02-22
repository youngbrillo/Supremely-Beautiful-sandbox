#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Transform.h"


class Model
{
public:
	Model(const char* modelFilePath) { LoadModel(modelFilePath); }
	void Draw(Shader* shader);
	void Debug();
private:
	void LoadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<vTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
private:
	std::vector<Mesh> meshes;
	std::vector<vTexture> textures_loaded;
	std::string directory;
public:
	Transform transform;
	glm::vec4 color = glm::vec4(1.0f);
};

