#include "Scene.h"
#include <imgui.h>
#include "GameObject.h"
#include "PerspectiveCamera.h"

class PerspectiveCamScene : public Scene
{
public: 
	GameObject* m_object;
public:
	PerspectiveCamScene() : Scene()
	{
		GameObjectDefinition def;
		def.name = "Unlit Cube";
		def.T.pivot = glm::vec3(0.0f);
		def.T.orientation = glm::vec3(0, 1, 0);
		def.ColorMain = glm::vec4(0, 1.0f, 0.8f, 1.0f);
		def.textureReference = ResourceManager::GetTexture("default");
		m_object = new GameObject(&def);

		m_surface = new cubeSurface(); m_surface->Generate();

		App::g_App->MainCamera = new PerspectiveCamera(glm::vec4(0, 0, ScreenWidth, ScreenHeight));
	}
	~PerspectiveCamScene()
	{
		delete m_object;
		App::g_App->MainCamera = new Camera(glm::vec4(0, 0, ScreenWidth, ScreenHeight));
	}

public: //frame updates
	virtual void Update(float deltaTime)
	{
		m_object->Update(deltaTime);
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
		m_object->Draw(m_shader, m_surface);
	};
	virtual void DrawSceneBackground() override
	{

	};
	virtual void DrawDebug() override
	{
		m_object->Debug();
	}

public:
	static Scene* Create() { return new PerspectiveCamScene; }
};


static int testIndex = RegisterScene("Showcase", "Perspective View", PerspectiveCamScene::Create);



