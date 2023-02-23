#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
//#include "CameraController.h"

class Camera
{
public:
	Camera(glm::vec4 viewPort);
	virtual ~Camera();

	virtual glm::mat4 GetViewMatrix();
	virtual glm::mat4 getProjectionMatrix();
	virtual glm::vec2 GetScreenExtents();
	virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode);
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y);
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void debug();
	void Update(float dt) { onUpdate(dt); };

	virtual void Reset();
	virtual void updateCamVectors();
protected:
	virtual void debugInner();
	virtual void onUpdate(float dt) {}
	virtual glm::mat4 CalculateView();
	virtual glm::mat4 CalculateProjection();

public:
	bool Keys[1024];
	virtual glm::vec2 convertScreenToWorld(const glm::vec2& mousePos);
	int Width, Height; //width and height of screen
public:
	glm::vec3 position, right, up, front;
	float zoom;
protected:
	glm::vec2 screenCenter, mousePoint, mousePointSS;
	float nearPlane, farPlane ;

	bool rightMouseDown, leftMouseDown;

	glm::mat4 view, projection;


	friend class CameraController;
};
#endif // !CAMERA_H

