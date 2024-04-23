#pragma once

#include <stdint.h>
#include "RenderPipeline/Mesh/Mesh.h"

struct MeshProperties;

Mesh* createQuad(const MeshProperties& props);
Mesh* createCube(const MeshProperties& props);
Mesh* createFlatSphere(const MeshProperties& props, uint32_t sectorCount, uint32_t stackCount);
Mesh* createSmoothSphere(const MeshProperties& props, uint32_t sectorCount, uint32_t stackCount);
Mesh* createSphere(const MeshProperties& props, int approximateVertexCount, bool smooth = true);
