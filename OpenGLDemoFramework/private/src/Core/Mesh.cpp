#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include "Core/Shader.hpp"
#include <algorithm>
#include <cstdio>
#include "Math/GeometryAlgorithm.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

Mesh::Mesh() : 
material(0),
ibo(0),
castsShadow(false),
receivesShadow(false)
{
    vbos.resize(5);
}
Mesh::Mesh(const Mesh& rhs) :
    material(rhs.material),
    ibo(rhs.ibo),
    vbos(rhs.vbos),
    model(rhs.model),
    castsShadow(rhs.castsShadow),
    receivesShadow(rhs.receivesShadow)
{
    for (unsigned int i = 0; i < rhs.children.size(); i++)
    {
        children.push_back(rhs.children[i]->clone());
    }
}

Mesh::~Mesh()
{
}

const IIndexBuffer * Mesh::getIBO()
{
    return ibo;
}

std::vector<const IVertexBuffer*>& Mesh::getVBOs()
{
    return vbos;
}

void Mesh::setModelMatrix(const Matrix4 & model)
{
    this->model = model;

    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->setModelMatrix(model);
    }
}

Matrix4 Mesh::getModelMatrix() const
{
    return model;
}

void Mesh::setIndices(const IIndexBuffer & indices)
{
    ibo = &const_cast<IIndexBuffer&>(indices);
}

void Mesh::setVertices(const IVertexBuffer & vertices)
{
    vbos[0] = &vertices;
}

void Mesh::setNormals(const IVertexBuffer & normals)
{
    vbos[1] = &normals;
}

void Mesh::setTexCoords(const IVertexBuffer & texCoords)
{
    vbos[2] = &texCoords;
}

void Mesh::setTangents(const IVertexBuffer& tangents)
{
    vbos[3] = &tangents;
}

void Mesh::setBitangents(const IVertexBuffer& bitangents)
{
    vbos[4] = &bitangents;
}

void Mesh::setMaterial(IMaterial * material)
{
    this->material = material;

    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->setMaterial(material);
    }
}

IMaterial & Mesh::getMaterial() const
{
    return *material;
}

void Mesh::addChild(shared_ptr<IMesh> child)
{
    children.push_back(child);
}

std::vector<shared_ptr<IMesh>>& Mesh::getChildren()
{
    return children;
}

void Mesh::SetCastsShadow(const bool castsShadow)
{
    this->castsShadow = castsShadow;
}

void Mesh::SetReceivesShadow(const bool receivesShadow)
{
    this->receivesShadow = receivesShadow;
}

Vec3 Mesh::getPosition()
{
    Vec4 base(0.0f, 0.0f, 0.0f, 1.0f);
    base = model * base;
    return Vec3(base.x, base.y, base.z);
}

void Mesh::Scale(const float scaleX, const float scaleY, const float scaleZ)
{
    Matrix4 m = GeometryAlgorithm::CreateSRTMatrix(Vec3(scaleX, scaleY, scaleZ), Vec3(0, 0, 0), Vec3(0, 0, 0));
    model = m * model;

    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->Scale(scaleX, scaleY, scaleZ);
    }
}

void Mesh::Rotate(const float thetaX, const float thetaY, const float thetaZ)
{
    Matrix4 m;
    m.setRotation(thetaX, thetaY, thetaZ);
    model = m * model;
    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->Rotate(thetaX, thetaY, thetaZ);
    }
}

void Mesh::Translate(const float transX, const float transY, const float transZ)
{
    Matrix4 m;
    m.setTranslation(Vec3(transX, transY, transZ));
    model = m * model;

    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->Translate(transX, transY, transZ);
    }
}

shared_ptr<IMesh> Mesh::clone()
{
    return shared_ptr<IMesh>(new Mesh(*this));
}

float* Mesh::generateNormals(const float* vertexBuffer, const unsigned int vertexCount)
{
    float* normalsBuffer = new float[vertexCount * 3];

    for (unsigned int i = 0; i < vertexCount * 3; i += 9)
    {
        Vec3 a(vertexBuffer[i], vertexBuffer[i + 1], vertexBuffer[i + 2]);
        Vec3 b(vertexBuffer[i + 3], vertexBuffer[i + 4], vertexBuffer[i + 5]);
        Vec3 c(vertexBuffer[i + 6], vertexBuffer[i + 7], vertexBuffer[i + 8]);
        Vec3 cb = c - b;
        Vec3 ab = a - b;
        Vec3 res = (cb * ab).normalize();

        normalsBuffer[i] = res.x;
        normalsBuffer[i + 1] = res.y;
        normalsBuffer[i + 2] = res.z;

        normalsBuffer[i + 3] = res.x;
        normalsBuffer[i + 4] = res.y;
        normalsBuffer[i + 5] = res.z;

        normalsBuffer[i + 6] = res.x;
        normalsBuffer[i + 7] = res.y;
        normalsBuffer[i + 8] = res.z;
    }

    return normalsBuffer;
}

float* Mesh::generateUVs(const float* vertexBuffer, const unsigned int vertexCount)
{
    float* uvBuffer = new float[vertexCount * 2];

    unsigned int j = 0;
    for (unsigned int i = 0; i < vertexCount * 3; i += 3, j += 2)
    {
        Vec3 v(vertexBuffer[i], vertexBuffer[i + 1], vertexBuffer[i + 2]);
        v = -v.normalize();
        uvBuffer[j] = atan2f(v.z, v.x) / (2.0f * 3.14f) + 0.5f;
        uvBuffer[j + 1] = 0.5f - asinf(v.y) / 3.14f;
    }

    return uvBuffer;
}