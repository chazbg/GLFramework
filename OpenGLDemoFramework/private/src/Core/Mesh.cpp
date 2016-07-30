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
    castsShadow(rhs.castsShadow),
    receivesShadow(rhs.receivesShadow)
{
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
}

IMaterial & Mesh::getMaterial() const
{
    return *material;
}

void Mesh::SetCastsShadow(const bool castsShadow)
{
    this->castsShadow = castsShadow;
}

void Mesh::SetReceivesShadow(const bool receivesShadow)
{
    this->receivesShadow = receivesShadow;
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