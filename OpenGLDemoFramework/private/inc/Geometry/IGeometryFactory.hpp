#pragma once

#include "Geometry/CustomGeometry.hpp"
#include "Geometry/Rectangle.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Geometry/PlaneMesh.hpp"
#include "Core/IResourceManager.hpp"
#include <memory>

class IGeometryFactory
{
public:
    virtual std::shared_ptr<CustomGeometry> createCustomGeometry(const std::string fileName) = 0;
    virtual std::shared_ptr<CustomGeometry> createCustomGeometry(const std::string fileName, const bool flipFaces) = 0;
    virtual std::shared_ptr<Rectangle>      createRectangle() = 0;
    virtual std::shared_ptr<BlockMesh>      createBlockMesh() = 0;
    virtual std::shared_ptr<PlaneMesh>      createPlaneMesh(const unsigned int width, const unsigned int height) = 0;
};