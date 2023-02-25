#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Transform.h"

#include "animdata.h"
#include "assimp_glm_helpers.h"
#include <map>
class Model
{
public:
	Model(const char* modelFilePath) { LoadModel(modelFilePath); }
	~Model() {};

	std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }
	void Draw(Shader* shader);
	void Debug(const char* modelName = "Unammed Model");
private:
	void LoadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& verticies, aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void SetVertexBoneDataToDefault(Vertex& vertex);

	std::vector<vTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<Mesh> meshes;
	std::vector<vTexture> textures_loaded;
	std::string directory;

	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;
public:
	Transform transform;
	glm::vec4 color = glm::vec4(1.0f);
	bool gammaCorrection = false;
};

