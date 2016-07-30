#include "Geometry/GeometryFactory.hpp"
#include "Geometry/Rectangle.hpp"
#include "Geometry/BlockMesh.hpp"
#include "Geometry/PlaneMesh.hpp"

GeometryFactory::GeometryFactory(IResourceManager & rm) : rm(rm)
{
    auto rectangleGeometry = std::shared_ptr<Rectangle>(new Rectangle(rm));
    rectangle = std::shared_ptr<MeshNode>(new MeshNode(rectangleGeometry));

    auto blockGeometry = std::shared_ptr<BlockMesh>(new BlockMesh(rm));
    blockMesh = std::shared_ptr<MeshNode>(new MeshNode(blockGeometry));
}

std::shared_ptr<MeshNode> GeometryFactory::createCustomGeometry(const std::string fileName)
{
    return createCustomGeometry(fileName, false);
}

std::shared_ptr<MeshNode> GeometryFactory::createCustomGeometry(const std::string fileName, const bool flipFaces)
{
    std::pair<std::string, bool> key(fileName, flipFaces);
    auto it = customGeometries.find(key);
    
    if (it == customGeometries.end())
    {
        customGeometries[key] = loader.loadCustomGeometry(rm, fileName, flipFaces);
    }

    return std::shared_ptr<MeshNode>(new MeshNode(*customGeometries[key]));
}

std::shared_ptr<MeshNode> GeometryFactory::createRectangle()
{
    return std::shared_ptr<MeshNode>(new MeshNode(*rectangle));
}

std::shared_ptr<MeshNode> GeometryFactory::createBlockMesh()
{
    return std::shared_ptr<MeshNode>(new MeshNode(*blockMesh));
}

std::shared_ptr<MeshNode> GeometryFactory::createPlaneMesh(const unsigned int width, const unsigned int height)
{
    std::pair<unsigned int, unsigned int> key(width, height);
    auto it = planeMeshes.find(key);

    if (it == planeMeshes.end())
    {
        auto geometry = std::shared_ptr<PlaneMesh>(new PlaneMesh(rm, width, height));
        auto meshNode = std::shared_ptr<MeshNode>(new MeshNode(geometry));
        planeMeshes[key] = meshNode;
    }

    return std::shared_ptr<MeshNode>(new MeshNode(*planeMeshes[key]));
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
