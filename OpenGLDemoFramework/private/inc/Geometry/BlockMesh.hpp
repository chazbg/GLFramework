#pragma once

#include "Core/Mesh.hpp"
#include "Core/IResourceManager.hpp"

class BlockMesh : public Mesh
{
public:
    BlockMesh(IResourceManager&, const float width = 1, const float height = 1, const float length = 1);
    ~BlockMesh();
private:
    float* genVerts(const float width, const float height, const float length);
    float* genUVs(const float width, const float height, const float length);
};