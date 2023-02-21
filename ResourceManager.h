#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "Texture.h"
#include "shader.h"
#include "Surface.h"


class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Texture>   Textures;
    //Creates the most utilized Textures at the beginining of the application and stores them in memory
    static void startup();
    //Destroy all textures created in the startup() function
    static void cleanup();
    // loads (and generates) a texture from file
    static Texture LoadTexture(const char* file, bool alpha, std::string name, unsigned int minFilter = 0x2600, unsigned int magFilter = 0x2600);
    // retrieves a stored texture
    static Texture* GetTexture(std::string name);

    // properly de-allocates all loaded resources
    static void Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    // loads a single texture from file
        // the repective classes do that now
    static bool ranStartup;
};


#endif