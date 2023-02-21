#pragma once
#include "Transform.h"
#include "Shader.h"
#include "Surface.h"
#include "Texture.h"
#include <string>
#include "Callback.h"
struct GameObjectDefinition
{
	Transform T;
	glm::vec4 ColorMain = glm::vec4(1.0f);
	glm::vec4 ColorAlt = glm::vec4(0.5, 0.5, 0.5, 1.0f);
	glm::vec4 ColorTer = glm::vec4(0.0, 0.0, 0.0, 1.0f);


	Texture* textureReference = nullptr;
	std::string name = "Game Object";

	bool ui_isSelectable = false;
	glm::vec4 ui_TextColorMain = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 ui_TextOffset = glm::vec3(0.0f, 0.0f, 0.75f);

	std::string ui_Text = "PLACEHOLDER TEXT";
};




class GameObject
{
public: //constructors/destructor
	GameObject();
	GameObject(GameObjectDefinition* definition, Callback* _update = nullptr);
	virtual ~GameObject();

public: //member functions
	unsigned int GetId() const { return id; }

	virtual void FixedUpdate(float dt);
	virtual void Update(float dt);
	virtual void Draw(Shader* _shader, Surface* _surface);
	void Debug();
protected:
	virtual void DebugData();
public: //member variables
	Transform transform;
	glm::vec4 drawColor, Color, AltColor;
	bool visible, enabled, deleteFlag;
	Texture* texture;
	std::string name;
public: //callback functions
	void setUpdateCallback(void(*ptr)()) { updateCallback = new Callback(ptr); }
	Callback* updateCallback = nullptr;

private:
	unsigned int id;
	static int instances;
};

