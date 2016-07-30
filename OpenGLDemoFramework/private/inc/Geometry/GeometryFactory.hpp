#pragma once

#include "Geometry/IGeometryFactory.hpp"
#include "Core/IResourceManager.hpp"
#include "Core/GeometryLoader.hpp"
#include <map>

class GeometryFactory : public IGeometryFactory
{
public:
    GeometryFactory(IResourceManager& rm);
    virtual std::shared_ptr<MeshNode> createCustomGeometry(const std::string fileName);
    virtual std::shared_ptr<MeshNode> createCustomGeometry(const std::string fileName, const bool flipFaces);
    virtual std::shared_ptr<MeshNode> createRectangle();
    virtual std::shared_ptr<MeshNode> createBlockMesh();
    virtual std::shared_ptr<MeshNode> createPlaneMesh(const unsigned int width, const unsigned int height);
private:
    typedef std::map<std::pair<std::string, bool>, std::shared_ptr<MeshNode>> CustomGeometries;
    typedef std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<MeshNode>> PlaneMeshes;
    IResourceManager& rm;
    GeometryLoader loader;
    CustomGeometries customGeometries;
    PlaneMeshes planeMeshes;
    std::shared_ptr<MeshNode> rectangle;
    std::shared_ptr<MeshNode> blockMesh;

private:
    float* genRectangleVertices(const Vec2& topLeft, const Vec2& bottomRight);
    float* genRectangleUVs(const Vec2& topLeft, const Vec2& bottomRight);
    float* genBlockMeshVertices(const float width, const float height, const float length);
    float* genBlockMeshUVs(const float width, const float height, const float length);
    float* genPlaneMeshVertices(const unsigned int width, const unsigned int height);
    float* genPlaneMeshUVs(const unsigned int width, const unsigned int height);
};