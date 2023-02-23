#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "SimpleCamera.h"
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STILL
};


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(glm::vec4 ViewPort);
    ~PerspectiveCamera() {};
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 CalculateView() override;
    glm::mat4 CalculateProjection() override;

    virtual void mouseCallback(GLFWwindow* window, int button, int action, int mode) override;
    virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y) override;
    virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void onUpdate(float dt);
    //// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
protected:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCamVectors() override;
    //// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    void debugInner() override;

public:
    // camera Attributes
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw, Pitch;
    // camera options
    float MovementSpeed, MouseSensitivity, DragSensitivity;
};
#endif