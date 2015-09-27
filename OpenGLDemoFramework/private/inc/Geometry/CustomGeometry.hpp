#pragma once

#include <Core/Mesh.hpp>
#include <Core/VertexBufferObject.hpp>
#include <Core/IndexBufferObject.hpp>
#include <string>

class CustomGeometry : public Mesh
{
public:
    CustomGeometry(const std::string fileName);
    ~CustomGeometry();

private:
    IndexBufferObject* indices;
    VertexBufferObject* vertices;
    VertexBufferObject* normals;
    VertexBufferObject* uvs;
};