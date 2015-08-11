#include "BlockMesh.hpp"
#include "GeometryAlgorithm.hpp"

BlockMesh::BlockMesh(const float width, const float height, const float length) :
width(width),
height(height),
length(length)
{
	unsigned int vertexCount = 36; //6 sides * 2 triangles * 3 vertices
	float* vb = genVerts();
	SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
	SetVertexBuffer(vb, vertexCount);
}

BlockMesh::~BlockMesh()
{

}

void BlockMesh::Render()
{
	time += 1;
	SetUniformValue("time", time);
	SetUniformValue("depthMvp", GetProjectionMatrix() * GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)) * GetModelMatrix());
	Mesh::Render();
}

void BlockMesh::SetShaders(const string vertexShaderPath, const string fragmentShaderPath)
{
	Mesh::SetShaders(vertexShaderPath, fragmentShaderPath);
	BindUniform("time");
	BindUniform("depthMvp");
	BindUniform("sampler");
	SetUniformValue("sampler", 0);
	Matrix4 proj = GeometryAlgorithm::CreatePerspectiveMatrix(3.14f / 4.0f, 1.0f, 10.0f, 1000);
	SetUniformValue("depthMvp", proj * GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 25), Vec3(0, 0, 0), Vec3(0, 1, 0)) * model);
	Matrix4 view = GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 2), Vec3(0, 0, 0), Vec3(0, 1, 0));

	SetProjectionMatrix(proj);
	SetViewMatrix(view);
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