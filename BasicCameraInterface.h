#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
class BasicCameraInterface
{
public: //public functions
	BasicCameraInterface() {};
	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual glm::vec2 GetScreenExtents() = 0;
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) = 0;
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) = 0;;
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
	virtual void debug() = 0;
	virtual void Update(float dt) = 0;
	virtual void Reset() = 0;
	virtual void updateCamVectors() = 0;

	virtual glm::vec2 convertScreenToWorld(const glm::vec2& mousePos) = 0;
protected: //protected functions




};

