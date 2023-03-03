#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

enum CAMERA_MOVEMENT {
	CAMERA_FORWARD,
	CAMERA_BACK,
	CAMERA_LEFT,
	CAMERA_RIGHT,
	CAMERA_UP,
	CAMERA_DOWN,
};
class BasicCameraInterface
{
public: //public functions
	BasicCameraInterface()

	{}
	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual glm::vec2 GetScreenExtents() = 0;
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) = 0;
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) = 0;;
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
	virtual void HandleKey(int key, bool isDown) = 0;
	virtual void debug() = 0;
	virtual void Update(float dt) = 0;
	virtual void Reset() = 0;
	virtual void updateCamVectors() = 0;

	virtual glm::vec2 convertScreenToWorld(const glm::vec2& mousePos) = 0;
protected: //protected functions

public:
	glm::mat4 Projection;
	glm::mat4 View;

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

	float FOV;
	float Aspect;
	float Near;
	float Far;
	bool  Perspective;


};

