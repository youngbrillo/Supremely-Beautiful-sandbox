#include "GameObject_UI.h"
#include <imgui.h>

GameObject_UI::GameObject_UI() 
	: GameObject()
	, selectable(false)
	, isSelected(false)
	, isHovered(false)
	, textVisible(true)
	, baseVisible(true)
	, textDrawerReference(nullptr)
	, Text("")
{
	name = "UI GameObject";
	transform.position.z = 0.5f;
	//transform.UpdateMatrix();
}

GameObject_UI::GameObject_UI(GameObjectDefinition* definition, TextRenderer* ref, Callback* _update,  Callback* _select, Callback* _hover)
	: GameObject(definition, _update)
	, selectable(definition->ui_isSelectable)
	, isSelected(false)
	, isHovered(false)
	, textVisible(true)
	, baseVisible(false)
	, textDrawerReference(ref)
	, Text(definition->ui_Text)
	, TextCoordOffset(definition->ui_TextOffset)
	, textDrawColor(definition->ui_TextColorMain)
	, textColor(definition->ui_TextColorMain)
	, textColorHover(1.0f, 1.0f, 0.0f ,1.0f)
	, textColorActive(1.0f, 0, 0, 1.0f)
	, onSelect(_select)
	, onHover(_hover)
{
	transform.position.z = 0.5f;
	//transform.UpdateMatrix();

}

GameObject_UI::~GameObject_UI()
{
	if (onSelect) delete onSelect;
	if (onHover) delete onHover;

	onSelect = nullptr; onHover = nullptr;
	textDrawerReference = nullptr;

}
#include "AppManager.h"

void GameObject_UI::Draw(Shader* shader, Surface* surface)
{

	if (!visible) return;
	if (baseVisible) 
	{
		//GameObject::Draw(shader, surface);
		shader->Use()
			.SetMatrix4("model", transform.m_model)
			.SetColor("color", drawColor);

		if (texture) texture->Bind();

		surface->Bind();
	}

	if (selectable)
	{
		if (isHovered) textDrawColor = textColorHover;
		if (isSelected) textDrawColor = textColorActive;

		if(!isHovered && !isSelected) textDrawColor = textColor;
	}

	if (textDrawerReference && textVisible)
	{
		textDrawerReference->DrawText(Text, transform.position.x + TextCoordOffset.x, transform.position.y + TextCoordOffset.y, TextCoordOffset.z, textDrawColor);
	}
	//else  isHovered = true;
}

void GameObject_UI::DebugData()
{
	if (ImGui::TreeNode("Text Config"))
	{
		ImGui::Checkbox("text visible", &textVisible);
		ImGui::SameLine();
		ImGui::Checkbox("selectable", &selectable);
		ImGui::Checkbox("currently selected", &isSelected);
		ImGui::SameLine();
		ImGui::Checkbox("currently hovered", &isHovered);
		ImGui::SliderFloat2("Text offset", &TextCoordOffset.x, 0, App::MainCamera->Height);
		ImGui::SliderFloat("Text size", &TextCoordOffset.z, 0.1, 10);
		ImGui::ColorEdit4("text draw color", &textDrawColor.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("text color", &textColor.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("tex col. Hover", &textColorHover.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("tex col. Active", &textColorActive.x, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Game Object config"))
	{
		ImGui::Checkbox("base class visibility", &baseVisible);
		GameObject::DebugData();
		ImGui::TreePop();
	}
}

void GameObject_UI::HandleOnSelect()
{
	if (!selectable) return;
	if (onSelect)
	{
		onSelect->evaluate();
	}
	else {
		printf("No 'onSelect' Callback found for (%d) %s\n", this->GetId(), this->name.c_str());
	}
}

void GameObject_UI::HandleOnHover()
{
	if (!selectable) return;
	if (onHover)
	{
		onHover->evaluate();
	}
	else
	{
		printf("No 'onHover' Callback found for (%d) %s\n", this->GetId(), this->name.c_str());
	}
}
