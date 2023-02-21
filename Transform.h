#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Transform
{
public:
	Transform();
	~Transform();

	glm::mat4& Model();
	void UpdateMatrix();

	void Debug(const char* owner);

	//This is not accurate for transforms with rotations applied!!
	//If matrix transform changes, make sure update matrix is called before hand!
	bool isInBounds(const glm::vec2& pos);
public:
	glm::ivec2 direction;
	glm::vec3 position, size, pivot, orientation;
	glm::mat4 m_model;
	glm::vec4 _bounds;
	float rotation, scale;
};

#endif// !TRANSFORM_H