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
        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_1D_SHADOW:
        case GL_SAMPLER_2D_SHADOW:
        case GL_SAMPLER_1D_ARRAY:
        case GL_SAMPLER_2D_ARRAY:
        case GL_SAMPLER_1D_ARRAY_SHADOW:
        case GL_SAMPLER_2D_ARRAY_SHADOW:
        case GL_SAMPLER_2D_MULTISAMPLE:
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_SAMPLER_CUBE_SHADOW:
        case GL_SAMPLER_BUFFER:
        case GL_SAMPLER_2D_RECT:
        case GL_SAMPLER_2D_RECT_SHADOW:
        case GL_INT_SAMPLER_1D:
        case GL_INT_SAMPLER_2D:
        case GL_INT_SAMPLER_3D:
        case GL_INT_SAMPLER_CUBE:
        case GL_INT_SAMPLER_1D_ARRAY:
        case GL_INT_SAMPLER_2D_ARRAY:
        case GL_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_INT_SAMPLER_BUFFER:
        case GL_INT_SAMPLER_2D_RECT:
        case GL_UNSIGNED_INT_SAMPLER_1D:
        case GL_UNSIGNED_INT_SAMPLER_2D:
        case GL_UNSIGNED_INT_SAMPLER_3D:
        case GL_UNSIGNED_INT_SAMPLER_CUBE:
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        {
            addProperty(iUniforms, name, values[2]);
            break;
        }
        case GL_UNSIGNED_INT:
        {
            addProperty(uiUniforms, name, values[2]);
            break;
        }
        case GL_FLOAT:
        {
            addProperty(fUniforms, name, values[2]);
            break;
        }
        case GL_FLOAT_VEC2:
        {
            addProperty(v2Uniforms, name, values[2]);
            break;
        }
        case GL_FLOAT_VEC3:
        {
            addProperty(v3Uniforms, name, values[2]);
            break;
        }
        case GL_FLOAT_MAT4:
        {
            addProperty(m4Uniforms, name, values[2]);
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
    textureCubemaps(rhs.textureCubemaps)
{
    copyProperties(iUniforms,  rhs.iUniforms);
    copyProperties(uiUniforms, rhs.uiUniforms);
    copyProperties(fUniforms,  rhs.fUniforms);
    copyProperties(v2Uniforms, rhs.v2Uniforms);
    copyProperties(v3Uniforms, rhs.v3Uniforms);
    copyProperties(m4Uniforms, rhs.m4Uniforms);
}

OpenGLMaterial::~OpenGLMaterial()
{
}

void OpenGLMaterial::getProperty(const std::string name, IntPropertySharedPtr& materialProperty)
{
    getProperty(iUniforms, name, materialProperty);
}

void OpenGLMaterial::getProperty(const std::string name, UintPropertySharedPtr& materialProperty)
{
    getProperty(uiUniforms, name, materialProperty);
}

void OpenGLMaterial::getProperty(const std::string name, FloatPropertySharedPtr& materialProperty)
{
    getProperty(fUniforms, name, materialProperty);
}

void OpenGLMaterial::getProperty(const std::string name, Vec2PropertySharedPtr& materialProperty)
{
    getProperty(v2Uniforms, name, materialProperty);
}

void OpenGLMaterial::getProperty(const std::string name, Vec3PropertySharedPtr& materialProperty)
{
    getProperty(v3Uniforms, name, materialProperty);
}

void OpenGLMaterial::getProperty(const std::string name, Mat4PropertySharedPtr& materialProperty)
{
    getProperty(m4Uniforms, name, materialProperty);
}

void OpenGLMaterial::setProperty(IntPropertySharedPtr p, const int v)
{
    OpenGLMaterialProperty<int>* glProperty = reinterpret_cast<OpenGLMaterialProperty<int>*>(p.get());
    glProperty->value = v;
}

void OpenGLMaterial::setProperty(UintPropertySharedPtr p, const unsigned int v)
{
    OpenGLMaterialProperty<unsigned int>* glProperty = reinterpret_cast<OpenGLMaterialProperty<unsigned int>*>(p.get());
    glProperty->value = v;
}

void OpenGLMaterial::setProperty(FloatPropertySharedPtr p, const float v)
{
    OpenGLMaterialProperty<float>* glProperty = reinterpret_cast<OpenGLMaterialProperty<float>*>(p.get());
    glProperty->value = v;
}

void OpenGLMaterial::setProperty(Vec2PropertySharedPtr p, const Vec2 & v)
{
    OpenGLMaterialProperty<Vec2>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Vec2>*>(p.get());
    glProperty->value = v;
}

void OpenGLMaterial::setProperty(Vec3PropertySharedPtr p, const Vec3 & v)
{
    OpenGLMaterialProperty<Vec3>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Vec3>*>(p.get());
    glProperty->value = v;
}

void OpenGLMaterial::setProperty(Mat4PropertySharedPtr p, const Matrix4 & v)
{
    OpenGLMaterialProperty<Matrix4>* glProperty = reinterpret_cast<OpenGLMaterialProperty<Matrix4>*>(p.get());
    glProperty->value = v;
}

int OpenGLMaterial::getId() const
{
    return id;
}

const std::vector<std::shared_ptr<OpenGLMaterialProperty<int>>>& OpenGLMaterial::getIntProperties() const
{
    return iUniforms;
}

const std::vector<std::shared_ptr<OpenGLMaterialProperty<unsigned int>>>& OpenGLMaterial::getUintProperties() const
{
    return uiUniforms;
}

const std::vector<OpenGLFloatPropertySharedPtr>& OpenGLMaterial::getFloatProperties() const
{
    return fUniforms;
}

const std::vector<OpenGLVec2PropertySharedPtr>& OpenGLMaterial::getVec2Properties() const
{
    return v2Uniforms;
}

const std::vector<OpenGLVec3PropertySharedPtr>& OpenGLMaterial::getVec3Properties() const
{
    return v3Uniforms;
}

const std::vector<OpenGLMat4PropertySharedPtr>& OpenGLMaterial::getMatrix4Properties() const
{
    return m4Uniforms;
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