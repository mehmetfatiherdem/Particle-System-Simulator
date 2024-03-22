#include <cmath>
#include "Shapes.h"

Mesh createQuad(const MeshProperties& props)
{
	std::vector<Vertex> vertices{
		Vertex{glm::vec3{+0.5f, +0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, +0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{+0.5f, -0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{1.0f, 0.0f}},
	};

	std::vector<unsigned int> indices{0, 1, 2, 2, 3, 0};

	Mesh mesh{std::move(vertices), std::move(indices), props};
	return mesh;
}

Mesh createCube(const MeshProperties& props)
{
	std::vector<Vertex> vertices{
		//FRONT FACE
		Vertex{glm::vec3{+0.5f, +0.5f, +0.5f}, glm::vec3{0.0f, 0.0f, +1.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, +0.5f, +0.5f}, glm::vec3{0.0f, 0.0f, +1.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, +0.5f}, glm::vec3{0.0f, 0.0f, +1.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{+0.5f, -0.5f, +0.5f}, glm::vec3{0.0f, 0.0f, +1.0f}, glm::vec2{1.0f, 0.0f}},

		//RIGHT FACE
		Vertex{glm::vec3{+0.5f, +0.5f, -0.5f}, glm::vec3{+1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{+0.5f, +0.5f, +0.5f}, glm::vec3{+1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{+0.5f, -0.5f, +0.5f}, glm::vec3{+1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{+0.5f, -0.5f, -0.5f}, glm::vec3{+1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},

		//BACK FACE
		Vertex{glm::vec3{-0.5f, +0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{+0.5f, +0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{+0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{1.0f, 0.0f}},

		//LEFT FACE
		Vertex{glm::vec3{-0.5f, +0.5f, +0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, +0.5f, -0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, +0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},

		//TOP FACE
		Vertex{glm::vec3{+0.5f, +0.5f, -0.5f}, glm::vec3{0.0f, +1.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, +0.5f, -0.5f}, glm::vec3{0.0f, +1.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, +0.5f, +0.5f}, glm::vec3{0.0f, +1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{+0.5f, +0.5f, +0.5f}, glm::vec3{0.0f, +1.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},

		//BOTTOM FACE
		Vertex{glm::vec3{+0.5f, -0.5f, +0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, +0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
		Vertex{glm::vec3{+0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
	};

	std::vector<unsigned int> indices{
		 0,  1,  2,  2,  3,  0,	//FRONT FACE
		 4,  5,  6,  6,  7,  4,	//RIGHT FACE
		 8,  9, 10, 10, 11,  8,	//BACK FACE
		12, 13, 14, 14, 15, 12,	//LEFT FACE
		16, 17, 18, 18, 19, 16,	//TOP FACE
		20, 21, 22, 22, 23, 20,	//BOTTOM FACE
	};

	Mesh mesh{std::move(vertices), std::move(indices), props};
	return mesh;
}

glm::vec3 computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

Mesh createFlatSphere(const MeshProperties& props, unsigned int sectorCount, unsigned int stackCount)
{
    const float PI = 3.141592653589793f;

    std::vector<Vertex> vertices;
    vertices.reserve(2 * sectorCount * ((2 * stackCount) - 1));

    std::vector<unsigned int> indices;
    indices.reserve(6 * sectorCount * (stackCount - 1));

    std::vector<float> tempVertices;
    tempVertices.reserve(5 * (sectorCount + 1) * (stackCount + 1));

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(size_t i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;
        float xy = 0.5f * cosf(stackAngle);
        float z = 0.5f * sinf(stackAngle);

        for(size_t j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;
            tempVertices.insert(tempVertices.end(), {xy * cosf(sectorAngle), xy * sinf(sectorAngle), z, (float)j, float(i)});
        }
    }

    unsigned int index = 0;

    for(size_t i = 0; i < stackCount; ++i)
    {
        unsigned int vi1 = i * (sectorCount + 1);
        unsigned int vi2 = (i + 1) * (sectorCount + 1);

        for(size_t j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            float v1[5], v2[5], v3[5], v4[5];

            for(size_t k = 0; k < 5; ++k)
            {
                v1[k] = tempVertices[5 * vi1 + k];
                v2[k] = tempVertices[5 * vi2 + k];
                v3[k] = tempVertices[5 * (vi1 + 1) + k];
                v4[k] = tempVertices[5 * (vi2 + 1) + k];
            }

            if(i == 0)
            {
                glm::vec3 normal = computeFaceNormal(v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v4[0], v4[1], v4[2]);

                vertices.push_back({glm::vec3{v1[0], v1[1], v1[2]}, normal, glm::vec2{v1[3], v1[4]}});
                vertices.push_back({glm::vec3{v2[0], v2[1], v2[2]}, normal, glm::vec2{v2[3], v2[4]}});
                vertices.push_back({glm::vec3{v4[0], v4[1], v4[2]}, normal, glm::vec2{v4[3], v4[4]}});

                indices.insert(indices.end(), {index, index + 1, index + 2});
                index += 3;
            }
            else if(i == (stackCount - 1))
            {
                glm::vec3 normal = computeFaceNormal(v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]);

                vertices.push_back({glm::vec3{v1[0], v1[1], v1[2]}, normal, glm::vec2{v1[3], v1[4]}});
                vertices.push_back({glm::vec3{v2[0], v2[1], v2[2]}, normal, glm::vec2{v2[3], v2[4]}});
                vertices.push_back({glm::vec3{v3[0], v3[1], v3[2]}, normal, glm::vec2{v3[3], v3[4]}});

                indices.insert(indices.end(), {index, index + 1, index + 2});
                index += 3;
            }
            else
            {
                glm::vec3 normal = computeFaceNormal(v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]);

                vertices.push_back({glm::vec3{v1[0], v1[1], v1[2]}, normal, glm::vec2{v1[3], v1[4]}});
                vertices.push_back({glm::vec3{v2[0], v2[1], v2[2]}, normal, glm::vec2{v2[3], v2[4]}});
                vertices.push_back({glm::vec3{v3[0], v3[1], v3[2]}, normal, glm::vec2{v3[3], v3[4]}});
                vertices.push_back({glm::vec3{v4[0], v4[1], v4[2]}, normal, glm::vec2{v4[3], v4[4]}});

                indices.insert(indices.end(), {index, index + 1, index + 2, index + 2, index + 1, index + 3});
                index += 4;
            }
        }
    }

    Mesh mesh{std::move(vertices), std::move(indices), props};
    return mesh;
}

Mesh createSmoothSphere(const MeshProperties& props, unsigned int sectorCount, unsigned int stackCount)
{
    const float PI = 3.141592653589793f;

    std::vector<Vertex> vertices;
    vertices.reserve((sectorCount + 1) * (stackCount + 1));

    std::vector<unsigned int> indices;
    indices.reserve(6 * sectorCount * (stackCount - 1));

    float lengthInv = 2.0f;
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;

    for(size_t i = 0; i <= stackCount; ++i)
    {
        float stackAngle = PI / 2 - i * stackStep;
        float xy = 0.5f * cosf(stackAngle);
        float z = 0.5f * sinf(stackAngle);

        for(size_t j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float nx = x * lengthInv;
            float ny = y * lengthInv;
            float nz = z * lengthInv;
            float s = (float)j / sectorCount;
            float t = (float)i / stackCount;

            vertices.push_back({glm::vec3{x, y, z}, glm::vec3{nx, ny, nz}, glm::vec2{s, t}});
        }
    }

    for(int i = 0; i < stackCount; ++i)
    {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.insert(indices.end(), {k1, k2, k1 + 1});
            }

            if(i != (stackCount - 1))
            {
                indices.insert(indices.end(), {k1 + 1, k2, k2 + 1});
            }
        }
    }

    Mesh mesh{std::move(vertices), std::move(indices), props};
    return mesh;
}

Mesh createSphere(const MeshProperties& props, int approximateVertexCount, bool smooth)
{
    float a, b, c;

    if(smooth)
    {
        a = 1.0f;
        b = 2.0f;
        c = 1 - approximateVertexCount;
    }
    else
    {
        a = 4.0f;
        b = -2.0f;
        c = -approximateVertexCount;
    }

    float positiveSolution = (-b + sqrtf((b * b) - (4 * a * c))) / (2 * a);
    unsigned int rounded = static_cast<unsigned int>(roundf(positiveSolution));

    return (smooth ? createSmoothSphere(props, rounded, rounded) : createFlatSphere(props, rounded, rounded));
}

glm::vec3 computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    const float EPSILON = 0.000001f;

    glm::vec3 normal{0.0f, 0.0f, 0.0f};
    float nx, ny, nz;

    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    float length = sqrtf(nx * nx + ny * ny + nz * nz);

    if(length > EPSILON)
    {
        float lengthInv = 1.0f / length;
        normal = {nx * lengthInv, ny * lengthInv, nz * lengthInv};
    }

    return normal;
}