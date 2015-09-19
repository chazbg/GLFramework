#pragma once
#include <vector>
#include <map>
#include <string>
#include <Core/Texture.hpp>
#include <Math/Vector.hpp>
#include <Math/Matrix.hpp>

using namespace std;

class IMaterial
{
public:
	virtual ~IMaterial() {}
	virtual int getId() const = 0;
	virtual vector<const Texture*>    getTextures() const = 0; //TODO: Don't copy the vector.
    virtual map<string, int>          getIntProperties() const = 0; //TODO: Don't copy the map. Refactor the many similar calls.
	virtual map<string, unsigned int> getUintProperties() const = 0; //TODO: Don't copy the map. Refactor the many similar calls.
	virtual map<string, float>        getFloatProperties() const = 0;//TODO: Don't copy the map. Refactor the many similar calls.
	virtual map<string, Vec3>         getVec3Properties() const = 0;//TODO: Don't copy the map. Refactor the many similar calls.
	virtual map<string, Matrix4>      getMatrix4Properties() const = 0;//TODO: Don't copy the map. Refactor the many similar calls.
    virtual void setProperty(const string name, const int v) = 0;
    virtual void setProperty(const string name, const unsigned int v) = 0;
    virtual void setProperty(const string name, const float v) = 0;
    virtual void setProperty(const string name, const Vec3& v) = 0;
    virtual void setProperty(const string name, const Matrix4& v) = 0;
};