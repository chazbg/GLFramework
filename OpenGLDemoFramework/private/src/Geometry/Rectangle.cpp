#include "Geometry/Rectangle.hpp"

Rectangle::Rectangle(const Rectangle & rhs) :
    Mesh(rhs),
    topLeft(rhs.topLeft),
    bottomRight(rhs.bottomRight)
{
}

Rectangle::Rectangle(IResourceManager& rm) : topLeft(-1, 1), bottomRight(1, -1)
{
    init(rm);
}

Rectangle::Rectangle(IResourceManager& rm, Vec2 topLeft, Vec2 bottomRight) : topLeft(topLeft), bottomRight(bottomRight)
{
    init(rm);
}

void Rectangle::init(IResourceManager& rm)
{
    unsigned int vertexCount = 6;
    
    float* vertexBuffer  = genVertices();
    float* normalsBuffer = generateNormals(vertexBuffer, vertexCount);
    float* uvs           = genTexCoords();

    IVertexBuffer* vertices  = rm.createVertexBuffer(vertexCount, 3, 0, vertexBuffer);
    IVertexBuffer* normals   = rm.createVertexBuffer(vertexCount, 3, 1, normalsBuffer);
    IVertexBuffer* texCoords = rm.createVertexBuffer(vertexCount, 2, 2, uvs);
    
    setVertices(*vertices);
    setTexCoords(*texCoords);
    setNormals(*normals);

    delete[] vertexBuffer;
    delete[] normalsBuffer;
    delete[] uvs;
}

float* Rectangle::genVertices()
{
    float* verts = new float[18];

    //Top left
    verts[0] = topLeft.x;
    verts[1] = topLeft.y;
    verts[2] = 0.0f;

    //Bottom left
    verts[3] = topLeft.x;
    verts[4] = bottomRight.y;
    verts[5] = 0.0f;

    //Top right
    verts[6] = bottomRight.x;
    verts[7] = topLeft.y;
    verts[8] = 0.0f;

    //Bottom left
    verts[9]  = topLeft.x;
    verts[10] = bottomRight.y;
    verts[11] = 0.0f;
    
    //Bottom right
    verts[12] = bottomRight.x;
    verts[13] = bottomRight.y;
    verts[14] = 0.0f;

    //Top right
    verts[15] = bottomRight.x;
    verts[16] = topLeft.y;
    verts[17] = 0.0f;

    return verts;
}

float* Rectangle::genTexCoords()
{
    float* tc = new float[12];

    //Top left
    tc[0] = 0;
    tc[1] = 1;

    //Bottom left
    tc[2] = 0;
    tc[3] = 0;

    //Top right
    tc[4] = 1;
    tc[5] = 1;

    //Bottom left
    tc[6] = 0;
    tc[7] = 0;

    //Bottom right
    tc[8] = 1;
    tc[9] = 0;

    //Top right
    tc[10] = 1;
    tc[11] = 1;

    return tc;
}