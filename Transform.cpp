#include "Transform.h"

Transform::Transform()
	: position(glm::vec3(0.0f))
	, size(glm::vec3(1.0f))
	, pivot(glm::vec3(-0.5f, -0.5f, 0.0f))
	, orientation(glm::vec3(0.0f, 0.0f, 1.0f))
	, m_model(glm::mat4(1.0f))
	, rotation(0.0f)
	, scale(1.0f)
	, direction(1)
	, _bounds(0, 0,  1, 1)
{
}


Transform::~Transform()
{
}

glm::mat4& Transform::Model()
{
	UpdateMatrix();

	return m_model;
}

void Transform::UpdateMatrix()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, pivot);
	{
		m_model = glm::translate(m_model, position);
		m_model = glm::rotate(m_model, rotation, orientation);
		m_model = glm::scale(m_model, glm::vec3(size.x * scale * direction.x, size.y * scale * direction.y, size.z * scale));
	}
	m_model = glm::translate(m_model, -pivot);


	//calculate bounds
	//assume that the object is centered therefore the bounds will be form the center of the object
	float paX = -0.5f, paY = 0.5;
	float x1 = position.x;
	float y1 = position.y;
	float x2 = position.x + (scale * size.x);
	float y2 = position.y + (scale * size.y);
	_bounds = glm::vec4(x1, y1, x2, y2);

}

#include <imgui.h>
#include "AppManager.h"
void Transform::Debug(const char* owner)
{
	if (ImGui::TreeNode(owner))
	{
		
		if (ImGui::SliderFloat3("pos", &position.x,-50, 50)) { UpdateMatrix(); }
		if (ImGui::SliderFloat3("pivot", &pivot.x, -0.5, 0.5)) {

			position += pivot;
			position -= pivot;
			UpdateMatrix();
		}
		if (ImGui::SliderAngle("rotation", &rotation)) { UpdateMatrix(); }
		if (ImGui::SliderFloat3("orientation", &orientation.x, -1, 1)) { UpdateMatrix(); }
		if (ImGui::SliderFloat("scale", &scale, 0.01, 10)) { UpdateMatrix(); }
		if (ImGui::SliderFloat3("size", &size.x, -10, 10)) { UpdateMatrix(); }
		if (ImGui::SliderInt2("direction", &direction.x, -1, 1)) { UpdateMatrix(); }


		ImGui::SliderFloat4("bounds", &_bounds.x, 0,0);
		ImGui::TreePop();
	}

}

bool Transform::isInBounds(const glm::vec2& pos)
{
	if (pos.x < _bounds.x || pos.x > _bounds.z) return false;
	if (pos.y < _bounds.y || pos.y >_bounds.w) return false;

	return true;
}
