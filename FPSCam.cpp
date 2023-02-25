#include "FPSCam.h"
#include <imgui.h>
FPSCam::FPSCam(glm::vec3 startingPosition)
	: Position(startingPosition)
	, SPos(startingPosition)
	, Front(0.0f, 0.0f, -1.0f)
	, Up(0.0f, 1.0f, 0.0f)
	, firstMouse(true)
	, movementEnabled(false)
	, isOrtho(false)
	, yaw(- 90.0f)
	, pitch(0.0f)
	, lastX(800.0f / 2.0)
	, lastY(600.0 / 2.0)
	, fov(45.0f)
	, nearPlane (0.1f), farPlane (100.0f)
	, Swidth(0), Sheight(0)

{
}

FPSCam::~FPSCam()
{
}

void FPSCam::reset()
{
	Position = SPos;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	firstMouse = true, movementEnabled = false;
	yaw = -90.0f;	
	pitch = 0.0f;
	lastX = Swidth / 2.0;
	lastY = Sheight / 2.0;
	fov = 45.0f;
	nearPlane = 0.1f, farPlane = 100.0f;
}

glm::mat4 FPSCam::getProjectionMatrix()
{

	float ratio = Swidth / Sheight;
	if (isOrtho)
		return glm::ortho(
			-Swidth * ratio * glm::radians(fov), Swidth * ratio * glm::radians(fov), 
			-Sheight * ratio * glm::radians(fov),  Sheight * ratio * glm::radians(fov),
			nearPlane, farPlane );

	return glm::perspective(glm::radians(fov), ratio, nearPlane, farPlane);
}

glm::mat4 FPSCam::getViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void FPSCam::CaptureMouseMovement(float xPos, float yPos)
{
	if (!movementEnabled) return;

	float xpos = xPos, ypos = yPos;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(front);
}

void FPSCam::ChangeZoom(float offset)
{
	fov -= offset;

	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void FPSCam::HandleKey(int key, bool isDown)
{
	if (key == GLFW_KEY_W) mFwd = isDown;
	if (key == GLFW_KEY_S) mBack = isDown;
	if (key == GLFW_KEY_A) mLeft = isDown;
	if (key == GLFW_KEY_D) mRight = isDown;
	if (key == GLFW_KEY_UP) mUp = isDown;
	if (key == GLFW_KEY_DOWN) mDown = isDown;
	if (key == GLFW_KEY_RIGHT_SHIFT) rshiftdown = isDown;
	if (key == GLFW_KEY_LEFT_SHIFT) lshiftdown = isDown;


}

void FPSCam::Update(float dt)
{
	MoveCam(dt);
}

void FPSCam::MoveCam(float dt)
{

	float cameraSpeed = camSpeed * dt;
	if (mFwd)
		Position += cameraSpeed * Front;
	if (mBack)
		Position -= cameraSpeed * Front;
	if (mLeft)
		Position -= glm::normalize(glm::cross(Front,Up)) * cameraSpeed;
	if (mRight)
		Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
	if (mUp)
		Position += cameraSpeed * Up; 
	if (mDown)
		Position -= cameraSpeed * Up;
}

void FPSCam::Debug(const char* name)
{
	if (ImGui::TreeNode(name))
	{
		ImGui::Checkbox("front", &mFwd);
		ImGui::Checkbox("bac", &mBack);
		ImGui::Checkbox("up", &mUp);
		ImGui::Checkbox("down", &mDown);
		ImGui::Checkbox("lef", &mLeft);
		ImGui::Checkbox("rig", &mRight);
		ImGui::Checkbox("lshif", &lshiftdown);
		ImGui::Checkbox("rshif", &rshiftdown);



		ImGui::Checkbox("Orthographic", &isOrtho);
		ImGui::SliderFloat("camera speed", &camSpeed, 0, 30.f);

		ImGui::Checkbox("enable movement", &movementEnabled);


		if (ImGui::Button("Reset"))reset();
		ImGui::SliderFloat("nearPlane", &nearPlane, -100, farPlane);
		ImGui::SliderFloat("farPlane", &farPlane, nearPlane, 100.0f);

		ImGui::SliderFloat("s.dim.x", &Swidth, 0, 0);
		ImGui::SliderFloat("s.dim.y", &Sheight, 0, 0.0f);


		ImGui::SliderFloat3("Pos", &Position.x, -30, 30);
		ImGui::SliderFloat3("front", &Front.x, -1, 1);
		ImGui::SliderFloat3("Up", &Up.x, -1, 1);

		ImGui::SliderFloat("Yaw", &yaw, -360, 360);
		ImGui::SliderFloat("pitch", &pitch, -360, 360);
		ImGui::SliderFloat("fov", &fov, -360, 360);
		ImGui::TreePop();
	}
}
