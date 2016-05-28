#include "Rendering/Variants/OpenGL/OpenGLMaterial.hpp"
#include <GL/glew.h>

OpenGLMaterial::OpenGLMaterial(const int id) :
    id(id)
{
    GLint numActiveUniforms = 0;
    glGetProgramInterfaceiv(id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_LOCATION);
    properties.push_back(GL_ARRAY_SIZE);
    std::vector<GLint> values(properties.size());

    for (int uniform = 0; uniform < numActiveUniforms; ++uniform)
    {
        glGetProgramResourceiv(id, GL_UNIFORM, uniform, properties.size(), &properties[0], values.size(), NULL, &values[0]);
        glGetProgramResourceiv(id, GL_UNIFORM, uniform, properties.size(), &properties[1], values.size(), NULL, &values[1]);
        glGetProgramResourceiv(id, GL_UNIFORM, uniform, properties.size(), &properties[2], values.size(), NULL, &values[2]);

        nameData.resize(properties[0]); //The length of the name.
        glGetProgramResourceName(id, GL_UNIFORM, uniform, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0]);

        switch (values[1])
        {
        case GL_INT:
        {
            iUniforms[name].first = values[2];
            break;
        }
        case GL_UNSIGNED_INT:
        {
            uiUniforms[name].first = values[2];
            break;
        }
        case GL_FLOAT:
        {
            fUniforms[name].first = values[2];
            break;
        }
        case GL_FLOAT_VEC2:
        {
            v2Uniforms[name].first = values[2];
            break;
        }
        case GL_FLOAT_VEC3:
        {
            vUniforms[name].first = values[2];
            break;
        }
        case GL_FLOAT_MAT4:
        {
            mUniforms[name].first = values[2];
            break;
        }
        default:
            break;
        }

       // cout << "Uniform - Type: " << typeToString(values[1]) << " | Name: " << name << endl;
    }
}

OpenGLMaterial::OpenGLMaterial(const OpenGLMaterial& rhs) :
    id(rhs.id),
    textures(rhs.textures),
    textureCubemaps(rhs.textureCubemaps),
    iUniforms(rhs.iUniforms),
    uiUniforms(rhs.uiUniforms),
    fUniforms(rhs.fUniforms),
    v2Uniforms(rhs.v2Uniforms),
    vUniforms(rhs.vUniforms),
    mUniforms(rhs.mUniforms)
{

}

OpenGLMaterial::~OpenGLMaterial()
{
}

int OpenGLMaterial::getId() const
{
    return id;
}

const std::map<std::string, std::pair<int, int>>& OpenGLMaterial::getIntProperties() const
{
    return iUniforms;
}

const std::map<std::string, std::pair<int, unsigned int>>& OpenGLMaterial::getUintProperties() const
{
    return uiUniforms;
}

const std::map<std::string, std::pair<int, float>>& OpenGLMaterial::getFloatProperties() const
{
    return fUniforms;
}

const std::map<std::string, std::pair<int, Vec2>>& OpenGLMaterial::getVec2Properties() const
{
    return v2Uniforms;
}

const std::map<std::string, std::pair<int, Vec3>>& OpenGLMaterial::getVec3Properties() const
{
    return vUniforms;
}

const std::map<std::string, std::pair<int, Matrix4>>& OpenGLMaterial::getMatrix4Properties() const
{
    return mUniforms;
}

const std::vector<const ITexture*>& OpenGLMaterial::getTextures() const
{
    return textures;
}

const std::vector<const ITextureCubemap*>& OpenGLMaterial::getTextureCubemaps() const
{
    return textureCubemaps;
}

void OpenGLMaterial::addTexture(const ITexture* tex)
{
    textures.push_back(tex);
}

void OpenGLMaterial::addTextureCubemap(const ITextureCubemap* tex)
{
    textureCubemaps.push_back(tex);
}

void OpenGLMaterial::removeTexture(const ITexture* tex)
{
    vector<const ITexture*>::iterator it = find(textures.begin(), textures.end(), tex);
    if (textures.end() != it)
    {
        textures.erase(it);
    }
}

void OpenGLMaterial::removeTextureCubemap(const ITextureCubemap* tex)
{
    vector<const ITextureCubemap*>::iterator it = find(textureCubemaps.begin(), textureCubemaps.end(), tex);
    if (textureCubemaps.end() != it)
    {
        textureCubemaps.erase(it);
    }
}

void OpenGLMaterial::setProperty(const string name, const int v)
{
    setProperty(iUniforms, name, v);
}

void OpenGLMaterial::setProperty(const string name, const unsigned int v)
{
    setProperty(uiUniforms, name, v);
}

void OpenGLMaterial::setProperty(const string name, const float v)
{
    setProperty(fUniforms, name, v);
}

void OpenGLMaterial::setProperty(const std::string name, const Vec2 & v)
{
    setProperty(v2Uniforms, name, v);
}

void OpenGLMaterial::setProperty(const string name, const Vec3 & v)
{
    setProperty(vUniforms, name, v);
}

void OpenGLMaterial::setProperty(const string name, const Matrix4 & v)
{
    setProperty(mUniforms, name, v);
}
