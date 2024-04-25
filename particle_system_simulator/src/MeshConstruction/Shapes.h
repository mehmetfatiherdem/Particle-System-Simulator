#pragma once

#include <stdint.h>
#include "RenderPipeline/Mesh/Mesh.h"

Mesh createQuad();
Mesh createCube();
Mesh createFlatSphere(uint32_t sectorCount, uint32_t stackCount);
Mesh createSmoothSphere(uint32_t sectorCount, uint32_t stackCount);
Mesh createSphere(uint32_t approximateVertexCount, bool smooth = true);
