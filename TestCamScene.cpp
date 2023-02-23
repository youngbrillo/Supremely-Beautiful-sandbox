#include "Scene.h"
#include <imgui.h>
#include "GameObject.h"

struct t_Cam
{
	// camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstMouse = true, movementEnabled = false, defaultMats = false;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;
	float nearPlane = 0.1f, farPlane = 100.0f;

	float Swidth, Sheight;

	void reset()
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		firstMouse = true, movementEnabled = false;
		yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		pitch = 0.0f;
		lastX = Swidth / 2.0;
		lastY = Sheight / 2.0;
		fov = 45.0f;
		nearPlane = 0.1f, farPlane = 100.0f;

	}
	void Debug()
	{
		if (ImGui::TreeNode("t cam"))
		{
			
			ImGui::Checkbox("use default matrix", &defaultMats);
			ImGui::Checkbox("enable movement", &movementEnabled);
			if (ImGui::Button("Reset"))reset();
			ImGui::SliderFloat("nearPlane", &nearPlane, -100, farPlane);
			ImGui::SliderFloat("farPlane", &farPlane, nearPlane, 100.0f);

			ImGui::SliderFloat("s.dim.x", &Swidth, 0, 0);
			ImGui::SliderFloat("s.dim.y", &Sheight, 0, 0.0f);


			ImGui::SliderFloat3("Pos", &cameraPos.x, -30, 30);
			ImGui::SliderFloat3("front", &cameraFront.x, -1, 1);
			ImGui::SliderFloat3("Up", &cameraUp.x, -1, 1);

			ImGui::SliderFloat("Yaw", &yaw, -360, 360);
			ImGui::SliderFloat("pitch", &pitch, -360, 360); 
			ImGui::SliderFloat("fov", &fov, -360, 360);
			ImGui::TreePop();
		}
	}

	glm::mat4 getProjection()
	{
		float ratio = Swidth / Sheight;
		if(defaultMats)
			return glm::ortho(
				-Swidth * ratio * glm::radians(fov), //left
				Swidth * ratio * glm::radians(fov), //right
				-Sheight * ratio * glm::radians(fov), //bottom
				Sheight * ratio * glm::radians(fov), //top
				nearPlane,
				farPlane
			);

		return glm::perspective(glm::radians(fov), ratio, nearPlane, farPlane);
	}
	glm::mat4 getView()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	void CaptureMouseMovement(float xPos, float yPos)
	{
		if (movementEnabled)
		{
			float xpos = xPos, ypos = yPos;
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.1f; // change this value to your liking
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(front);
		}
	}
	void Zoom(float offset)
	{
		fov -= offset;
		//if (fov < 1.0f) fov = 1.0f;
		//if (fov > 45.0f) fov = 45.0f;
	}
};


class TestCamScene : public Scene
{
private:
	std::vector<GameObject*> objs;
	Shader* ourShader;
	t_Cam m_cam;
	bool forward, backward, left, right;
	bool usetestShader = true;
	bool showMouse = true;
	float _cameraSpeed = 2.5;
public:
	TestCamScene() 
		: Scene()
		, forward(false), backward(false), left(false), right(false)
		
	{

		GameObjectDefinition def;
		def.name = "Unlit Cube";
		def.T.pivot = glm::vec3(0.0f);
		//def.T.orientation = glm::vec3(0, 1, 0);
		def.T.orientation = glm::vec3(1.0f, 0.3f, 0.5f);
		def.ColorMain = glm::vec4(0, 1.0f, 0.8f, 1.0f);
		def.textureReference = ResourceManager::GetTexture("default");


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
			def.T.position = cubePositions[i];
			def.T.rotation = 20.0f * i;
			def.T.UpdateMatrix();

			objs.emplace_back(new GameObject(&def));
		}

		m_surface = new cubeSurface(); m_surface->Generate();


		ourShader = new Shader("./Shaders/MVP.vert", "./Shaders/SimpleColor2.frag");
		ourShader->Use().setInt("image", 0);

		m_cam.lastX = ScreenWidth / 2.0f; m_cam.lastY = ScreenHeight / 2.0f;
		m_cam.Swidth = ScreenWidth;
		m_cam.Sheight = ScreenHeight;

	}
	~TestCamScene()
	{
		for (auto m_object : objs) {
			delete m_object;
			m_object = nullptr;
		}
		delete ourShader;

	}

public: //frame updates
	virtual void Update(float deltaTime)
	{
		for(auto m_object:objs) m_object->Update(deltaTime);


		float cameraSpeed = _cameraSpeed * deltaTime;
		if (forward)
			m_cam.cameraPos += cameraSpeed * m_cam.cameraFront;
		if (backward)
			m_cam.cameraPos -= cameraSpeed * m_cam.cameraFront;
		if (left)
			m_cam.cameraPos -= glm::normalize(glm::cross(m_cam.cameraFront, m_cam.cameraUp)) * cameraSpeed;
		if (right)
			m_cam.cameraPos += glm::normalize(glm::cross(m_cam.cameraFront, m_cam.cameraUp)) * cameraSpeed;
	}
	virtual void FixedUpdate(float deltaTime)
	{

	}
public: //handle Inputs
	virtual void onKeyPress(int key)
	{
		if (key == GLFW_KEY_W) forward = true;
		if (key == GLFW_KEY_S) backward = true;
		if (key == GLFW_KEY_A) left = true;
		if (key == GLFW_KEY_D) right = true;
	}
	virtual void onKeyRelease(int key)
	{
		if (key == GLFW_KEY_W) forward = false;
		if (key == GLFW_KEY_S) backward = false;
		if (key == GLFW_KEY_A) left = false;
		if (key == GLFW_KEY_D) right = false;


		if (key == GLFW_KEY_TAB) m_cam.movementEnabled = !m_cam.movementEnabled;

		if (key == GLFW_KEY_Q)
		{
			showMouse = !showMouse;
			m_cam.movementEnabled = !showMouse;
			if(!showMouse) 
				glfwSetInputMode(App::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(App::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}

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
	}
	virtual void onMouseScrollCallback(double xOffset, double yOffset)
	{
		if (ImGui::GetIO().WantCaptureMouse) return;
		m_cam.Zoom(-(float)yOffset);
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
		glm::mat4 projection = m_cam.getProjection();
		glm::mat4 view = m_cam.getView();
		if(usetestShader)
		m_shader->Use().SetMatrix4("projection", projection).SetMatrix4("view", view);

		//Shader* ptr = usetestShader ? ourShader : m_shader; ptr->Use();

		for (auto m_object : objs) m_object->Draw(m_shader, m_surface);
		//for (auto m_object : objs) m_object->Draw(ptr, m_surface);


	};
	virtual void DrawSceneBackground() override
	{

	};
	virtual void DrawDebug() override
	{
		ImGui::Checkbox("use test Shader", &usetestShader);
		ImGui::SliderFloat("_cameraSpeed", &_cameraSpeed, 0, 30.0f);
		m_cam.Debug();
		for (auto m_object : objs) m_object->Debug();
	}

public:
	static Scene* Create() { return new TestCamScene; }
};


static int testIndex = RegisterScene("Showcase", "Scene B", TestCamScene::Create);



