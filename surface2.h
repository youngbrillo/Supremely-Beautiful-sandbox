#pragma once
#include "Surface.h"


struct texturedVertex
{
	glm::vec3 Position;
	glm::vec2 TextureCoords;
};

class TriangleSurface : public Surface
{
public:
	TriangleSurface() : Surface() {};
	~TriangleSurface() { Surface::~Surface(); }

	virtual void Generate(float min = 0.0f, float max = 1.0f);
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef) { printf("Triangle surface generate func Not Implemented\n"); };
	virtual void Bind();
};


class CircleSurface : public Surface
{
public:
	CircleSurface(int sub = 16) : Surface(), subdivisions(sub), maxSd(16) {};
	~CircleSurface() { Surface::~Surface(); }

	virtual void Generate(float min = 0.0f, float max = 1.0f);
	virtual void Generate(glm::vec2 toprig, glm::vec2 botrig, glm::vec2 botlef, glm::vec2 toplef) { printf("circle surface generate func Not Implemented\n"); };
	virtual void Bind();
public:
	int subdivisions, maxSd;
};
