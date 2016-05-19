#pragma once
#include "Core/Mesh.hpp"
#include "Core/IResourceManager.hpp"

class PlaneMesh : public Mesh
{
public:
    PlaneMesh(IResourceManager& rm, int width, int height);
private:
    float* generatePlaneVertices(int width, int height);
    float* generateUVs(int width, int height);
};

