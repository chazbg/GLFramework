#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "Core/Shader.hpp"
#include <map>

using namespace std;

static map<pair<string, string>, int> shaders;

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

    map<pair<string, string>, int>::iterator it;
    if ((it = shaders.find(pair<string, string>(vertex_file_path, fragment_file_path))) != shaders.end())
    {
        return it->second;
    }

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }



    GLint Result = GL_FALSE;
    int InfoLogLength;



    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    shaders[pair<string, string>(vertex_file_path, fragment_file_path)] = ProgramID;

    return ProgramID;
}

static string typeToString(const int glslType)
{
    string ret;
    switch (glslType)
    {
    case GL_FLOAT:
        ret = "GL_FLOAT";
        break;
    case GL_FLOAT_VEC2:
        ret = "GL_FLOAT_VEC2";
        break;
    case GL_FLOAT_VEC3:
        ret = "GL_FLOAT_VEC3";
        break;
    case GL_FLOAT_VEC4:
        ret = "GL_FLOAT_VEC4";
        break;
    case GL_DOUBLE:
        ret = "GL_DOUBLE";
        break;
    case GL_DOUBLE_VEC2:
        ret = "GL_DOUBLE_VEC2";
        break;
    case GL_DOUBLE_VEC3:
        ret = "GL_DOUBLE_VEC3";
        break;
    case GL_DOUBLE_VEC4:
        ret = "GL_DOUBLE_VEC4";
        break;
    case GL_INT:
        ret = "GL_INT";
        break;
    case GL_UNSIGNED_INT:
        ret = "GL_UNSIGNED_INT";
        break;
    case GL_BOOL:
        ret = "GL_BOOL";
        break;
    case GL_FLOAT_MAT2:
        ret = "GL_FLOAT_MAT2";
        break;
    case GL_FLOAT_MAT3:
        ret = "GL_FLOAT_MAT3";
        break;
    case GL_FLOAT_MAT4:
        ret = "GL_FLOAT_MAT4";
        break;
    case GL_SAMPLER_1D:
        ret = "GL_SAMPLER_1D";
        break;
    case GL_SAMPLER_2D:
        ret = "GL_SAMPLER_2D";
        break;
    case GL_SAMPLER_3D:
        ret = "GL_SAMPLER_3D";
        break;
    case GL_SAMPLER_CUBE:
        ret = "GL_SAMPLER_CUBE";
        break;
    default:
        ret = "Unknown";
        break;
    };

    return ret;
}

void enumerateProperties(const GLuint programId)
{
    GLint numActiveAttribs = 0;
    GLint numActiveUniforms = 0;
    glGetProgramInterfaceiv(programId, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
    glGetProgramInterfaceiv(programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);
    std::vector<GLint> values(properties.size());
    cout << "---------------------------" << endl;
    cout << "Enumerating Program ID: " << programId << endl;

    for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
    {
        glGetProgramResourceiv(programId, GL_PROGRAM_INPUT, attrib, properties.size(), &properties[0], values.size(), NULL, &values[0]);
        glGetProgramResourceiv(programId, GL_PROGRAM_INPUT, attrib, properties.size(), &properties[1], values.size(), NULL, &values[1]);

        nameData.resize(properties[0]); //The length of the name.
        glGetProgramResourceName(programId, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0]);

        cout << "Attribute - Type: " << typeToString(values[1]) << " | Name: " << name << endl;
    }

    for (int uniform = 0; uniform < numActiveUniforms; ++uniform)
    {
        glGetProgramResourceiv(programId, GL_UNIFORM, uniform, properties.size(), &properties[0], values.size(), NULL, &values[0]);
        glGetProgramResourceiv(programId, GL_UNIFORM, uniform, properties.size(), &properties[1], values.size(), NULL, &values[1]);

        nameData.resize(properties[0]); //The length of the name.
        glGetProgramResourceName(programId, GL_UNIFORM, uniform, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0]);

        cout << "Uniform - Type: " << typeToString(values[1]) << " | Name: " << name << endl;
    }
}