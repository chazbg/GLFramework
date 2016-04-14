#pragma once

#include "Core/Mesh.hpp"
#include <Core/VertexBuffer.hpp>

class BlockMesh : public Mesh
{
public:
    BlockMesh(const float width = 1, const float height = 1, const float length = 1);
    ~BlockMesh();
    void setTime(const unsigned int time);
    unsigned int getTime();
private:
    float* genVerts();
    float* genUVs();
    float width;
    float height;
    float length;
    unsigned int time;
    VertexBuffer* vertices;
    VertexBuffer* normals;
    VertexBuffer* uvs;
};