#pragma once

#include "Core/Mesh.hpp"
#include "Core/IResourceManager.hpp"

class BlockMesh : public Mesh
{
public:
    BlockMesh(IResourceManager&, const float width = 1, const float height = 1, const float length = 1);
    ~BlockMesh();
    void setTime(const unsigned int time);
    unsigned int getTime();
private:
    float* genVerts();
    float* genUVs();
    IResourceManager& rm;
    float width;
    float height;
    float length;
    unsigned int time;
    IVertexBuffer* vertices;
    IVertexBuffer* normals;
    IVertexBuffer* uvs;
};