//#include "Geometry/Triangle.hpp"
//#include "Math/GeometryAlgorithm.hpp"
//
//Triangle::Triangle()
//{
//	unsigned int vertexCount = 3;
//	float* vb = genVerts();
//	SetShaders("Shaders/cube.vs", "Shaders/cube.fs");
//	SetVertexBuffer(vb, vertexCount, 3);
//}
//
//Triangle::~Triangle()
//{
//
//}
//
//void Triangle::Render()
//{
//	time += 1;
//	SetUniformValue("time", time);
//	Mesh::Render();
//}
//
//void Triangle::SetShaders(const string vertexShaderPath, const string fragmentShaderPath)
//{
//	Mesh::SetShaders(vertexShaderPath, fragmentShaderPath);
//	BindUniform("time");
//	BindUniform("depthMvp");
//	BindUniform("sampler");
//	SetUniformValue("sampler", 0);
//	SetUniformValue("depthMvp", GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 10.0, 10.0), Vec3(0, 0, 0), Vec3(0, 1, 0)));
//	Matrix4 proj = GeometryAlgorithm::CreatePerspectiveMatrix(3.14f / 6.0f, 1.0f, 0.1f, 1000);
//	Matrix4 view = GeometryAlgorithm::CreateLookAtMatrix(Vec3(0, 0, 100), Vec3(0, 0, 0), Vec3(0, 1, 0));
//	//Matrix4 model = Matrix4(Vec4(0.1f, 0, 0, 0),
//	//	Vec4(0, 0.1f, 0, 0),
//	//	Vec4(0, 0, 0.1f, 0),
//	//	Vec4(0, 0, 0, 1));
//
//	SetProjectionMatrix(proj);
//	//SetModelMatrix(model);
//	SetViewMatrix(view);
//	//cout << "view " << endl << (view).toString() << endl;
//	cout << "prj " << endl << (proj * view).toString() << endl;
//	cout << "res " << endl << (proj * view * Vec4(10, 10, 10, 1)).toString() << endl;
//	cout << "res " << endl << (proj * view * Vec4(10.0, 10.0, 10.0, 1)).toString() << endl;
//	cout << "res " << endl << (proj * view * Vec4(-10.0, -10.0, -10.0, 1)).toString() << endl;
//
//}
//
//float* Triangle::genVerts()
//{
//	float* verts = new float[3 * 3];
//
//	verts[0] = -10.0f;
//	verts[1] = -10.0f;
//	verts[2] = 0.0f;
//	verts[3] = 10.0f;
//	verts[4] = -10.0f;
//	verts[5] = 0.0f;
//	verts[6] = 0.0f;
//	verts[7] = 10.0f;
//	verts[8] = 0.0f;
//
//	return verts;
//}