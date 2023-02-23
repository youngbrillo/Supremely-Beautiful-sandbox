#include "Scene.h"
#include <imgui.h>


class TemplateScene : public Scene
{
public:
	TemplateScene() : Scene()
	{

	}
	~TemplateScene()
	{

	}

public: //frame updates
	virtual void Update(float deltaTime)
	{

	}
	virtual void FixedUpdate(float deltaTime)
	{

	}
public: //handle Inputs
	virtual void onKeyPress(int key)
	{

	}
	virtual void onKeyRelease(int key)
	{

	}
	virtual void onKeyPressContinuous(int key)
	{

	}
	virtual void onMouseKeyCallBack(int mouseButton, int action, int mode) 
	{

	}
	virtual void onMouseCursorCallback(double xPos, double yPos)
	{

	}
	virtual void onMouseScrollCallback(double xOffset, double yOffset)
	{

	}
	virtual void onWindowResize(int newWidth, int newHeight)
	{

	}

public: //rendering
	virtual void DrawSceneForeground() override
	{

	};
	virtual void DrawSceneBackground() override
	{

	};
	virtual void DrawDebug() override
	{

	}

public:
	static Scene* Create() { return new TemplateScene; }
};


//static int testIndex = RegisterScene("Default", "Scene A", TemplateScene::Create);



