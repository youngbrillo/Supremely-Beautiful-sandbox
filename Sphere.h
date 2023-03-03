#pragma once
#include "SBMesh.h"
namespace SB
{
    /*
      3D unit sphere charactized by its number of horizontal (xSegments) and vertical (ySegments)
      rings.
    */
    class Sphere :  public SBMesh
    {
    public:
        Sphere(unsigned int xSegments, unsigned int ySegments);
    };
}
