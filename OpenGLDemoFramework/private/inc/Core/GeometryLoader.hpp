#pragma once

#include "Core/MeshNode.hpp"
#include "Core/IResourceManager.hpp"
#include <string>

class GeometryLoader
{
public:
    std::shared_ptr<MeshNode> loadCustomGeometry(IResourceManager& rm, const std::string fileName);
    std::shared_ptr<MeshNode> loadCustomGeometry(IResourceManager& rm, const std::string fileName, const bool flipFaces);
private:
};