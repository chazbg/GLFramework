#pragma once

#include "Geometry/IGeometryFactory.hpp"
#include "Core/IResourceManager.hpp"
#include <map>

class GeometryFactory : public IGeometryFactory
{
public:
    GeometryFactory(IResourceManager& rm);
    virtual std::shared_ptr<CustomGeometry> createCustomGeometry(const std::string fileName);
    virtual std::shared_ptr<CustomGeometry> createCustomGeometry(const std::string fileName, const bool flipFaces);
    virtual std::shared_ptr<Rectangle>      createRectangle();
    virtual std::shared_ptr<BlockMesh>      createBlockMesh();
    virtual std::shared_ptr<PlaneMesh>      createPlaneMesh(const unsigned int width, const unsigned int height);
private:
    typedef std::map<std::pair<std::string, bool>, CustomGeometry*> CustomGeometries;
    typedef std::map<std::pair<unsigned int, unsigned int>, PlaneMesh*> PlaneMeshes;
    IResourceManager& rm;
    CustomGeometries customGeometries;
    PlaneMeshes planeMeshes;
    Rectangle rectangle;
    BlockMesh blockMesh;

private:
    float* genRectangleVertices(const Vec2& topLeft, const Vec2& bottomRight);
    float* genRectangleUVs(const Vec2& topLeft, const Vec2& bottomRight);
    float* genBlockMeshVertices(const float width, const float height, const float length);
    float* genBlockMeshUVs(const float width, const float height, const float length);
    float* genPlaneMeshVertices(const unsigned int width, const unsigned int height);
    float* genPlaneMeshUVs(const unsigned int width, const unsigned int height);
};