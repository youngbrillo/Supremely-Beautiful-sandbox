#include "Scene.h"
#include <imgui.h>
#include "TextRenderer.h"
#include "GameObject.h"
#include "Quad.h"
#include "Sphere.h"


class LightTestScene00 : public Scene
{
private: 
	TextRenderer m_textrenderer;
	Shader* k_shader;
	Texture* t_texture;
	Transform m_transform;
	glm::vec4 color;

	GameObject* lightSourceObj;
	float specularStrength = 0.5f, ambientStrength = 0.5f;

	SB::SBMesh* mesh;
public:
	LightTestScene00() : Scene()
	{
		

		k_shader = new Shader("./Shaders/MVP.vert", "./Shaders/SimpleColor2.frag");
		k_shader->Use().setInt("image", 0);

		t_texture = ResourceManager::GetTexture("icon");
		color = glm::vec4(1.0f);



		GameObjectDefinition def;
		def.name = "Light Source";
		def.T.pivot = glm::vec3(0.0f);
		def.T.position = glm::vec3(-3.0f, 0.0f, 0.0f);
		//def.T.orientation = glm::vec3(0, 1, 0);
		def.T.orientation = glm::vec3(1.0f, 0.3f, 0.5f);
		def.textureReference = ResourceManager::GetTexture("default");
		def.ColorMain = glm::vec4(1.0f);

		lightSourceObj = new GameObject(&def);

		m_surface = new cubeSurface(); m_surface->Generate();

		//mesh = new SB::Quad(0.5f, 0.5f);
		mesh = new SB::Sphere(12, 12);

	}
	~LightTestScene00()
	{
		delete lightSourceObj;
		//delete k_shader;
		t_texture = nullptr;
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
		m_shader->Use();
		lightSourceObj->Draw(m_shader, mesh);

		glm::mat4 projection = App::MainCamera->getProjectionMatrix();
		glm::mat4 view = App::MainCamera->GetViewMatrix();
		k_shader->Use().SetMatrix4("projection", projection).SetMatrix4("view", view)
			.SetMatrix4("model", m_transform.m_model)
			.SetVector4f("lightColor", lightSourceObj->Color)
			.SetColor("color", color)
			.SetVector3f("lightPosition", lightSourceObj->transform.position)
			.setFloat("ambientStrength", ambientStrength)
			.setFloat("specularStrength", specularStrength)
			.SetVector3f("viewPos", App::MainCamera->position);

		m_shader->Use()
			.SetMatrix4("model", m_transform.m_model)
			.SetColor("color", color);

		t_texture->Bind();
		mesh->Draw();
		//m_surface->Bind();

		m_textrenderer.DrawText("Test for basic lighting", 25.0f, ScreenHeight - 36.0f, 0.5f);
		m_textrenderer.DrawText("1.) Update Mesh class to handle primative meshes like spheres and toruses", 25.0f, ScreenHeight - 2*36.0f, 0.5f);
	};
	virtual void DrawSceneBackground() override
	{

	};
	virtual void DrawDebug() override
	{
		lightSourceObj->Debug();

		m_transform.Debug("test transform");
		ImGui::ColorEdit4("color", &color.x);
		ImGui::SliderFloat("ambient", &ambientStrength, 0, 1.0f);
		ImGui::SliderFloat("specular", &specularStrength, 0, 1.0f);
	}

public:
	static Scene* Create() { return new LightTestScene00; }
};


static int testIndex = RegisterScene("Light ", "Basic: Primative Mesh test", LightTestScene00::Create);



