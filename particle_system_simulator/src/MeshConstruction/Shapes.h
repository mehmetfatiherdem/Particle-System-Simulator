#pragma once

#include "RenderPipeline/Mesh/Mesh.h"

struct VertexAttributes;

Mesh* createQuad(const VertexAttributes& props);
Mesh* createCube(const VertexAttributes& props);
Mesh* createFlatSphere(const VertexAttributes& props, unsigned int sectorCount, unsigned int stackCount);
Mesh* createSmoothSphere(const VertexAttributes& props, unsigned int sectorCount, unsigned int stackCount);
Mesh* createSphere(const VertexAttributes& props, int approximateVertexCount, bool smooth = true);
