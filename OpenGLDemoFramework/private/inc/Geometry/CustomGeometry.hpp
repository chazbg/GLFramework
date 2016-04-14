#pragma once

#include <Core/Mesh.hpp>
#include <Core/VertexBuffer.hpp>
#include <Core/IndexBuffer.hpp>
#include <string>
#include <vector>

class CustomGeometry : public Mesh
{
public:
    CustomGeometry(const std::string fileName);
    CustomGeometry(const std::string fileName, const bool flipFaces);
    ~CustomGeometry();
};