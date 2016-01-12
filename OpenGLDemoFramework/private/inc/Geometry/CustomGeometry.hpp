#pragma once

#include <Core/Mesh.hpp>
#include <Core/VertexBufferObject.hpp>
#include <Core/IndexBufferObject.hpp>
#include <string>
#include <vector>

class CustomGeometry : public Mesh
{
public:
    CustomGeometry(const std::string fileName);
    ~CustomGeometry();
};