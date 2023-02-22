#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
	Model(const char* modelFilePath) { LoadModel(modelFilePath); }
	void Draw(Shader* shader);
private:
	void LoadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<vTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
private:
	std::vector<Mesh> meshes;
	std::vector<vTexture> textures_loaded;
	std::string directory;
};

