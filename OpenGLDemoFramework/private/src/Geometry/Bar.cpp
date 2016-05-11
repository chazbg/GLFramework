//#include "Bar.hpp"
//#include "Shader.hpp"
//#include <cmath>
//
//const GLfloat Bar::g_vertex_buffer_data[] = {
//    //front side
//    //1
//    0.0f, 0.0f, 0.0f,
//    1.0f, 0.0f, 0.0f,
//    1.0f, 1.0f, 0.0f,
//
//    //2
//    0.0f, 0.0f, 0.0f,
//    1.0f, 1.0f, 0.0f,
//    0.0f, 1.0f, 0.0f,
//
//    //3
//    0.0f, 1.0f, 0.0f,
//    1.0f, 1.0f, 0.0f,
//    1.0f, 2.0f, 0.0f,
//
//    //4
//    0.0f, 1.0f, 0.0f,
//    1.0f, 2.0f, 0.0f,
//    0.0f, 2.0f, 0.0f,
//
//    //5
//    0.0f, 2.0f, 0.0f,
//    1.0f, 2.0f, 0.0f,
//    1.0f, 3.0f, 0.0f,
//
//    //6
//    0.0f, 2.0f, 0.0f,
//    1.0f, 3.0f, 0.0f,
//    0.0f, 3.0f, 0.0f,
//
//    //7
//    0.0f, 3.0f, 0.0f,
//    1.0f, 3.0f, 0.0f,
//    1.0f, 4.0f, 0.0f,
//
//    //8
//    0.0f, 3.0f, 0.0f,
//    1.0f, 4.0f, 0.0f,
//    0.0f, 4.0f, 0.0f,
//
//    //9
//    0.0f, 4.0f, 0.0f,
//    1.0f, 4.0f, 0.0f,
//    1.0f, 5.0f, 0.0f,
//
//    //10
//    0.0f, 4.0f, 0.0f,
//    1.0f, 5.0f, 0.0f,
//    0.0f, 5.0f, 0.0f,
//
//    //11
//    0.0f, 5.0f, 0.0f,
//    1.0f, 5.0f, 0.0f,
//    1.0f, 6.0f, 0.0f,
//
//    //12
//    0.0f, 5.0f, 0.0f,
//    1.0f, 6.0f, 0.0f,
//    0.0f, 6.0f, 0.0f,
//
//    //13
//    0.0f, 6.0f, 0.0f,
//    1.0f, 6.0f, 0.0f,
//    1.0f, 7.0f, 0.0f,
//
//    //14
//    0.0f, 6.0f, 0.0f,
//    1.0f, 7.0f, 0.0f,
//    0.0f, 7.0f, 0.0f,
//
//    //15
//    0.0f, 7.0f, 0.0f,
//    1.0f, 7.0f, 0.0f,
//    1.0f, 8.0f, 0.0f,
//
//    //16
//    0.0f, 7.0f, 0.0f,
//    1.0f, 8.0f, 0.0f,
//    0.0f, 8.0f, 0.0f,
//
//    //17
//    0.0f, 8.0f, 0.0f,
//    1.0f, 8.0f, 0.0f,
//    1.0f, 9.0f, 0.0f,
//
//    //18
//    0.0f, 8.0f, 0.0f,
//    1.0f, 9.0f, 0.0f,
//    0.0f, 9.0f, 0.0f,
//
//    //19
//    0.0f, 9.0f, 0.0f,
//    1.0f, 9.0f, 0.0f,
//    1.0f, 10.0f, 0.0f,
//
//    //20
//    0.0f, 9.0f, 0.0f,
//    1.0f, 10.0f, 0.0f,
//    0.0f, 10.0f, 0.0f,
//
//    //21
//    0.0f, 10.0f, 0.0f,
//    1.0f, 10.0f, 0.0f,
//    1.0f, 11.0f, 0.0f,
//
//    //22
//    0.0f, 10.0f, 0.0f,
//    1.0f, 11.0f, 0.0f,
//    0.0f, 11.0f, 0.0f,
//
//    //23
//    0.0f, 11.0f, 0.0f,
//    1.0f, 11.0f, 0.0f,
//    1.0f, 12.0f, 0.0f,
//
//    //24
//    0.0f, 11.0f, 0.0f,
//    1.0f, 12.0f, 0.0f,
//    0.0f, 12.0f, 0.0f,
//
//    //back side
//    //1
//    1.0f, 0.0f, 1.0f,
//    0.0f, 0.0f, 1.0f,
//    0.0f, 1.0f, 1.0f,
//
//    //2
//    1.0f, 0.0f, 1.0f,
//    0.0f, 1.0f, 1.0f,
//    1.0f, 1.0f, 1.0f,
//
//    //3
//    1.0f, 1.0f, 1.0f,
//    0.0f, 1.0f, 1.0f,
//    0.0f, 2.0f, 1.0f,
//
//    //4
//    1.0f, 1.0f, 1.0f,
//    0.0f, 2.0f, 1.0f,
//    1.0f, 2.0f, 1.0f,
//
//    //5
//    1.0f, 2.0f, 1.0f,
//    0.0f, 2.0f, 1.0f,
//    0.0f, 3.0f, 1.0f,
//
//    //6
//    1.0f, 2.0f, 1.0f,
//    0.0f, 3.0f, 1.0f,
//    1.0f, 3.0f, 1.0f,
//
//    //7
//    1.0f, 3.0f, 1.0f,
//    0.0f, 3.0f, 1.0f,
//    0.0f, 4.0f, 1.0f,
//
//    //8
//    1.0f, 3.0f, 1.0f,
//    0.0f, 4.0f, 1.0f,
//    1.0f, 4.0f, 1.0f,
//
//    //9
//    1.0f, 4.0f, 1.0f,
//    0.0f, 4.0f, 1.0f,
//    0.0f, 5.0f, 1.0f,
//
//    //10
//    1.0f, 4.0f, 1.0f,
//    0.0f, 5.0f, 1.0f,
//    1.0f, 5.0f, 1.0f,
//
//    //11
//    1.0f, 5.0f, 1.0f,
//    0.0f, 5.0f, 1.0f,
//    0.0f, 6.0f, 1.0f,
//
//    //12
//    1.0f, 5.0f, 1.0f,
//    0.0f, 6.0f, 1.0f,
//    1.0f, 6.0f, 1.0f,
//
//    //13
//    1.0f, 6.0f, 1.0f,
//    0.0f, 6.0f, 1.0f,
//    0.0f, 7.0f, 1.0f,
//
//    //14
//    1.0f, 6.0f, 1.0f,
//    0.0f, 7.0f, 1.0f,
//    1.0f, 7.0f, 1.0f,
//
//    //15
//    1.0f, 7.0f, 1.0f,
//    0.0f, 7.0f, 1.0f,
//    0.0f, 8.0f, 1.0f,
//
//    //16
//    1.0f, 7.0f, 1.0f,
//    0.0f, 8.0f, 1.0f,
//    1.0f, 8.0f, 1.0f,
//
//    //17
//    1.0f, 8.0f, 1.0f,
//    0.0f, 8.0f, 1.0f,
//    0.0f, 9.0f, 1.0f,
//
//    //18
//    1.0f, 8.0f, 1.0f,
//    0.0f, 9.0f, 1.0f,
//    1.0f, 9.0f, 1.0f,
//
//    //19
//    1.0f, 9.0f, 1.0f,
//    0.0f, 9.0f, 1.0f,
//    0.0f, 10.0f, 1.0f,
//
//    //20
//    1.0f, 9.0f, 1.0f,
//    0.0f, 10.0f, 1.0f,
//    1.0f, 10.0f, 1.0f,
//
//    //21
//    1.0f, 10.0f, 1.0f,
//    0.0f, 10.0f, 1.0f,
//    0.0f, 11.0f, 1.0f,
//
//    //22
//    1.0f, 10.0f, 1.0f,
//    0.0f, 11.0f, 1.0f,
//    1.0f, 11.0f, 1.0f,
//
//    //23
//    1.0f, 11.0f, 1.0f,
//    0.0f, 11.0f, 1.0f,
//    0.0f, 12.0f, 1.0f,
//
//    //24
//    1.0f, 11.0f, 1.0f,
//    0.0f, 12.0f, 1.0f,
//    1.0f, 12.0f, 1.0f,
//
//    //bottom side
//    1.0f, 0.0f, 0.0f,
//    0.0f, 0.0f, 0.0f,
//    0.0f, 0.0f, 1.0f,
//
//    1.0f, 0.0f, 0.0f,
//    0.0f, 0.0f, 1.0f,
//    1.0f, 0.0f, 1.0f,
//
//    //top side
//    0.0f, 12.0f, 0.0f,
//    1.0f, 12.0f, 0.0f,
//    1.0f, 12.0f, 1.0f,
//
//    0.0f, 12.0f, 0.0f,
//    1.0f, 12.0f, 1.0f,
//    0.0f, 12.0f, 1.0f,
//
//    //right side
//    //1
//    1.0f, 0.0f, 0.0f,
//    1.0f, 0.0f, 1.0f,
//    1.0f, 1.0f, 1.0f,
//
//    //2
//    1.0f, 0.0f, 0.0f,
//    1.0f, 1.0f, 1.0f,
//    1.0f, 1.0f, 0.0f,
//
//    //3
//    1.0f, 1.0f, 0.0f,
//    1.0f, 1.0f, 1.0f,
//    1.0f, 2.0f, 1.0f,
//
//    //4
//    1.0f, 1.0f, 0.0f,
//    1.0f, 2.0f, 1.0f,
//    1.0f, 2.0f, 0.0f,
//
//    //5
//    1.0f, 2.0f, 0.0f,
//    1.0f, 2.0f, 1.0f,
//    1.0f, 3.0f, 1.0f,
//
//    //6
//    1.0f, 2.0f, 0.0f,
//    1.0f, 3.0f, 1.0f,
//    1.0f, 3.0f, 0.0f,
//
//    //7
//    1.0f, 3.0f, 0.0f,
//    1.0f, 3.0f, 1.0f,
//    1.0f, 4.0f, 1.0f,
//
//    //8
//    1.0f, 3.0f, 0.0f,
//    1.0f, 4.0f, 1.0f,
//    1.0f, 4.0f, 0.0f,
//
//    //9
//    1.0f, 4.0f, 0.0f,
//    1.0f, 4.0f, 1.0f,
//    1.0f, 5.0f, 1.0f,
//
//    //10
//    1.0f, 4.0f, 0.0f,
//    1.0f, 5.0f, 1.0f,
//    1.0f, 5.0f, 0.0f,
//
//    //11
//    1.0f, 5.0f, 0.0f,
//    1.0f, 5.0f, 1.0f,
//    1.0f, 6.0f, 1.0f,
//
//    //12
//    1.0f, 5.0f, 0.0f,
//    1.0f, 6.0f, 1.0f,
//    1.0f, 6.0f, 0.0f,
//
//    //13
//    1.0f, 6.0f, 0.0f,
//    1.0f, 6.0f, 1.0f,
//    1.0f, 7.0f, 1.0f,
//
//    //14
//    1.0f, 6.0f, 0.0f,
//    1.0f, 7.0f, 1.0f,
//    1.0f, 7.0f, 0.0f,
//
//    //15
//    1.0f, 7.0f, 0.0f,
//    1.0f, 7.0f, 1.0f,
//    1.0f, 8.0f, 1.0f,
//
//    //16
//    1.0f, 7.0f, 0.0f,
//    1.0f, 8.0f, 1.0f,
//    1.0f, 8.0f, 0.0f,
//
//    //17
//    1.0f, 8.0f, 0.0f,
//    1.0f, 8.0f, 1.0f,
//    1.0f, 9.0f, 1.0f,
//
//    //18
//    1.0f, 8.0f, 0.0f,
//    1.0f, 9.0f, 1.0f,
//    1.0f, 9.0f, 0.0f,
//
//    //19
//    1.0f, 9.0f, 0.0f,
//    1.0f, 9.0f, 1.0f,
//    1.0f, 10.0f, 1.0f,
//
//    //20
//    1.0f, 9.0f, 0.0f,
//    1.0f, 10.0f, 1.0f,
//    1.0f, 10.0f, 0.0f,
//
//    //21
//    1.0f, 10.0f, 0.0f,
//    1.0f, 10.0f, 1.0f,
//    1.0f, 11.0f, 1.0f,
//
//    //22
//    1.0f, 10.0f, 0.0f,
//    1.0f, 11.0f, 1.0f,
//    1.0f, 11.0f, 0.0f,
//
//    //23
//    1.0f, 11.0f, 0.0f,
//    1.0f, 11.0f, 1.0f,
//    1.0f, 12.0f, 1.0f,
//
//    //24
//    1.0f, 11.0f, 0.0f,
//    1.0f, 12.0f, 1.0f,
//    1.0f, 12.0f, 0.0f,
//
//    //left side
//    //1
//    0.0f, 0.0f, 1.0f,
//    0.0f, 0.0f, 0.0f,
//    0.0f, 1.0f, 0.0f,
//
//    //2
//    0.0f, 0.0f, 1.0f,
//    0.0f, 1.0f, 0.0f,
//    0.0f, 1.0f, 1.0f,
//
//    //3
//    0.0f, 1.0f, 1.0f,
//    0.0f, 1.0f, 0.0f,
//    0.0f, 2.0f, 0.0f,
//
//    //4
//    0.0f, 1.0f, 1.0f,
//    0.0f, 2.0f, 0.0f,
//    0.0f, 2.0f, 1.0f,
//
//    //5
//    0.0f, 2.0f, 1.0f,
//    0.0f, 2.0f, 0.0f,
//    0.0f, 3.0f, 0.0f,
//
//    //6
//    0.0f, 2.0f, 1.0f,
//    0.0f, 3.0f, 0.0f,
//    0.0f, 3.0f, 1.0f,
//
//    //7
//    0.0f, 3.0f, 1.0f,
//    0.0f, 3.0f, 0.0f,
//    0.0f, 4.0f, 0.0f,
//
//    //8
//    0.0f, 3.0f, 1.0f,
//    0.0f, 4.0f, 0.0f,
//    0.0f, 4.0f, 1.0f,
//
//    //9
//    0.0f, 4.0f, 1.0f,
//    0.0f, 4.0f, 0.0f,
//    0.0f, 5.0f, 0.0f,
//
//    //10
//    0.0f, 4.0f, 1.0f,
//    0.0f, 5.0f, 0.0f,
//    0.0f, 5.0f, 1.0f,
//
//    //11
//    0.0f, 5.0f, 1.0f,
//    0.0f, 5.0f, 0.0f,
//    0.0f, 6.0f, 0.0f,
//
//    //12
//    0.0f, 5.0f, 1.0f,
//    0.0f, 6.0f, 0.0f,
//    0.0f, 6.0f, 1.0f,
//
//    //13
//    0.0f, 6.0f, 1.0f,
//    0.0f, 6.0f, 0.0f,
//    0.0f, 7.0f, 0.0f,
//
//    //14
//    0.0f, 6.0f, 1.0f,
//    0.0f, 7.0f, 0.0f,
//    0.0f, 7.0f, 1.0f,
//
//    //15
//    0.0f, 7.0f, 1.0f,
//    0.0f, 7.0f, 0.0f,
//    0.0f, 8.0f, 0.0f,
//
//    //16
//    0.0f, 7.0f, 1.0f,
//    0.0f, 8.0f, 0.0f,
//    0.0f, 8.0f, 1.0f,
//
//    //17
//    0.0f, 8.0f, 1.0f,
//    0.0f, 8.0f, 0.0f,
//    0.0f, 9.0f, 0.0f,
//
//    //18
//    0.0f, 8.0f, 1.0f,
//    0.0f, 9.0f, 0.0f,
//    0.0f, 9.0f, 1.0f,
//
//    //19
//    0.0f, 9.0f, 1.0f,
//    0.0f, 9.0f, 0.0f,
//    0.0f, 10.0f, 0.0f,
//
//    //20
//    0.0f, 9.0f, 1.0f,
//    0.0f, 10.0f, 0.0f,
//    0.0f, 10.0f, 1.0f,
//
//    //21
//    0.0f, 10.0f, 1.0f,
//    0.0f, 10.0f, 0.0f,
//    0.0f, 11.0f, 0.0f,
//
//    //22
//    0.0f, 10.0f, 1.0f,
//    0.0f, 11.0f, 0.0f,
//    0.0f, 11.0f, 1.0f,
//
//    //23
//    0.0f, 11.0f, 1.0f,
//    0.0f, 11.0f, 0.0f,
//    0.0f, 12.0f, 0.0f,
//
//    //24
//    0.0f, 11.0f, 1.0f,
//    0.0f, 12.0f, 0.0f,
//    0.0f, 12.0f, 1.0f,
//};
//
//Bar::Bar()
//{
//    programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
//    timeID = glGetUniformLocation(programID, "time");
//    segmentsID = glGetUniformLocation(programID, "segments");
//    startAngleID = glGetUniformLocation(programID, "startAngle");
//    offsetAngleID = glGetUniformLocation(programID, "offsetAngle");
//    fillPercentageID = glGetUniformLocation(programID, "fillPercentage");
//
//    time = 0;
//    segments = 0;
//    startAngle = 0;
//    offsetAngle = 0;
//    fillPercentage = 0;
//
//    glUniform1ui(timeID, time);
//    glUniform1ui(segmentsID, segments);
//    glUniform1f(startAngleID, startAngle);
//    glUniform1f(offsetAngleID, offsetAngle);
//    glUniform1f(fillPercentageID, fillPercentage);
//
//    glGenBuffers(1, &vertexBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glVertexAttribPointer(
//        0,        
//        3,                 
//        GL_FLOAT,
//        GL_FALSE,
//        0,
//        (void*)0
//        );
//
//    UseProgram();
//}
//
//Bar::~Bar()
//{
//    glDisableVertexAttribArray(0);
//}
//
//void Bar::SetTime(GLuint time)
//{
//    this->time = time;
//    glUniform1ui(timeID, time);
//}
//
//GLuint Bar::GetTime()
//{
//    return time;
//}
//
//void Bar::SetSegments(GLuint segments)
//{
//    this->segments = segments;
//    glUniform1ui(segmentsID, segments);
//}
//
//
//GLuint Bar::GetSegments()
//{
//    return segments;
//}
//
//void Bar::SetStartAngle(GLfloat startAngle)
//{
//    this->startAngle = startAngle;
//    glUniform1f(startAngleID, startAngle);
//}
//
//GLfloat Bar::GetStartAngle()
//{
//    return startAngle;
//}
//
//void Bar::SetOffsetAngle(GLfloat offsetAngle)
//{
//    this->offsetAngle = offsetAngle;
//    glUniform1f(offsetAngleID, offsetAngle);
//}
//
//GLfloat Bar::GetOffsetAngle()
//{
//    return offsetAngle;
//}
//
//void Bar::SetFillPercentage(GLfloat fillPercentage)
//{
//    this->fillPercentage = fillPercentage;
//    glUniform1f(fillPercentageID, fillPercentage);
//}
//
//
//GLfloat Bar::GetFillPercentage()
//{
//    return fillPercentage;
//}
//
//void Bar::UseProgram()
//{
//    glUseProgram(programID);
//}
//
//void Bar::Render()
//{
//    //UseProgram();
//    SetTime(time + 1);
//    SetFillPercentage(abs(sin(time * 0.02f)));
//    //SetFillPercentage(4.0f);
//    SetOffsetAngle(3.14f / 2.0f);
//    SetSegments(12);
//    SetStartAngle(3.0f * 3.14f / 2.0f);
//
//    //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//    //glEnableVertexAttribArray(0);
//    //glVertexAttribPointer(
//    //    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//    //    3,                  // size
//    //    GL_FLOAT,           // type
//    //    GL_FALSE,           // normalized?
//    //    0,                  // stride
//    //    (void*)0            // array buffer offset
//    //    );
//
//    glDrawArrays(GL_TRIANGLES, 0, 300); // 144 + 12 + 144 indices starting at 0 -> 48 + 4 + 48 triangles
//
//    //glDisableVertexAttribArray(0);
//}