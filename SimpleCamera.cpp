#include "SimpleCamera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//#include <glad/glad.h>

Camera::Camera(glm::vec4 viewPort) :
	Width((int)viewPort.z), Height((int)viewPort.w)
	, screenCenter(viewPort.z / 2, viewPort.w / 2)
	, position(0, 0, 0)
	, zoom(1.0f)
	, nearPlane(-50.0f), farPlane(50.0f)
	, rightMouseDown(false), leftMouseDown(false)
	, mousePoint(0.0f), mousePointSS(0.0f)
	, right(1.0f, 0, 0)
	, up(0, 1, 0)
	, front(0, 0, -1)
	, Keys()
{
	//printf("Camera.cpp::Camera():: initializing Camera (%d x %d)\n", Width, Height);
	CalculateProjection();
	CalculateView();
	updateCamVectors();
}

void Camera::Reset()
{
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->position = glm::vec3(0, 0, 0);
	this->zoom = 1.0f;
	this->mousePoint = glm::vec2(0.0f);
	mousePointSS = mousePoint;
	this->nearPlane = -51.0f;
	this->farPlane = 51.0f;
	this->updateCamVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	return CalculateView();
}

glm::mat4 Camera::getProjectionMatrix()
{
	return CalculateProjection();
}

glm::vec2 Camera::GetScreenExtents()
{
	glm::vec2 A(0.0f), C(this->Width, this->Height);
	glm::vec2 AA = convertScreenToWorld(A);
	glm::vec2 CC = convertScreenToWorld(C);
	//glm::vec2 result = glm::vec2(abs(wA.x) + abs(wC.x), abs(wA.y) + abs(wC.y));
	glm::vec2 result = glm::vec2(abs(CC.x - AA.x), abs(CC.y - AA.y));
	return result;
}

void Camera::mouseCallback(GLFWwindow* window, int button, int action, int mode)
{
	if (ImGui::GetIO().WantCaptureMouse) return;


	double xd, yd;
	glfwGetCursorPos(window, &xd, &yd);
	glm::vec2 ps((float)xd, (float)yd);
	glm::vec2 rcmp = ps;
	if (button == GLFW_MOUSE_BUTTON_2)
	{
		if (action == GLFW_PRESS)
		{
			rcmp = this->convertScreenToWorld(ps);
			rightMouseDown = true;
			mousePoint = rcmp;

		}
		if (action == GLFW_RELEASE)
		{
			rightMouseDown = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		if (action == GLFW_PRESS)
		{
			leftMouseDown = true;
		}
		if (action == GLFW_RELEASE)
		{
			leftMouseDown = false;
		}
	}
}

void Camera::mouseCallback_Cursor(GLFWwindow* w, double x, double y)
{
	mousePointSS = glm::vec2(x, y);


	if (rightMouseDown)
	{

		glm::vec2 ps((float)x, (float)y);
		glm::vec2 pw = convertScreenToWorld(ps);

		glm::vec2 diff = pw - mousePoint;
		position.x -= diff.x;
		position.y -= diff.y;
		mousePoint = convertScreenToWorld(ps);
	}
}

void Camera::mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (ImGui::GetIO().WantCaptureMouse) return;


	if (yoffset > 0) {
		zoom /= 1.1f;
	}
	else {
		zoom *= 1.1f;
	}

	return;
	float clamMin = 0.25, clampMax = 2.25;
	if (zoom > clampMax)
		zoom = clampMax;

	if (zoom < clamMin)
		zoom = clamMin;
}


glm::mat4 Camera::CalculateView()
{

	glm::mat4 _view = glm::mat4(1.0f);
	_view = glm::translate(_view, position);
	_view = glm::lookAt(position, position + front, up);

	view = _view;
	return _view;
}

glm::mat4 Camera::CalculateProjection()
{
	float ratio = Width / Height;

	glm::mat4 _projection = glm::ortho(
		-Width * ratio * glm::radians(zoom), //left
		Width * ratio * glm::radians(zoom), //right
		-Height * ratio * glm::radians(zoom), //bottom
		Height * ratio * glm::radians(zoom), //top
		nearPlane,
		farPlane
	);

	//what if perspective??

	projection = _projection;
	return _projection;
}

glm::vec2 Camera::convertScreenToWorld(const glm::vec2& mousePos)
{
	//required variables
	glm::mat4 projection = getProjectionMatrix();
	glm::mat4 view = GetViewMatrix();
	glm::vec4 output;

	glm::vec4 NDC;

	{ //get normalized device coords
		float w, h, x, y, ndc_x, ndc_y;
		w = Width * 1.0f;
		h = Height * 1.0f;
		x = mousePos.x;
		y = mousePos.y;



		ndc_x = 2.0 * x / w - 1.0;
		ndc_y = 1.0 - 2.0 * y / h; //invert y axis

		NDC = glm::vec4(ndc_x, ndc_y, 0, 1);
	}

	{
		output = NDC * glm::inverse(projection) * glm::inverse(view);
		{
			output.x += position.x;
			output.y += position.y;
		}
	}

	return glm::vec2(output.x, output.y);
}

void Camera::debug()
{
	//if (ImGui::TreeNode("Camera"))
	if (ImGui::BeginTabItem("Camera"))
	{
		debugInner();

		ImGui::EndTabItem();
		//ImGui::TreePop();
	}
}

void Camera::debugInner()
{
	ImGui::Checkbox("right mouse btn down", &rightMouseDown);
	ImGui::Checkbox("left mouse btn down", &leftMouseDown);
	ImGui::SliderFloat3("cam Pos", &position.x, -25, 25);
	ImGui::SliderFloat3("front", &front.x, -1, 1);
	ImGui::SliderFloat3("up", &up.x, -1, 1);
	ImGui::SliderFloat3("right", &right.x, -1, 1);
	ImGui::SliderFloat("zoom", &zoom, 0, 25);
	ImGui::SliderFloat("Near plane", &nearPlane, 0.01, 100);
	ImGui::SliderFloat("Far  Plane", &farPlane, 0.01, 100);
}

void Camera::updateCamVectors()
{

}
