#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
class FPSCam
{
public:
	FPSCam(glm::vec3 startingPosition = glm::vec3(0.0f, 0.0f, 3.0f));
	~FPSCam();

public:
	void reset();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void CaptureMouseMovement(float xPos, float yPos);
	void ChangeZoom(float offset);
	void HandleKey(int key, bool isDown);
	void Update(float dt);
	void MoveCam(float dt);


	void Debug(const char* title = "FPS Camera");
public:
	glm::vec3 SPos, Position, Front, Up;

	bool firstMouse = true, movementEnabled = false, isOrtho = false;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;
	float nearPlane = 0.1f, farPlane = 100.0f;

	float Swidth, Sheight;
	bool mLeft = false, mRight = false, mDown = false, mUp = false, mFwd = false, mBack = false;
	bool lshiftdown = false, rshiftdown = false;
	float camSpeed = 5.50f;
};

