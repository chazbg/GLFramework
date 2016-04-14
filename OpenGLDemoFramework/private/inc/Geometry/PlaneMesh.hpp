#pragma once
#include <Core/Mesh.hpp>
#include <Core/VertexBuffer.hpp>

class PlaneMesh : public Mesh
{
public:
    PlaneMesh(int _width, int _height);
    ~PlaneMesh();

private:
    float* generatePlaneVertices(int width, int height);
    float* generateUVs(int width, int height);
    int width;
    int height;
    VertexBuffer* vertices;
    VertexBuffer* uvs;
};

