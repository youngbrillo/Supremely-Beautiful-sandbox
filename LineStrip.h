#pragma once
#include "SBMesh.h"
namespace SB
{
    /*
      A line strip of specified width / length.
      Segments equals the number of 2-triangle sets while width equals the width of the line.
      Useful for line systems that require a filled line strip.
    */
    class LineStrip :  public SBMesh
    {
    public:
        LineStrip(float width, unsigned int segments);

    };
}

