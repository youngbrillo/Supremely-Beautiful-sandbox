#include "Scene.h"
#include <imgui.h>
#include "GameObject.h"
#include "FPSCam.h"

#include "Model.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Animator.h"

#include "Quad.h"
#include "Sphere.h"
#include "Cube.h"

class TestCamScene : public Scene
{
private:
	std::vector<GameObject*> objs;
	GameObject* lightSourceObj;
	Shader* k_shader, * model_shader;
	FPSCam m_cam;
	bool forward, backward, left, right;
	bool usetestShader = true;
	bool showMouse = true;
	float _cameraSpeed = 2.5;
	Model* m_Model, *n_Model, *o_Model;

	bool linesEnabled = false, cullface = false;
	bool rotateModel, rotateLight;
	Animator* m_animator;
	Animation* walkAnim, * runAnim, * idleAnim;
	bool shiftHeldDown = false;
	glm::vec3 rotationPoint = glm::vec3(0, 0, 1.0f);

	SB::SBMesh* m_mesh, *k_mesh;
public:
	TestCamScene() 
		: Scene()
		, forward(false), backward(false), left(false), right(false)
		, rotateModel(false), rotateLight(false)
		
	{
		m_cam = FPSCam(glm::vec3(0, 0, 16.5f));
		GameObjectDefinition def;
		def.name = "Light Source";
		def.T.pivot = glm::vec3(0.0f);
		def.T.position = glm::vec3(-5, 0, -5);
		//def.T.orientation = glm::vec3(0, 1, 0);
		def.T.orientation = glm::vec3(1.0f, 0.3f, 0.5f);
		def.textureReference = ResourceManager::GetTexture("default");
		def.ColorMain = glm::vec4(1.0f);

		lightSourceObj = new GameObject(&def);
		def.ColorMain = glm::vec4(0, 1.0f, 0.8f, 1.0f);
		def.name = "Unlit Cube";


		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		for (int i = 0; i < 10; i++)
		{
			def.textureReference = ResourceManager::GetTexture("icon");
			def.T.position = cubePositions[i];
			def.T.rotation = 20.0f * i;
			def.T.UpdateMatrix();
			if(i == 0)
				objs.emplace_back(new GameObject(&def, new Callback(RotateAroundPointWrapper, this)));
			else 
				objs.emplace_back(new GameObject(&def));
		}

		m_surface = new cubeSurface(); m_surface->Generate();


		k_shader = new Shader("./Shaders/MVP.vert", "./Shaders/SimpleColor2.frag");
		k_shader->Use().setInt("image", 0);

		model_shader = new Shader("./Shaders/Model00.vert", "./Shaders/Model01_light.frag");
	

		m_cam.lastX = ScreenWidth / 2.0f; m_cam.lastY = ScreenHeight / 2.0f;
		m_cam.Swidth = ScreenWidth;
		m_cam.Sheight = ScreenHeight;
		m_cam.Position = glm::vec3(1.75f, 5.0f, 11.5f);
		m_cam.fov = 45.0f;
		m_cam.Front = glm::vec3(0, -0.07, -1);
		m_cam.yaw = -90.0f;
		m_cam.pitch = -4.1f;



		//m_Model = new Model("./assets/models/hero/hero.obj");
		m_Model = new Model("./assets/models/michelle/Michelle.dae");

		m_Model->transform.pivot = glm::vec3(0.0f, -0.7f, 0.0f);
		m_Model->transform.pivot = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Model->transform.orientation = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Model->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Model->transform.scale = 0.027f;
		//m_Model->transform.scale = 0.285f;
		//m_Model->transform.scale = 0.01f;
		m_Model->transform.UpdateMatrix();

		idleAnim = new Animation("./assets/models/michelle/Idle.dae", m_Model);
		walkAnim = new Animation("./assets/models/michelle/Walking.dae", m_Model);
		runAnim = new Animation("./assets/models/michelle/FastRun.dae", m_Model);
		//m_animator = new Animator(walkAnim);


		n_Model = new Model("./assets/models/hero/hero.obj");
		n_Model->transform.position = glm::vec3(-3.0f, 0.0f, 0.0f);
		n_Model->transform.scale = 1.0f;
		n_Model->transform.UpdateMatrix();

		o_Model = new Model("./assets/models/Pine/Pine.dae");
		o_Model->transform.position = glm::vec3(3.0f, 0.0f, 0.0f);
		o_Model->transform.scale = 0.250f;
		o_Model->transform.UpdateMatrix();

		m_mesh = new SB::Sphere(12, 12);
		k_mesh = new SB::Cube();
	}
	~TestCamScene()
	{
		for (auto m_object : objs) {
			delete m_object;
			m_object = nullptr;
		}
		delete k_shader;
		delete model_shader;
		delete lightSourceObj;
		delete m_Model;
		delete n_Model;
		delete o_Model;
		//delete m_animator;
		delete walkAnim;
	}

	static void RotateAroundPointWrapper(TestCamScene* i)
	{
		i->RotateAroundPoint();
	}
	void RotateAroundPoint()
	{
		//GameObject* a, * b;
		//a = objs[0]; b = objs[1];

		//a->transform.UpdateMatrix();
		//a->transform.m_model = glm::rota
	}

public: //frame updates
	virtual void Update(float deltaTime)
	{
		if (rotateLight)
		{
			//lightSourceObj->transform.rotation += deltaTime;

			glm::mat4 trans(1.0f);
			float rotInc = 30.0f * deltaTime;
			trans = glm::rotate(trans, glm::radians(rotInc), rotationPoint);
			glm::vec4 result = trans * glm::vec4(lightSourceObj->transform.position, 1.0f);

			lightSourceObj->transform.position = glm::vec3(result.x, result.y, result.z);

			lightSourceObj->transform.UpdateMatrix();

		}

		if (rotateModel)
		{
			m_Model->transform.rotation += deltaTime;
			if (m_Model->transform.rotation > 2* M_PI) m_Model->transform.rotation = 0.0f;
			m_Model->transform.UpdateMatrix();
		}



		lightSourceObj->Update(deltaTime);
		for(auto m_object:objs) m_object->Update(deltaTime);
		m_cam.Update(deltaTime);


		//m_animator->UpdateAnimator(deltaTime);


	}
	virtual void FixedUpdate(float deltaTime)
	{

	}
public: //handle Inputs
	virtual void onKeyPress(int key)
	{
		m_cam.HandleKey(key, true);
		if (key == GLFW_KEY_LEFT_SHIFT) shiftHeldDown = true;
	}
	virtual void onKeyRelease(int key)
	{
		m_cam.HandleKey(key, false);

		if (key == GLFW_KEY_Q)
		{
			showMouse = !showMouse;
			m_cam.movementEnabled = !showMouse;
			if(!showMouse) 
				glfwSetInputMode(App::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(App::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}


		if (key == GLFW_KEY_1)
		{
			linesEnabled = !linesEnabled;
			if (linesEnabled)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		if (key == GLFW_KEY_2)
		{
			cullface = !cullface;
			if (cullface)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);

		}

		if (key == GLFW_KEY_LEFT_SHIFT) shiftHeldDown = false;


	}
	virtual void onKeyPressContinuous(int key)
	{

	}
	virtual void onMouseKeyCallBack(int mouseButton, int action, int mode)
	{

	}
	virtual void onMouseCursorCallback(double xPos, double yPos)
	{
		m_cam.CaptureMouseMovement(xPos, yPos);

		if (shiftHeldDown)
		{
			
		}
	}
	virtual void onMouseScrollCallback(double xOffset, double yOffset)
	{
		if (ImGui::GetIO().WantCaptureMouse) return;
		m_cam.ChangeZoom((float)yOffset);
	}
	virtual void onWindowResize(int newWidth, int newHeight)
	{
		m_cam.Swidth = ScreenWidth;
		m_cam.Sheight = ScreenHeight;
	}
public: //rendering
	//virtual void Draw() override {}
	virtual void DrawSceneForeground() override
	{

		// activate shader

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = m_cam.getProjectionMatrix();
		glm::mat4 view = m_cam.getViewMatrix();
		//if(usetestShader)
		k_shader->Use().SetMatrix4("projection", projection).SetMatrix4("view", view)
			.SetVector4f("lightColor", lightSourceObj->Color)
			.SetVector3f("lightPosition", lightSourceObj->transform.position)
			.setFloat("ambientStrength", ambientLightStrength)
			.setFloat("specularStrength", specularStrength)
			.SetVector3f("viewPos", m_cam.Position);

		for (auto m_object : objs) m_object->Draw(k_shader, m_surface);
		for (int i = 0; i < objs.size(); i++)
		{
			SB::SBMesh* n = i < objs.size() / 2 ? m_mesh : k_mesh;

			objs[i]->Draw(k_shader, n);
		}

		model_shader->Use().SetMatrix4("projection", projection).SetMatrix4("view", view)
			.SetVector4f("lightColor", lightSourceObj->Color)
			.SetVector3f("lightPosition", lightSourceObj->transform.position)
			.setFloat("ambientStrength", ambientLightStrength)
			.setFloat("specularStrength", specularStrength)
			.SetVector3f("viewPos", m_cam.Position);


		n_Model->Draw(model_shader);
		o_Model->Draw(model_shader);
		//auto transforms = m_animator->GetFinalBoneMatrices();
		//for (int i = 0; i < transforms.size(); i++)
		//{
		//	std::string fbmindex = "finalBonesMatrices[" + std::to_string(i) + "]";
		//	model_shader->SetMatrix4(fbmindex.c_str(), transforms[i]);
		//}
		m_Model->Draw(model_shader);


		m_shader->Use().SetMatrix4("projection", projection).SetMatrix4("view", view);
		lightSourceObj->Draw(m_shader, m_surface);


	};
	virtual void DrawSceneBackground() override
	{

	};
	float ambientLightStrength = 1.0f/*.250f*/, specularStrength = 0.5f;
	virtual void DrawDebug() override
	{
		ImGui::Checkbox("Rotate Light", &rotateLight);
		ImGui::Checkbox("Rotate Model", &rotateModel);
		
		ImGui::SliderFloat3("lite.rot.point", &rotationPoint.x, -10, 10.0f);

		//if (ImGui::Button("Idle")) { m_animator->PlayAnimation(idleAnim); }
		//if (ImGui::Button("walk")) { m_animator->PlayAnimation(walkAnim); }
		//if (ImGui::Button("run")) { m_animator->PlayAnimation(runAnim); }



		if (ImGui::Button("Hot reload model shader")) { delete model_shader; model_shader = new Shader("./Shaders/Model00.vert", "./Shaders/Model01_light.frag"); }
		if (ImGui::Button("Hot reload k shader")) { delete k_shader; k_shader = new Shader("./Shaders/MVP.vert", "./Shaders/SimpleColor2.frag"); }
		if (ImGui::Button("Hot reload m shader")) { delete m_shader; m_shader = new Shader("./Shaders/MVP.vert", "./Shaders/SimpleColor.frag"); }


		ImGui::SliderFloat("ambient light factor", &ambientLightStrength, 0, 1.0f);
		ImGui::SliderFloat("specular factor", &specularStrength, 0,1.0f);

		m_Model->Debug("Michelle.dae");
		n_Model->Debug("Hero.obj");
		o_Model->Debug("Pine.dae");
		ImGui::Checkbox("use test Shader", &usetestShader);
		ImGui::SliderFloat("_cameraSpeed", &_cameraSpeed, 0, 30.0f);
		m_cam.Debug();
		lightSourceObj->Debug();
		for (auto m_object : objs) m_object->Debug();
	}

public:
	static Scene* Create() { return new TestCamScene; }
};


static int testIndex = RegisterScene("Tests", "light Test", TestCamScene::Create);



