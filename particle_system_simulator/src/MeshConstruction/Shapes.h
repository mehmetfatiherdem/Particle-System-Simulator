#pragma once

#include "RenderPipeline/Mesh/Mesh.h"

struct MeshProperties;

Mesh* createQuad(const MeshProperties& props);
Mesh* createCube(const MeshProperties& props);
Mesh* createFlatSphere(const MeshProperties& props, unsigned int sectorCount, unsigned int stackCount);
Mesh* createSmoothSphere(const MeshProperties& props, unsigned int sectorCount, unsigned int stackCount);
Mesh* createSphere(const MeshProperties& props, int approximateVertexCount, bool smooth = true);
