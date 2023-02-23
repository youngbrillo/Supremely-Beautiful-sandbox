#pragma once
#include <GLFW/glfw3.h>
#include "SimpleCamera.h"
#include "ResourceManager.h"
#include "FileIOoperations.h"
#include "SettingsFile.h"
class Scene; 

class App
{
public:
	enum AppState {
		App_paused	= 0,
		App_running = 1,
		App_exit	= 2
	};


public:
	App(unsigned int width, unsigned int height, GLFWwindow* currentWindow);
	virtual ~App();

	virtual void Initialize();
	virtual void ProcessInput(GLFWwindow*, float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void Render();
public:
	//virtual void KeyCallBack(GLFWwindow* win, int key, int scancode, int action, int mode);
	virtual void mouseCallback(GLFWwindow*, int button, int action, int mode);
	virtual void mouseCallback_Cursor(GLFWwindow* w, double x, double y);
	virtual void mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	virtual void ResizeWindowCallback(GLFWwindow*, int width, int height);
	virtual void KeyCallBack(GLFWwindow* win, int key, int scancode, int action, int mode);
	virtual void SetKey(int key, bool isPressed);
protected:
	virtual void DebugRender();
	void RenderSceneSelection();

	void restartScene();
	void orderSceneEntries();

public:
	static int GetScene(const char* category, const char* title);
	static void LoadScenei1(const int& index);
	static void LoadScenec2(const char* category, const char* title);
	static void QuitToDesktop(){ glfwSetWindowShouldClose(g_App->mainWindow, true); }
	static App* g_App;
	//static Settings g_settings;
public:
	bool hasExited() { return shouldExit; }
	glm::vec4 ClearColor;

private:
	float timeSinceLastFrame,timeSinceLastFrame_Target;
	Scene* m_scene;
	bool shouldExit;
	GLFWwindow* mainWindow;
	int sceneIndex;
	bool Keys[1024];
	unsigned int screenWidth, screenHeight;
public:
	AppState state;

private: //debug
	bool ImGui_Demo_visible = false, showUI = true;

public: 
	static Camera* MainCamera;
	static FileIO::SettingsFile* m_settings;
	static GLFWwindow* GetWindow() { if (g_App) { return g_App->mainWindow; } return nullptr; }
};

