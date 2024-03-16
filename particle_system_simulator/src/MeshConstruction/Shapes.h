#pragma once

#include "RenderPipeline/Mesh/Mesh.h"

Mesh createQuad();
Mesh createCube();
Mesh createFlatSphere(unsigned int sectorCount, unsigned int stackCount);
Mesh createSmoothSphere(unsigned int sectorCount, unsigned int stackCount);
Mesh createSphere(int approximateVertexCount, bool smooth = true);
