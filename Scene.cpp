#include "Scene.h"
#include "AppManager.h"


SceneEntry g_Scenes[MAX_APPS] = { {nullptr} };
int g_SceneCount = 0;
int RegisterScene(const char* category, const char* name, SceneCreationFunction* func)
{

	int index = g_SceneCount;
	if (index < MAX_APPS)
	{
		g_Scenes[index] = { category, name, func };
		++g_SceneCount;
		return index;
	}

	return -1;
}
Scene::Scene()
	: m_shader(nullptr)
	, m_surface(nullptr)
{
	m_surface = new quadSurface(); m_surface->Generate();
	m_shader = new Shader("./Shaders/MVP.vert", "./Shaders/SimpleColor.frag");

	ScreenWidth = App::MainCamera->Width;
	ScreenHeight = App::MainCamera->Height;


}

Scene::~Scene()
{
	delete m_shader;
	delete m_surface;
}

void Scene::DrawTitle()
{
}

void Scene::Draw()
{
	m_shader->Use()
		.SetMatrix4("projection", App::MainCamera->getProjectionMatrix())
		.SetMatrix4("view", App::MainCamera->GetViewMatrix())
		.SetColor("color", glm::vec4(0.5, 0.5, 1.0f, 1.0f));

	DrawSceneBackground();
	DrawSceneForeground();
}

void Scene::DrawDebug()
{
}