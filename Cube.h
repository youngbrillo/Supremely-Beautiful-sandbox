#pragma once
#include "SBMesh.h"
namespace SB
{
    /*
      A 3D cube in the range [-0.5, 0.5]. Vertices are hard-coded.
    */
    class Cube :  public SBMesh
    {
        public:
            Cube();
    };
}
