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

private:
    std::vector<IndexBufferObject*> indexBuffers;
    std::vector<VertexBufferObject*> vertexBuffers;
    std::vector<VertexBufferObject*> normalBuffers;
    std::vector<VertexBufferObject*> uvBuffers;
};