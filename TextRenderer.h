#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
#include <map>
#include <vector>
#include "Shader.h"

struct Character
{
	unsigned int	TextureID;	// id for the glyph texture
	glm::ivec2		Size;		// size of glyph
	glm::ivec2		Bearing;	// offset from baseline to top-left of glyph
	unsigned int	Advance;	// offset to advance to next glyph
	float verticies[6][4] = {};

};

class TextRenderer
{
public:
	//TextRenderer(const char* filepath = "./assets/fonts/DkHandRegular-orna.ttf");
	TextRenderer(const char* filepath = "C:/Windows/Fonts/arial.ttf");

	~TextRenderer();
	void LoadFont(const char* filepath);
	glm::vec2 DrawText2(const char* string, ...);
	glm::vec2 DrawText(const char* string);
	glm::vec2 DrawText(std::string string, const float& xposition, const float& yposition, const float& scale, glm::vec4 color = glm::vec4(1.0f));

	void Debug(const char* name = "Text renderer debug");
public:
	Transform transform;
	glm::vec4 fontColor;

	std::map<char, Character> Characters;

	static Shader* fontShader;
	unsigned int VAO, VBO;

	bool lazyDraw, cachedDraw;
};

