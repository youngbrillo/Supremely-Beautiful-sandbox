#include "Sphere.h"
#include <glm/ext.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
SB::Sphere::Sphere(unsigned int xSegments, unsigned int ySegments)
{
    for (unsigned int y = 0; y <= ySegments; ++y)
    {
        for (unsigned int x = 0; x <= xSegments; ++x)
        {
            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;


            float xPos = std::cos(xSegment * M_PI * 2.0f) * std::sin(ySegment * M_PI); // TAU is 2PI
            float yPos = std::cos(ySegment * M_PI);
            float zPos = std::sin(xSegment * M_PI * 2.0f) * std::sin(ySegment * M_PI);

            Positions.push_back(glm::vec3(xPos, yPos, zPos));
            UV.push_back(glm::vec2(xSegment, ySegment));
            Normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (int y = 0; y < ySegments; ++y)
    {
        for (int x = 0; x < xSegments; ++x)
        {
            Indices.push_back((y + 1) * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x + 1);

            Indices.push_back((y + 1) * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x + 1);
            Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
        }
    }

    Topology = TRIANGLES;
    Finalize();
}
