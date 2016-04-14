#include "Geometry/BlockMesh.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include <iostream>

BlockMesh::BlockMesh(const float width, const float height, const float length) :
    width(width),
    height(height),
    length(length)
{
    unsigned int vertexCount = 36; //6 sides * 2 triangles * 3 vertices
    float* vb = genVerts();
    float* normalBuffer = generateNormals(vb, vertexCount);
    float* uvBuffer = genUVs();

    vertices = new VertexBuffer(vb, vertexCount, 3);
    normals = new VertexBuffer(normalBuffer, vertexCount, 3);
    uvs = new VertexBuffer(uvBuffer, vertexCount, 2);

    setVertices(*vertices);
    setNormals(*normals);
    setTexCoords(*uvs);

    delete[] vb;
    delete[] normalBuffer;
    delete[] uvBuffer;
}

BlockMesh::~BlockMesh()
{
    delete[] vertices;
    delete[] normals;
    delete[] uvs;
}

void BlockMesh::setTime(const unsigned int time)
{
    SetUniformValue("time", time);
}

unsigned int BlockMesh::getTime()
{
    auto properties = material->getUintProperties();
    auto iterator = properties.find("time");
    unsigned int time = 0;
    if (iterator != properties.end())
    {
        time = iterator->second;
    }
    else
    {
        cout << "Warning: No property type time" << endl;
    }

    return time;
}

float* BlockMesh::genVerts()
{
    float* verts = new float[6 * 6 * 3];

    verts[0] = (width / 2.0f);
    verts[1] = (height / 2.0f);
    verts[2] = (length / 2.0f);
    verts[3] = -width / 2.0f;
    verts[4] = (height / 2.0f);
    verts[5] = (length / 2.0f);
    verts[6] = -width / 2.0f;
    verts[7] = -height / 2.0f;
    verts[8] = (length / 2.0f);
    verts[9] = (width / 2.0f);
    verts[10] = (height / 2.0f);
    verts[11] = (length / 2.0f);
    verts[12] = -width / 2.0f;
    verts[13] = -height / 2.0f;
    verts[14] = (length / 2.0f);
    verts[15] = (width / 2.0f);
    verts[16] = -height / 2.0f;
    verts[17] = (length / 2.0f);
    verts[18] = -width / 2.0f;
    verts[19] = (height / 2.0f);
    verts[20] = -length / 2.0f;
    verts[21] = (width / 2.0f);
    verts[22] = (height / 2.0f);
    verts[23] = -length / 2.0f;
    verts[24] = (width / 2.0f);
    verts[25] = -height / 2.0f;
    verts[26] = -length / 2.0f;
    verts[27] = -width / 2.0f;
    verts[28] = (height / 2.0f);
    verts[29] = -length / 2.0f;
    verts[30] = (width / 2.0f);
    verts[31] = -height / 2.0f;
    verts[32] = -length / 2.0f;
    verts[33] = -width / 2.0f;
    verts[34] = -height / 2.0f;
    verts[35] = -length / 2.0f;
    verts[36] = -width / 2.0f;
    verts[37] = (height / 2.0f);
    verts[38] = (length / 2.0f);
    verts[39] = (width / 2.0f);
    verts[40] = (height / 2.0f);
    verts[41] = (length / 2.0f);
    verts[42] = (width / 2.0f);
    verts[43] = (height / 2.0f);
    verts[44] = -length / 2.0f;
    verts[45] = -width / 2.0f;
    verts[46] = (height / 2.0f);
    verts[47] = (length / 2.0f);
    verts[48] = (width / 2.0f);
    verts[49] = (height / 2.0f);
    verts[50] = -length / 2.0f;
    verts[51] = -width / 2.0f;
    verts[52] = (height / 2.0f);
    verts[53] = -length / 2.0f;
    verts[54] = (width / 2.0f);
    verts[55] = -height / 2.0f;
    verts[56] = (length / 2.0f);
    verts[57] = -width / 2.0f;
    verts[58] = -height / 2.0f;
    verts[59] = (length / 2.0f);
    verts[60] = -width / 2.0f;
    verts[61] = -height / 2.0f;
    verts[62] = -length / 2.0f;
    verts[63] = (width / 2.0f);
    verts[64] = -height / 2.0f;
    verts[65] = (length / 2.0f);
    verts[66] = -width / 2.0f;
    verts[67] = -height / 2.0f;
    verts[68] = -length / 2.0f;
    verts[69] = (width / 2.0f);
    verts[70] = -height / 2.0f;
    verts[71] = -length / 2.0f;
    verts[72] = -width / 2.0f;
    verts[73] = (height / 2.0f);
    verts[74] = (length / 2.0f);
    verts[75] = -width / 2.0f;
    verts[76] = (height / 2.0f);
    verts[77] = -length / 2.0f;
    verts[78] = -width / 2.0f;
    verts[79] = -height / 2.0f;
    verts[80] = -length / 2.0f;
    verts[81] = -width / 2.0f;
    verts[82] = (height / 2.0f);
    verts[83] = (length / 2.0f);
    verts[84] = -width / 2.0f;
    verts[85] = -height / 2.0f;
    verts[86] = -length / 2.0f;
    verts[87] = -width / 2.0f;
    verts[88] = -height / 2.0f;
    verts[89] = (length / 2.0f);
    verts[90] = (width / 2.0f);
    verts[91] = (height / 2.0f);
    verts[92] = -length / 2.0f;
    verts[93] = (width / 2.0f);
    verts[94] = (height / 2.0f);
    verts[95] = (length / 2.0f);
    verts[96] = (width / 2.0f);
    verts[97] = -height / 2.0f;
    verts[98] = (length / 2.0f);
    verts[99] = (width / 2.0f);
    verts[100] = (height / 2.0f);
    verts[101] = -length / 2.0f;
    verts[102] = (width / 2.0f);
    verts[103] = -height / 2.0f;
    verts[104] = (length / 2.0f);
    verts[105] = (width / 2.0f);
    verts[106] = -height / 2.0f;
    verts[107] = -length / 2.0f;

    return verts;
}

float * BlockMesh::genUVs()
{
    float* uvs = new float[6 * 6 * 2];

    uvs[0] = 0;
    uvs[1] = 1;
    uvs[2] = 0;
    uvs[3] = 0;
    uvs[4] = 1;
    uvs[5] = 0;

    uvs[6] = 0;
    uvs[7] = 1;
    uvs[8] = 1;
    uvs[9] = 0;
    uvs[10] = 1;
    uvs[11] = 1;

    uvs[12] = 0;
    uvs[13] = 1;
    uvs[14] = 0;
    uvs[15] = 0;
    uvs[16] = 1;
    uvs[17] = 0;

    uvs[18] = 0;
    uvs[19] = 1;
    uvs[20] = 1;
    uvs[21] = 0;
    uvs[22] = 1;
    uvs[23] = 1;

    uvs[24] = 0;
    uvs[25] = 1;
    uvs[26] = 0;
    uvs[27] = 0;
    uvs[28] = 1;
    uvs[29] = 0;

    uvs[30] = 0;
    uvs[31] = 1;
    uvs[32] = 1;
    uvs[33] = 0;
    uvs[34] = 1;
    uvs[35] = 1;

    uvs[36] = 0;
    uvs[37] = 1;
    uvs[38] = 0;
    uvs[39] = 0;
    uvs[40] = 1;
    uvs[41] = 0;

    uvs[42] = 0;
    uvs[43] = 1;
    uvs[44] = 1;
    uvs[45] = 0;
    uvs[46] = 1;
    uvs[47] = 1;

    uvs[48] = 0;
    uvs[49] = 1;
    uvs[50] = 0;
    uvs[51] = 0;
    uvs[52] = 1;
    uvs[53] = 0;

    uvs[54] = 0;
    uvs[55] = 1;
    uvs[56] = 1;
    uvs[57] = 0;
    uvs[58] = 1;
    uvs[59] = 1;

    uvs[60] = 0;
    uvs[61] = 1;
    uvs[62] = 0;
    uvs[63] = 0;
    uvs[64] = 1;
    uvs[65] = 0;

    uvs[66] = 0;
    uvs[67] = 1;
    uvs[68] = 1;
    uvs[69] = 0;
    uvs[70] = 1;
    uvs[71] = 1;

    return uvs;
}
