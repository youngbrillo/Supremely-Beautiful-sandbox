#include "Scene.h"
#include <imgui.h>

#include "GameObject_UI.h"
#include <vector>
#include "Model.h"

class MeshScene : public Scene
{
private:
	TextRenderer tex;
	Shader* canvasShader;
	std::vector<GameObject*>canvasObjects;
	Model* m_Model;
public:
	MeshScene() : Scene()
	{

		glm::mat4 projection = glm::ortho(0.0f, ScreenWidth, 0.0f, ScreenHeight, -10.00f, 10.0f);
		canvasShader = new Shader("./Shaders/UIShader.vert", "./Shaders/SimpleColor.frag");
		canvasShader->Use().SetMatrix4("projection", projection);

		
		m_shader = new Shader("./Shaders/Model00.vert", "./Shaders/Model00.frag");
		m_Model = new Model("./assets/models/spider/spider.obj");
		//m_Model = new Model("./assets/models/WusonOBJ.obj");
		//m_Model = new Model("./assets/models/Irvine/Irvine.obj");
		//m_Model = new Model("./assets/models/Pine/Pine.fbx");
		//m_Model = new Model("./assets/models/backpack/Survival_BackPack_2.fbx");
		//m_Model = new Model("./assets/models/Michelle.fbx");
		m_Model = new Model("./assets/models/hero/hero_Tpose.obj");

		
		m_Model->transform.pivot = glm::vec3(0.0f);
		m_Model->transform.orientation = glm::vec3(0.0f, 1.0f,0.0f);
		//m_Model->transform.scale = 0.5f;
		m_Model->transform.UpdateMatrix();
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
		App::MainCamera->zoom = 2.6f;

	}
	~MeshScene()
	{
		for (auto g : canvasObjects) { delete g; g = nullptr; }

		delete m_Model;
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
		m_Model->Draw(m_shader);
	};
	virtual void DrawSceneForeground() override
	{
		for (auto i : canvasObjects) i->Draw(canvasShader, m_surface);
	};

	int surfaceIndex = 3, textureIndex = 0;
	virtual void DrawDebug() override
	{
		if (ImGui::Button("Hot Reload Shader"))
		{
			delete m_shader;
			m_shader = new Shader("./Shaders/Model00.vert", "./Shaders/Model00.frag");
		}
		m_Model->Debug();

		tex.Debug();
		ImGui::Separator();
		for (auto i : canvasObjects) i->Debug();


	}
	bool linesEnabled = false, cullface = false;
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


		if (key == GLFW_KEY_Q)
		{
			cullface = !cullface;
			if (cullface)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);

		}
	};

public:
	static Scene* Create() { return new MeshScene; }
};


static int testIndex = RegisterScene("Showcase", "Draw imported Meshes", MeshScene::Create);



