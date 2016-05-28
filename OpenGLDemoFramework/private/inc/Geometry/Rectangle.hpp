#pragma once
#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include "Core/IResourceManager.hpp"
#include <string>

class Rectangle : public Mesh
{
public:
    Rectangle(const Rectangle& rhs);
    Rectangle(IResourceManager& rm);
    Rectangle(IResourceManager& rm, Vec2 topLeft, Vec2 bottomRight);
private:
    Vec2 topLeft;
    Vec2 bottomRight;
    float* genVertices();
    float* genTexCoords();
    void init(IResourceManager& rm);
};