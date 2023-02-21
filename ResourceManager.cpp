#include "ResourceManager.h"
#include <glad/glad.h>

std::map<std::string, Texture>  ResourceManager::Textures;
bool  ResourceManager::ranStartup = false;

void ResourceManager::startup()
{
	if (ranStartup)
	{
		printf("ResourceManager.cpp::\tStartup has already run, run cleanup first to restart!\n");
		return;
	}
	//todo: in the future have the system read through everything and yada yada yada but for now i'll do it by hand
	printf("ResourceManager.cpp::\tCreating assets\n");

	//load item textures
	//ResourceManager::LoadTexture("./assets/SparePart.png", true, "SparePart");
	//ResourceManager::LoadTexture("./assets/EnergyCell.png", true, "EnergyCell");
	//ResourceManager::LoadTexture("./assets/Ammo.png", true, "Ammo");
	ResourceManager::LoadTexture("./assets/brick.png", true, "icon", GL_LINEAR, GL_LINEAR);

	ResourceManager::LoadTexture("./assets/sb/a00.png", true, "astroid1");
	ResourceManager::LoadTexture("./assets/sb/a01.png", true, "astroid2");
	ResourceManager::LoadTexture("./assets/sb/a02.png", true, "astroid3");
	ResourceManager::LoadTexture("./assets/sb/a03.png", true, "astroid4");
	ResourceManager::LoadTexture("./assets/sb/a04.png", true, "astroid5");
	ResourceManager::LoadTexture("./assets/sb/s01.png", true, "ship");

	ResourceManager::LoadTexture("./assets/effects/health02.png", true, "healthred");
	ResourceManager::LoadTexture("./assets/effects/health01.png", true, "healthgreen");
	ResourceManager::LoadTexture("./assets/effects/accel.png", true, "accelIcon");
	ResourceManager::LoadTexture("./assets/effects/spread.png", true, "spreadIcon");
	ResourceManager::LoadTexture("./assets/blankpixel.png", true, "default");

	ranStartup = true;
}

void ResourceManager::cleanup()
{
	if (!ranStartup)
	{
		printf("ResourceManager.cpp::\tCleanup should not run, b/c startup has not run!\n");
		return;
	}
	printf("ResourceManager.cpp::\tClearing All assets\n");
	ResourceManager::Clear();
	ranStartup = false;
}

void ResourceManager::Clear()
{
	// (properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.id);

	Textures.clear();

}


Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name, unsigned int minFilter, unsigned int magFilter)
{
	Textures[name] = Texture(file, alpha, minFilter, magFilter); 
	printf("ResourceManager.cpp::\t\tInitializing Texture\t'%s' (%s)\n", name.c_str(), file);
	return Textures[name];
}

Texture* ResourceManager::GetTexture(std::string name)
{
	return &Textures[name];
}
