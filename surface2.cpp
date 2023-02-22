#include "surface2.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
void TriangleSurface::Generate(float texCoordMin, float texCoordMax)
{
    float ve2rtices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   texCoordMax, texCoordMax, // top right
         0.5f, -0.5f, 0.0f,   texCoordMax, texCoordMin, // bottom right
        -0.5f, -0.5f, 0.0f,   texCoordMin, texCoordMin // bottom left
        //-0.5f,  0.5f, 0.0f,   texCoordMin, texCoordMax  // top left 
    };
    float vertices[] = {
        -0.5f, 0.0f, 0.0f,  texCoordMin, 0.0f
        ,0.0f, 0.5f, 0.0f,  texCoordMax * 0.5, texCoordMax
        ,0.5f, 0.0f, 0.0f,  texCoordMax, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2  // first Triangle
        //0, 1, 3,  // first Triangle
        //1, 2, 3   // second Triangle
    };

    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TriangleSurface::Bind()
{

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); //unbind vertex array
}

//circle surface /mesh *************************************************************

void CircleSurface::Generate(float texCoordMin, float texCoordMax)
{
    std::vector<texturedVertex> verts;
    std::vector<unsigned int> indis;


    float  radius = 0.50f;

    const float k_segments = 16.0f * 2;
    const float k_increment = 2.0f * M_PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    glm::vec2 center = glm::vec2(0.0f);
    glm::vec2 r1(1.0f, 0.0f);
    glm::vec2 v1 = center + radius * r1;

    subdivisions = 0;
    
    verts.push_back({ glm::vec3(0.0f),   glm::vec2(0.0f) });
    indis.push_back(0);
    for (int i = 0; i <= k_segments; ++i)
    {
            glm::vec2 r2;
            r2.x = cosInc * r1.x - sinInc * r1.y;
            r2.y = sinInc * r1.x + cosInc * r1.y;
            glm::vec2 v2 = center + radius * r2;

            texturedVertex nextVert = { glm::vec3(r2.x,  r2.y, 0.0f),   glm::vec2(r2.x, r2.y) };
            verts.push_back(nextVert);

            r1 = r2;
            v1 = v2;
       // verts.push_back({ glm::vec3(0.0f),   glm::vec2(0.0f) });
        printf("%02d Circle surface generate:\tVertex: (%.3f, %.3f, %.3f), Texture(%0.3f, %0.3f)\n", i, nextVert.Position.x, nextVert.Position.y, nextVert.Position.z, nextVert.TextureCoords.x, nextVert.TextureCoords.y);
    }
    subdivisions = indis.size();
    subdivisions = verts.size();
    maxSd = subdivisions;
    printf("indicies: %d", subdivisions);

    unsigned long long _size = verts.size() * sizeof(verts);
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(texturedVertex), &verts[0], GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indis.size() * sizeof(unsigned int), &indis[0], GL_STATIC_DRAW);
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(texturedVertex), (void*)0);
    glEnableVertexAttribArray(0);
    //texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(texturedVertex), (void*)offsetof(texturedVertex, TextureCoords));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void CircleSurface::Bind()
{
    glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, subdivisions, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, subdivisions);

    glBindVertexArray(0); //unbind vertex array
}
