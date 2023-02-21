#pragma once
#include "GameObject.h"
#include "TextRenderer.h"

class GameObject_UI : public GameObject
{
public:
	GameObject_UI();
	GameObject_UI(GameObjectDefinition* definition, TextRenderer* tr = nullptr, Callback* _update = nullptr,  Callback* _select = nullptr, Callback* _hover = nullptr );
	virtual ~GameObject_UI();

	virtual void Draw(Shader* shader, Surface* surface) override;
	virtual void DebugData() override;

	bool selectable, isSelected, isHovered, textVisible, baseVisible;
public:
	virtual void HandleOnSelect();
	virtual void HandleOnHover();
public:
	Callback* onSelect = nullptr, * onHover = nullptr;
	std::string Text;

	glm::vec3 TextCoordOffset; //z value is the scale
	glm::vec4 textDrawColor, textColor, textColorHover, textColorActive;
protected:
	TextRenderer* textDrawerReference;
};

