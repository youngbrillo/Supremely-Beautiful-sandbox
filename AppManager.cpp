#include "AppManager.h"
#include <box2d/box2d.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Scene.h"

Camera* App::MainCamera = nullptr;
App* App::g_App = nullptr;
FileIO::SettingsFile* App::m_settings = nullptr;
App::App(unsigned int width, unsigned int height, GLFWwindow* currentWindow)
	: ClearColor(0.5f, 0.5f, 0.5f, 1.0f)
	, timeSinceLastFrame(0.0f)
	, timeSinceLastFrame_Target(1.0f / 60.0f)
	, m_scene(nullptr)
	, m_Camera(nullptr)
	, shouldExit(false)
	, mainWindow(currentWindow)
	, sceneIndex(0)
	, Keys()
	, screenWidth(width), screenHeight(height)
	, state(AppState::App_running)
{
	m_Camera = new Camera(glm::vec4(0, 0, width, height));
	MainCamera = m_Camera;
	g_App = this;

	ResourceManager::startup();

	sceneIndex = m_settings->m_index;

}

App::~App()
{
	g_App = nullptr;
	MainCamera = nullptr;
	delete m_Camera;

	ResourceManager::cleanup();

}

void App::Initialize()
{
	//init Imgui
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}
	orderSceneEntries();

	if (g_SceneCount)
	{
		m_scene = g_Scenes[sceneIndex].creationFunc();
		restartScene();
	}
}

void App::ProcessInput(GLFWwindow* w, float deltaTime)
{
}

void App::FixedUpdate(float deltaTime)
{
	//if (m_scene) m_scene->Fixed(deltaTime);
	if (!m_scene) return;

	{
		float timestep = 1 / 60.0f;
		m_scene->FixedUpdate(timestep);
	}
}

void App::Update(float deltaTime)
{
	if (state == AppState::App_paused) return;
	timeSinceLastFrame += deltaTime;

	if (timeSinceLastFrame >= timeSinceLastFrame_Target)
	{
		timeSinceLastFrame = 0.0f;
		FixedUpdate(deltaTime);
	}



	if (m_scene) m_scene->Update(deltaTime);
}

void App::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//render the scene
	if (m_scene) m_scene->Draw();
	//render the debug
	DebugRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void App::mouseCallback(GLFWwindow* w, int button, int action, int mode)
{
	MainCamera->mouseCallback(w, button, action, mode);
	m_scene->onMouseKeyCallBack(button, action, mode);
}	


void App::mouseCallback_Cursor(GLFWwindow* w, double x, double y)
{
	MainCamera->mouseCallback_Cursor(w, x, y);
	m_scene->onMouseCursorCallback(x, y);

}

void App::mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	MainCamera->mouseScroll_callback(window, xoffset, yoffset);
	m_scene->onMouseScrollCallback(xoffset, yoffset);
}

void App::ResizeWindowCallback(GLFWwindow* w, int width, int height)
{
	//resize app screen width
	screenWidth = width;
	screenHeight = height;
	//resize camera
	MainCamera->Width = width;
	MainCamera->Height = height;
	MainCamera->updateCamVectors();
	//resize scene
	m_scene->ScreenWidth = width;
	m_scene->ScreenHeight = height;
	m_scene->onWindowResize(width, height);
	//save new window size to Settings
	m_settings->m_windowWidth = width;
	m_settings->m_windowHeight = height;
}

void App::KeyCallBack(GLFWwindow* win, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		m_scene->onKeyPress(key);
	}
	if (action == GLFW_RELEASE)
	{
		m_scene->onKeyRelease(key);
	}
	if (action == GLFW_REPEAT)
	{
		m_scene->onKeyPressContinuous(key);
	}



	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		switch (state)
		{
		case App::App_running:
			state = AppState::App_paused;
			printf("Pausing application\n");
			shouldExit = false;
			break;
		case App::App_paused:
			printf("exiting application\n");
			state = AppState::App_exit;
			shouldExit = true;
			break;
		case App::App_exit:
		default:
			break;
		}
	}

}

void App::SetKey(int key, bool isPressed)
{
	Keys[key] = isPressed;
}

void App::DebugRender()
{
#if true
	{
		ImGui::Begin("Code Switch Debug");//, & showUI, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);


		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Show The Demo Window", &ImGui_Demo_visible);
		ImGui::ColorEdit4("Window background Color", &ClearColor.x);

		static int elem = AppState::App_paused; //b/c its first
		const char* elem_names[AppState::App_exit + 1] = { "Paused", "Running", "Exit"};
		const char* elem_name = (elem >= 0 && elem < AppState::App_exit + 1) ? elem_names[elem] : "Unknown";

		elem = this->state;
		if (ImGui::SliderInt("State", &elem, 0, AppState::App_exit, elem_name)) {
			this->state = (AppState)elem;
		}
		if (ImGui::TreeNode("Fixed Update intervals"))
		{
			ImGui::SliderFloat("TSLF (time since last frame)", &timeSinceLastFrame, 0, 1);
			ImGui::SliderFloat("TSLF Target", &timeSinceLastFrame_Target, 0, 1);
			ImGui::TreePop();
		}

		if (ImGui_Demo_visible)
			ImGui::ShowDemoWindow(&ImGui_Demo_visible);

		ImGui::Separator();


		if (ImGui::BeginTabBar("ControlTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Controls"))
			{

				ImGui::Separator();

				ImVec2 button_sz = ImVec2(-1, 0);
				std::string pausState = state == AppState::App_paused ? "Resume (P)" : "Pause (P)" ;
				if (ImGui::Button(pausState.c_str(), button_sz))
				{
					state = state == AppState::App_paused ? AppState::App_running : AppState::App_paused;
				}
				if (state == AppState::App_paused)
				{
					/*if (ImGui::Button("Step (Ctrl + R Arrow)", button_sz))
					{
						if (m_scene->world != nullptr)
						{
							float timestep = 1 / 60.0f;

							int velocityIterations = 8, positionIterations = 3;
							m_scene->world->Step(timestep, velocityIterations, positionIterations);
						}
					}*/
				}

				if (ImGui::Button("Restart (R)", button_sz))
				{
					restartScene();
				}

				if (ImGui::Button("Quit (Q)", button_sz))
				{
					if (mainWindow != nullptr)
						glfwSetWindowShouldClose(mainWindow, GL_TRUE);
				}

				ImGui::Separator();

				if (m_scene) {
					m_scene->DrawDebug();
				}

				ImGui::EndTabItem();
			}

			m_Camera->debug();
			RenderSceneSelection();
			ImGui::EndTabBar();
		}

		ImGui::End();

	}
#else


#endif
}

void App::RenderSceneSelection()
{
	ImGuiTreeNodeFlags leafNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	leafNodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (ImGui::BeginTabItem("Scenes"))
	{
		int categoryIndex = 0;
		const char* category = g_Scenes[categoryIndex].category;
		int i = 0;
		while (i < g_SceneCount)
		{
			bool categorySelected = strcmp(category, g_Scenes[sceneIndex].category) == 0;
			ImGuiTreeNodeFlags nodeSelectionFlags = categorySelected ? ImGuiTreeNodeFlags_Selected : 0;
			bool nodeOpen = ImGui::TreeNodeEx(category, nodeFlags | nodeSelectionFlags);

			if (nodeOpen)
			{
				while (i < g_SceneCount && strcmp(category, g_Scenes[i].category) == 0)
				{
					ImGuiTreeNodeFlags selectionFlags = 0;
					if (sceneIndex == i)
					{
						selectionFlags = ImGuiTreeNodeFlags_Selected;
					}
					ImGui::TreeNodeEx((void*)(intptr_t)i, leafNodeFlags | selectionFlags, "%s", g_Scenes[i].name);
					if (ImGui::IsItemClicked())
					{
						sceneIndex = i;
						restartScene();
					}
					++i;
				}
				ImGui::TreePop();
			}
			else
			{
				while (i < g_SceneCount && strcmp(category, g_Scenes[i].category) == 0)
				{
					++i;
				}
			}

			if (i < g_SceneCount)
			{
				category = g_Scenes[i].category;
				categoryIndex = i;
			}
		}
		ImGui::EndTabItem();
	}
}

void App::restartScene()
{
	printf("Starting Scene %d: '%s'\n", sceneIndex, g_Scenes[sceneIndex].name);
	if(m_scene)
		delete m_scene;
	m_Camera->Reset();
	m_scene = g_Scenes[sceneIndex].creationFunc();
	m_settings->m_index = sceneIndex;
	state = AppState::App_running;
}

#include <algorithm>
#include <vector>
void App::orderSceneEntries()
{
	//sort in Alpha-beta by name
	std::sort(g_Scenes, g_Scenes + g_SceneCount, [](const SceneEntry& lhs, const SceneEntry& rhs)
		{return lhs.name < rhs.name; });
	//sort in Alpha-beta by category
	std::sort(g_Scenes, g_Scenes + g_SceneCount, [](const SceneEntry& lhs, const SceneEntry& rhs)
		{return lhs.category < rhs.category; });

}

int App::GetScene(const char* category, const char* title)
{
	for (int i = 0; i < g_SceneCount; i++)
	{

		if (g_Scenes[i].category == category && g_Scenes[i].name == title)
		{
			return i;
		}
	}

	return -1;
}

void App::LoadScenei1(const int& index)
{
	App::g_App->state == AppState::App_paused;

	if (index >= g_SceneCount || index < 0) {
		printf("App.cpp::LoadScene\tprovided scene index (%d) is out-of-bounds (%d)\n", index, g_SceneCount);
		return;
	}
	App::g_App->sceneIndex = index;
	App::g_App->restartScene();
}

void App::LoadScenec2(const char* category, const char* title)
{
	App::LoadScenei1( App::GetScene(category, title) );
}
