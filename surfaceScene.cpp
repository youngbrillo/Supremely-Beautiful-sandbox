#include "Scene.h"
#include <imgui.h>

#include "GameObject_UI.h"
#include <vector>
#include "Surface2.h"


class HelloWorldScene : public Scene
{
private:
	TextRenderer tex;
	Shader* canvasShader;
	std::vector<GameObject*> m_gameObjects, canvasObjects;
	Surface* k_surface;
	float rotationSpeed = 0.0f;

	CircleSurface* circleSurface;
public:
	HelloWorldScene() : Scene()
	{

		glm::mat4 projection = glm::ortho(0.0f, ScreenWidth, 0.0f, ScreenHeight, -10.00f, 10.0f);
		canvasShader = new Shader("./Shaders/UIShader.vert", "./Shaders/SimpleColor.frag");
		canvasShader->Use().SetMatrix4("projection", projection);

		//screen objects
		{
			GameObjectDefinition def;
			def.name = "circle";
			def.textureReference = ResourceManager::GetTexture("default");
			def.T.pivot = glm::vec3(0.0f);
			def.T.scale = 0.5f;
			def.T.orientation = glm::vec3(1, 1, 0.0f);
			
			//def.ColorMain = glm::vec4(0.f, 0.f, 1.0f, 1.0f);
			m_gameObjects.emplace_back(new GameObject(&def));

			def.name = "Square";
			def.T.position.x = -2.0f;
			def.T.scale = 1.0f;
			def.ColorMain = glm::vec4(0, 1.0f, 0.7f, 1.0f);
			m_gameObjects.emplace_back(new GameObject(&def));

		}
		//ui objects
		{
			GameObjectDefinition def;
			def.textureReference = ResourceManager::GetTexture("default");
			def.name = "Description";
			def.T.scale = 50;
			def.T.position.y = ScreenHeight - 50.0f;
			def.ui_TextColorMain = glm::vec4(1, 1, 0, 1);
			def.ui_Text = "To do: Generate 'Primative' surface meshes like circles and triangles";
			canvasObjects.emplace_back(new GameObject_UI(&def, &tex));
		}

		circleSurface = new CircleSurface(); circleSurface->Generate();
		k_surface = circleSurface;

	}
	~HelloWorldScene()
	{
		for (auto g : m_gameObjects) { delete g; g = nullptr; }
		for (auto g : canvasObjects) { delete g; g = nullptr; }
	}

public: //frame updates
	virtual void Update(float deltaTime)
	{
		m_gameObjects[0]->transform.rotation += deltaTime * rotationSpeed;
		m_gameObjects[0]->transform.UpdateMatrix();

		for (auto i : m_gameObjects) i->Update(deltaTime);
		for (auto i : canvasObjects) i->Update(deltaTime);
	}
public: //handle Inputs
	virtual void onWindowResize(int newWidth, int newHeight)
	{
		glm::mat4 projection = glm::ortho(0.0f, ScreenWidth, 0.0f, ScreenHeight, -10.00f, 10.0f);
		TextRenderer::fontShader->Use().SetMatrix4("projection", projection);
		canvasShader->Use().SetMatrix4("projection", projection);
	}

public: //rendering
	virtual void DrawSceneBackground() override
	{
		for (int i = 0; i <  m_gameObjects.size(); i++)
		{
			Surface* m = i == 0 ? k_surface : m_surface;
			
			m_gameObjects[i]->Draw(m_shader, m);
		}
	};
	virtual void DrawSceneForeground() override
	{
		for (auto i : canvasObjects) i->Draw(canvasShader, m_surface);
	};

	int surfaceIndex = 3, textureIndex = 0;
	const char* surfaceList[4] = { "Cube", "Quad", "Triangle", "Circle" };
	const char* textureList[6] = { "icon", "default", "ship", "astroid1", "healthred", "accelIcon"};
	virtual void DrawDebug() override
	{
		tex.Debug();

		const char* surfaceName = (surfaceIndex >= 0 && surfaceIndex < 4) ? surfaceList[surfaceIndex] : "Unknown";
		const char* texturName = (textureIndex >= 0 && textureIndex < 6) ? textureList[textureIndex] : "Unknown";

		if (ImGui::SliderInt("current Texture", &textureIndex, 0, 5, texturName))
		{
			m_gameObjects[0]->texture = ResourceManager::GetTexture(texturName);
		}
		if (ImGui::SliderInt("K.Surface/Mesh", &surfaceIndex, 0, 3, surfaceName))
		{
			if (k_surface == circleSurface)
				k_surface = nullptr;
			else
				delete k_surface;

			if (surfaceIndex == 0) { k_surface = new cubeSurface();  }
			else if (surfaceIndex == 1) { k_surface = new quadSurface(); }
			else if (surfaceIndex == 2) { k_surface = new TriangleSurface(); }
			else if (surfaceIndex == 3) { k_surface = new CircleSurface(); }
			else
			{
				k_surface = circleSurface;
			}
			k_surface->Generate();
		}
		ImGui::SliderInt("subdivisions", &circleSurface->subdivisions, 0, circleSurface->maxSd);
		ImGui::SliderFloat("rotationSpeed", &rotationSpeed, 0, 5);

		ImGui::Separator();
		for (auto i : m_gameObjects) i->Debug();
		ImGui::Separator();
		for (auto i : canvasObjects) i->Debug();

		
	}
	bool linesEnabled = false;
	void onKeyPress(int key) override
	{
		if (key == GLFW_KEY_TAB)
		{
			linesEnabled = !linesEnabled;
			if (linesEnabled)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	};

public:
	static Scene* Create() { return new HelloWorldScene; }
};


static int testIndex = RegisterScene("Showcase", "Draw Primative Surfaces", HelloWorldScene::Create);



