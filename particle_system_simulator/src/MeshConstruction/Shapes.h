#pragma once

#include "RenderPipeline/Mesh/Mesh.h"
#include <stdint.h>

Mesh createQuad();
Mesh createCube();
Mesh createFlatSphere(uint32_t sectorCount, uint32_t stackCount);
Mesh createSmoothSphere(uint32_t sectorCount, uint32_t stackCount);
Mesh createSphere(int approximateVertexCount, bool smooth = true);
