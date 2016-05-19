#pragma once

#include "Core/Mesh.hpp"
#include "Core/IResourceManager.hpp"
#include <string>

class CustomGeometry : public Mesh
{
public:
    CustomGeometry(IResourceManager& rm, const std::string fileName);
    CustomGeometry(IResourceManager& rm, const std::string fileName, const bool flipFaces);
};