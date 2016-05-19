#include "Geometry/GeometryFactory.hpp"

GeometryFactory::GeometryFactory(IResourceManager & rm) : rm(rm), rectangle(rm), blockMesh(rm)
{
}

CustomGeometry GeometryFactory::createCustomGeometry(const std::string fileName)
{
    return createCustomGeometry(fileName, false);
}

CustomGeometry GeometryFactory::createCustomGeometry(const std::string fileName, const bool flipFaces)
{
    std::pair<std::string, bool> key(fileName, flipFaces);
    auto it = customGeometries.find(key);
    
    if (it == customGeometries.end())
    {
        customGeometries[key] = new CustomGeometry(rm, fileName, flipFaces);
    }

    return *customGeometries[key];
}

Rectangle GeometryFactory::createRectangle()
{
    return rectangle;
}

BlockMesh GeometryFactory::createBlockMesh()
{
    return blockMesh;
}

PlaneMesh GeometryFactory::createPlaneMesh(const unsigned int width, const unsigned int height)
{
    std::pair<unsigned int, unsigned int> key(width, height);
    auto it = planeMeshes.find(key);

    if (it == planeMeshes.end())
    {
        planeMeshes[key] = new PlaneMesh(rm, width, height);
    }

    return *planeMeshes[key];
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
