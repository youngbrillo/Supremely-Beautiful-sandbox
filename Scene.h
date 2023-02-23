#pragma once

#include "Shader.h"
#include "Surface.h"
#include "AppManager.h"


class Scene
{
public:
	Scene();
	virtual ~Scene();
	
public: //frame updates
	virtual void Update(float deltaTime) {};
	virtual void FixedUpdate(float deltaTime) {};
public: //rendering
	void DrawTitle();
	virtual void Draw();
	virtual void DrawSceneForeground() {};
	virtual void DrawSceneBackground() {};
	virtual void DrawDebug();
public: //handle Inputs
	virtual void onKeyPress(int key) {};
	virtual void onKeyRelease(int key) {};
	virtual void onKeyPressContinuous(int key){};
	virtual void onMouseKeyCallBack(int mouseButton, int action, int mode) {};
	virtual void onMouseCursorCallback(double xPos, double yPos) {};
	virtual void onMouseScrollCallback(double xOffset, double yOffset) {};
	virtual void onWindowResize(int newWidth, int newHeight) {};
public: //physics
protected:
	Shader* m_shader;
	Surface* m_surface;
protected:
	friend class App;
public:
	float ScreenWidth, ScreenHeight;
};


#define MAX_APPS 256
typedef Scene* SceneCreationFunction();
int RegisterScene(const char* category, const char* name, SceneCreationFunction* func);

struct SceneEntry
{
	const char* category;
	const char* name;
	SceneCreationFunction* creationFunc;
};

extern SceneEntry g_Scenes[MAX_APPS];
extern int g_SceneCount;


