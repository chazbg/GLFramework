//#include "Geometry/PointListMesh.hpp"
//#include "Core/Shader.hpp"
//#include <cmath>
//
//#define BUFFER_OFFSET(i) ((void*)(i))
//
//PointListMesh::PointListMesh(const std::vector<Vec2>& pointsList, const Vec3& color, const float pointSize) : 
//Mesh(), color(color), pointSize(pointSize), is2d(true)
//{
//    vertexCount = pointsList.size();
//    vertexBuffer = genVerts(pointsList);
//
//    programID = LoadShaders("Shaders/primitives2d.vs", "Shaders/primitives2d.fs");
//    timeID = glGetUniformLocation(programID, "time");
//    colorID = glGetUniformLocation(programID, "color");
//    pointSizeID = glGetUniformLocation(programID, "pointSize");
//
//    time = 0;
//
//    UseProgram();
//
//    glUniform1ui(timeID, time);
//    glUniform3f(colorID, color.x, color.y, color.z);
//    glUniform1f(pointSizeID, pointSize);
//
//    glGenBuffers(1, &vertexBufferID);
//
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
//    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * 4, vertexBuffer, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(
//        0,
//        2,
//        GL_FLOAT,
//        GL_FALSE,
//        0,
//        BUFFER_OFFSET(0)
//        );
//
//    glDisableVertexAttribArray(0);
//}
//
//PointListMesh::PointListMesh(const std::vector<Vec3>& pointsList, const Vec3& color, const float pointSize) :
//Mesh(), color(color), pointSize(pointSize), is2d(false)
//{
//    vertexCount = pointsList.size();
//    vertexBuffer = genVerts(pointsList);
//
//    programID = LoadShaders("Shaders/primitives3d.vs", "Shaders/primitives3d.fs");
//    timeID = glGetUniformLocation(programID, "time");
//    colorID = glGetUniformLocation(programID, "color");
//    pointSizeID = glGetUniformLocation(programID, "pointSize");
//
//    time = 0;
//
//    UseProgram();
//
//    glUniform1ui(timeID, time);
//    glUniform3f(colorID, color.x, color.y, color.z);
//    glUniform1f(pointSizeID, pointSize);
//
//    glGenBuffers(1, &vertexBufferID);
//
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
//    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * 4, vertexBuffer, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(
//        0,
//        3,
//        GL_FLOAT,
//        GL_FALSE,
//        0,
//        BUFFER_OFFSET(0)
//        );
//
//    glDisableVertexAttribArray(0);
//}
//
//PointListMesh::~PointListMesh()
//{
//}
//
//void PointListMesh::SetTime(const GLuint time)
//{
//    this->time = time;
//    glUniform1ui(timeID, time);
//}
//
//GLuint PointListMesh::GetTime() const
//{
//    return time;
//}
//
//void PointListMesh::SetColor(const Vec3& color)
//{
//    this->color = color;
//    glUniform3f(colorID, color.x, color.y, color.z);
//}
//
//Vec3 PointListMesh::GetColor() const
//{
//    return color;
//}
//
//void PointListMesh::SetPointSize(const float pointSize)
//{
//    this->pointSize = pointSize;
//    glUniform1f(pointSizeID, pointSize);
//}
//
//float PointListMesh::GetPointSize() const
//{
//    return pointSize;
//}
//
//void PointListMesh::UseProgram()
//{
//    glUseProgram(programID);
//}
//
//void PointListMesh::Render()
//{
//    UseProgram();
//    SetTime(time + 1);
//    SetPointSize(pointSize);
//    SetColor(color);
//
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(
//        0,
//        is2d ? 2 : 3,
//        GL_FLOAT,
//        GL_FALSE,
//        0,
//        BUFFER_OFFSET(0)
//        );
//
//    glDrawArrays(GL_POINTS, 0, vertexCount);
//
//    glDisableVertexAttribArray(0);
//}
//
//float* PointListMesh::genVerts(const std::vector<Vec2>& pointsList)
//{
//    float* verts = new float[pointsList.size() * 2];
//
//    for (unsigned int i = 0; i < pointsList.size(); i++)
//    {
//        verts[2 * i] = pointsList[i].x;
//        verts[2 * i + 1] = pointsList[i].y;
//    }
//
//    return verts;
//}
//
//float* PointListMesh::genVerts(const std::vector<Vec3>& pointsList)
//{
//    float* verts = new float[pointsList.size() * 3];
//
//    for (unsigned int i = 0; i < pointsList.size(); i++)
//    {
//        verts[3 * i] = pointsList[i].x;
//        verts[3 * i + 1] = pointsList[i].y;
//        verts[3 * i + 2] = pointsList[i].z;
//    }
//
//    return verts;
//}