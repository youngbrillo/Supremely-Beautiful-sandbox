#include "Scene.h"
#include <imgui.h>

#include "GameObject_UI.h"

class HelloWorldScene : public Scene
{
private:
	TextRenderer tex;
	Shader* canvasShader;
public:
	HelloWorldScene()
	{

		//glm::mat4 projection = glm::ortho(0.0f, ScreenWidth, 0.0f, ScreenHeight, -10.00f, 10.0f);
		//canvasShader = new Shader("./Shaders/UIShader.vert", "./Shaders/SimpleColor.frag");
		//canvasShader->Use().SetMatrix4("projection", projection);
	}
	~HelloWorldScene()
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
		tex.DrawText("Hello world", ScreenWidth/2, ScreenHeight/2, 1.0f);
	};
	virtual void DrawSceneBackground() override
	{

	};
	virtual void DrawDebug() override
	{
		tex.Debug();
	}

public:
	static Scene* Create() { return new HelloWorldScene; }
};


static int testIndex = RegisterScene("HelloWorld", "Scene A", HelloWorldScene::Create);



