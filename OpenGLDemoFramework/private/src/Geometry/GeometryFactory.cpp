#include "Geometry/GeometryFactory.hpp"

GeometryFactory::GeometryFactory(IResourceManager & rm) : rm(rm), rectangle(rm), blockMesh(rm)
{
}

std::shared_ptr<CustomGeometry> GeometryFactory::createCustomGeometry(const std::string fileName)
{
    return createCustomGeometry(fileName, false);
}

std::shared_ptr<CustomGeometry> GeometryFactory::createCustomGeometry(const std::string fileName, const bool flipFaces)
{
    std::pair<std::string, bool> key(fileName, flipFaces);
    auto it = customGeometries.find(key);
    
    if (it == customGeometries.end())
    {
        customGeometries[key] = new CustomGeometry(rm, fileName, flipFaces);
    }

    return std::shared_ptr<CustomGeometry>(new CustomGeometry(*customGeometries[key]));
}

std::shared_ptr<Rectangle> GeometryFactory::createRectangle()
{
    return std::shared_ptr<Rectangle>(new Rectangle(rectangle));
}

std::shared_ptr<BlockMesh> GeometryFactory::createBlockMesh()
{
    return std::shared_ptr<BlockMesh>(new BlockMesh(blockMesh));
}

std::shared_ptr<PlaneMesh> GeometryFactory::createPlaneMesh(const unsigned int width, const unsigned int height)
{
    std::pair<unsigned int, unsigned int> key(width, height);
    auto it = planeMeshes.find(key);

    if (it == planeMeshes.end())
    {
        planeMeshes[key] = new PlaneMesh(rm, width, height);
    }

    return std::shared_ptr<PlaneMesh>(new PlaneMesh(*planeMeshes[key]));
}

float * GeometryFactory::genRectangleVertices(const Vec2 & topLeft, const Vec2 & bottomRight)
{
    return nullptr;
}

float * GeometryFactory::genRectangleUVs(const Vec2 & topLeft, const Vec2 & bottomRight)
{
    return nullptr;
}

float * GeometryFactory::genBlockMeshVertices(const float width, const float height, const float length)
{
    return nullptr;
}

float * GeometryFactory::genBlockMeshUVs(const float width, const float height, const float length)
{
    return nullptr;
}

float * GeometryFactory::genPlaneMeshVertices(const unsigned int width, const unsigned int height)
{
    return nullptr;
}

float * GeometryFactory::genPlaneMeshUVs(const unsigned int width, const unsigned int height)
{
    return nullptr;
}
