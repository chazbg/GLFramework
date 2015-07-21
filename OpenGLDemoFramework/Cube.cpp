#include "Cube.hpp"
#include "Shader.hpp"
#include <cmath>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "GeometryAlgorithm.hpp"

Cube::Cube() :
Mesh()
{
	unsigned int vertexCount = 36; //6 sides * 2 triangles * 3 vertices
	float* vertexBuffer = genVerts();
	SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
	SetVertexBuffer(vertexBuffer, vertexCount);
}

Cube::~Cube()
{
}

void Cube::SetTime(GLuint time)
{
	this->time = time;
	SetUniformValue("time", time);
}

GLuint Cube::GetTime()
{
	return time;
}

void Cube::Render()
{
	SetTime(time + 1);

	Mesh::Render();
}

void Cube::RenderToTexture(const unsigned int texId)
{
	SetTime(time + 1);

	Mesh::RenderToTexture(texId);
}

void Cube::SetShaders(const string vertexShaderPath, const string fragmentShaderPath)
{
	Mesh::SetShaders(vertexShaderPath, fragmentShaderPath);
	BindUniform("time");
	SetUniformValue("time", 0);
	//SetProjectionMatrix(GeometryAlgorithm::CreatePerspectiveMatrix(3.14f / 6.0f, 1.0f, 0.5f, 5));
	SetViewMatrix(GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 5, -5), Vec3(0, 0, 0), Vec3(0, 1, 0)));
}

float* Cube::genVerts()
{
	float* verts = new float[6 * 6 * 3];

	verts[0] = 0.0f;
	verts[1] = 0.0f;
	verts[2] = 0.0f;
	verts[3] = 1.0f;
	verts[4] = 0.0f;
	verts[5] = 0.0f;
	verts[6] = 1.0f;
	verts[7] = 1.0f;
	verts[8] = 0.0f;
	verts[9] = 0.0f;
	verts[10] = 0.0f;
	verts[11] = 0.0f;
	verts[12] = 1.0f;
	verts[13] = 1.0f;
	verts[14] = 0.0f;
	verts[15] = 0.0f;
	verts[16] = 1.0f;
	verts[17] = 0.0f;
	verts[18] = 1.0f;
	verts[19] = 0.0f;
	verts[20] = 1.0f;
	verts[21] = 0.0f;
	verts[22] = 0.0f;
	verts[23] = 1.0f;
	verts[24] = 0.0f;
	verts[25] = 1.0f;
	verts[26] = 1.0f;
	verts[27] = 1.0f;
	verts[28] = 0.0f;
	verts[29] = 1.0f;
	verts[30] = 0.0f;
	verts[31] = 1.0f;
	verts[32] = 1.0f;
	verts[33] = 1.0f;
	verts[34] = 1.0f;
	verts[35] = 1.0f;
	verts[36] = 1.0f;
	verts[37] = 0.0f;
	verts[38] = 0.0f;
	verts[39] = 0.0f;
	verts[40] = 0.0f;
	verts[41] = 0.0f;
	verts[42] = 0.0f;
	verts[43] = 0.0f;
	verts[44] = 1.0f;
	verts[45] = 1.0f;
	verts[46] = 0.0f;
	verts[47] = 0.0f;
	verts[48] = 0.0f;
	verts[49] = 0.0f;
	verts[50] = 1.0f;
	verts[51] = 1.0f;
	verts[52] = 0.0f;
	verts[53] = 1.0f;
	verts[54] = 0.0f;
	verts[55] = 1.0f;
	verts[56] = 0.0f;
	verts[57] = 1.0f;
	verts[58] = 1.0f;
	verts[59] = 0.0f;
	verts[60] = 1.0f;
	verts[61] = 1.0f;
	verts[62] = 1.0f;
	verts[63] = 0.0f;
	verts[64] = 1.0f;
	verts[65] = 0.0f;
	verts[66] = 1.0f;
	verts[67] = 1.0f;
	verts[68] = 1.0f;
	verts[69] = 0.0f;
	verts[70] = 1.0f;
	verts[71] = 1.0f;
	verts[72] = 1.0f;
	verts[73] = 0.0f;
	verts[74] = 0.0f;
	verts[75] = 1.0f;
	verts[76] = 0.0f;
	verts[77] = 1.0f;
	verts[78] = 1.0f;
	verts[79] = 1.0f;
	verts[80] = 1.0f;
	verts[81] = 1.0f;
	verts[82] = 0.0f;
	verts[83] = 0.0f;
	verts[84] = 1.0f;
	verts[85] = 1.0f;
	verts[86] = 1.0f;
	verts[87] = 1.0f;
	verts[88] = 1.0f;
	verts[89] = 0.0f;
	verts[90] = 0.0f;
	verts[91] = 0.0f;
	verts[92] = 1.0f;
	verts[93] = 0.0f;
	verts[94] = 0.0f;
	verts[95] = 0.0f;
	verts[96] = 0.0f;
	verts[97] = 1.0f;
	verts[98] = 0.0f;
	verts[99] = 0.0f;
	verts[100] = 0.0f;
	verts[101] = 1.0f;
	verts[102] = 0.0f;
	verts[103] = 1.0f;
	verts[104] = 0.0f;
	verts[105] = 0.0f;
	verts[106] = 1.0f;
	verts[107] = 1.0f;

	return verts;
}