#include "Scene.h"
#include <imgui.h>

#include "GameObject_UI.h"
#include <vector>

class MeshScene : public Scene
{
private:
	TextRenderer tex;
	Shader* canvasShader;
	std::vector<GameObject*>canvasObjects;
public:
	MeshScene() : Scene()
	{

		glm::mat4 projection = glm::ortho(0.0f, ScreenWidth, 0.0f, ScreenHeight, -10.00f, 10.0f);
		canvasShader = new Shader("./Shaders/UIShader.vert", "./Shaders/SimpleColor.frag");
		canvasShader->Use().SetMatrix4("projection", projection);


		//ui objects
		{
			GameObjectDefinition def;
			def.textureReference = ResourceManager::GetTexture("default");
			def.name = "Description";
			def.T.scale = 50;
			def.T.position.y = ScreenHeight - 50.0f;
			def.ui_TextColorMain = glm::vec4(0, 1, 1, 1);
			def.ui_Text = "Import Mesh's and Render them to the screen";
			canvasObjects.emplace_back(new GameObject_UI(&def, &tex));
		}


	}
	~MeshScene()
	{
		for (auto g : canvasObjects) { delete g; g = nullptr; }
	}

public: //frame updates
	virtual void Update(float deltaTime)
	{
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
	};
	virtual void DrawSceneForeground() override
	{
		for (auto i : canvasObjects) i->Draw(canvasShader, m_surface);
	};

	int surfaceIndex = 3, textureIndex = 0;
	virtual void DrawDebug() override
	{
		tex.Debug();
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
	static Scene* Create() { return new MeshScene; }
};


static int testIndex = RegisterScene("Showcase", "Draw imported Meshes", MeshScene::Create);



