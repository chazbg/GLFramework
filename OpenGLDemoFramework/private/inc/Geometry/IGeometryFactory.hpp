#pragma once

#include "Geometry/CustomGeometry.hpp"
#include "Geometry/Rectangle.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Geometry/PlaneMesh.hpp"
#include "Core/IResourceManager.hpp"

class IGeometryFactory
{
public:
    virtual CustomGeometry createCustomGeometry(const std::string fileName) = 0;
    virtual CustomGeometry createCustomGeometry(const std::string fileName, const bool flipFaces) = 0;
    virtual Rectangle      createRectangle() = 0;
    virtual BlockMesh      createBlockMesh() = 0;
    virtual PlaneMesh      createPlaneMesh(const unsigned int width, const unsigned int height) = 0;
};