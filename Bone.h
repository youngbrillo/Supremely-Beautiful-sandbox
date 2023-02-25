#pragma once

/* Container for bone data */

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "assimp_glm_helpers.h"

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};
struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};
struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};
class Bone
{
public: 
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);

public:
	void Update(float animationTime);

	glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetBoneName() const { return m_Name; }
	int GetBoneID() { return m_ID; }
	int GetPositionIndex(const float& animationTime);
	int GetRotationIndex(const float& animationTime);
	int GetScaleIndex(const float& animationTime);
private:
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScaling(float animationTime);


	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;
};

