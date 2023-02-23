#include "PerspectiveCamera.h"
#include <imgui.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;


PerspectiveCamera::PerspectiveCamera(glm::vec4 ViewPort)
    : Camera(ViewPort)
    , MovementSpeed(SPEED)
    , MouseSensitivity(SENSITIVITY)
    , DragSensitivity(0.01)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    position = glm::vec3(0.0f, 0.0f, 10.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    WorldUp = up;
    Yaw = YAW;
    Pitch = PITCH;
    nearPlane = 0.1f;
    farPlane = 100.0f;
    zoom = FOV;
    updateCamVectors();
}
glm::mat4 PerspectiveCamera::CalculateProjection()
{
    return glm::perspective(glm::radians(zoom), (float)Width / (float)Height, nearPlane, farPlane);
}

glm::mat4 PerspectiveCamera::CalculateView()
{
    return glm::lookAt(position, position + front, up);
}

void PerspectiveCamera::mouseCallback(GLFWwindow* window, int button, int action, int mode)
{
    Camera::mouseCallback(window, button, action, mode);
    if (leftMouseDown)
    {
        screenCenter = mousePointSS;
    }

}
void PerspectiveCamera::mouseCallback_Cursor(GLFWwindow* w, double x, double y)
{
    Camera::mouseCallback_Cursor(w, x, y);
    //if (rightMouseDown)
    //{

    //    glm::vec2 ps((float)x, (float)y);
    //    //glm::vec2 pw = convertScreenToWorld(ps);

    //    glm::vec2 diff = (ps - mousePointSS) * DragSensitivity;
    //    position.x -= diff.x;
    //    position.y -= diff.y;
    //    //mousePoint = convertScreenToWorld(ps);
    //    mousePoint = ps;
    //}

    if (leftMouseDown)
    {
        //screenCenter = glm::vec2(x, y);

        float _xoffset = x - screenCenter.x;
        float _yoffset = screenCenter.y - y; 
        ProcessMouseMovement(_xoffset, _yoffset, true);
    }
}
void PerspectiveCamera::mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    zoom -= (float)yoffset;
    //return;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}
void PerspectiveCamera::onUpdate(float dt)
{
    Camera_Movement mvmnt = STILL;
    if (Keys[GLFW_KEY_W]) mvmnt = Camera_Movement::FORWARD;
    if (Keys[GLFW_KEY_S]) mvmnt = Camera_Movement::BACKWARD;
    if (Keys[GLFW_KEY_A]) mvmnt = Camera_Movement::LEFT;
    if (Keys[GLFW_KEY_D]) mvmnt = Camera_Movement::RIGHT;

    ProcessKeyboard(mvmnt, dt);
}
// returns the view matrix calculated using Euler Angles and the LookAt Matrix

//// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void PerspectiveCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    if (direction == STILL) return;

    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void PerspectiveCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCamVectors();
}
void PerspectiveCamera::debugInner()
{
    ImGui::SliderFloat2("mouse position", &mousePointSS.x, 0, 0.0f);
    ImGui::SliderFloat("mouse sensitivity", &MouseSensitivity, 0, 1.0f);
    ImGui::SliderFloat("drag sensitivity", &DragSensitivity, 0, 1.0f);
    Camera::debugInner();

}

// calculates the front vector from the Camera's (updated) Euler Angles
void PerspectiveCamera::updateCamVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}