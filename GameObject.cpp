#include "GameObject.h"
#include <imgui.h>

int GameObject::instances = 0;


GameObject::GameObject()
	: transform()
	, drawColor(1.0f)
	, Color(1.0f)
	, AltColor(1.0f)
	, enabled(true), visible(true)
	, texture(nullptr)
	, deleteFlag(false)
{
	id = instances++;
	//transform.position.z = -0.9;
	transform.UpdateMatrix();
}

GameObject::GameObject(GameObjectDefinition* definition, Callback* _update)
	: transform(definition->T)
	, drawColor(definition->ColorMain)
	, Color(definition->ColorMain)
	, AltColor(definition->ColorAlt)
	, texture(definition->textureReference)
	, name(definition->name)
	, enabled(true), visible(true), deleteFlag(false)
	, updateCallback(_update)
{
	id = instances++;
	//transform.position.z = -0.9;
	transform.UpdateMatrix();
}

GameObject::~GameObject()
{
	texture = nullptr;
	if (updateCallback)
	{
		delete updateCallback;
	}
	updateCallback = nullptr;
}

void GameObject::FixedUpdate(float dt)
{
	if (!enabled) return;
}

void GameObject::Update(float dt)
{
	if (!enabled) return;
	if (updateCallback) {
		updateCallback->evaluate();
	}

}
void GameObject::Draw(Shader* _shader, Surface* _surface)
{
	if (!visible) return;
	_shader->Use()
		.SetMatrix4("model", transform.m_model)
		.SetColor("color", drawColor);

	if (texture) texture->Bind();

	_surface->Bind();
}

void GameObject::Debug()
{

	std::string _name;
	_name = std::to_string(id); _name += ": " + name;

	if (ImGui::TreeNode(_name.c_str()))
	{
		DebugData();
		ImGui::TreePop();
	}
}

void GameObject::DebugData()
{
	ImGui::Checkbox("Visible", &visible);
	ImGui::SameLine();
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::SameLine();
	ImGui::Checkbox("Delete", &deleteFlag);
	ImGui::ColorEdit4("draw color", &drawColor.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
	ImGui::ColorEdit4("color", &Color.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
	ImGui::ColorEdit4("Alt color", &AltColor.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
	transform.Debug("Transform");
	if (texture) texture->RenderDebug();
}
