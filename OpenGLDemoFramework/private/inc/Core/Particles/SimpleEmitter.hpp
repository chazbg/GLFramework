#pragma once

#include "Core/Particles/IEmitter.hpp"
#include "Core/Particles/IParticle2D.hpp"
#include "Core/Particles/IParticle2DRenderer.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std::placeholders;

class IAnimation
{
public:
    virtual void update(const float delta) = 0;
};

template<class T>
struct AnimationKeyframe
{
    AnimationKeyframe(const float time, const T& value) : time(time), value(value) {}
    bool operator<(const AnimationKeyframe& rhs)
    {
        return time < rhs.time;
    }

    bool operator<=(const AnimationKeyframe& rhs)
    {
        return time <= rhs.time;
    }

    float time;
    T value;
};

template<class T>
class LinearAnimation
{
public:
    LinearAnimation() : expired(false), interval(0), time(0.0f) 
    {
        std::cout << interval << std::endl;
    }
    void addKeyframe(const AnimationKeyframe<T>& keyframe)
    {
        keyframes.push_back(keyframe);
        std::sort(keyframes.begin(), keyframes.end());
        value = keyframes[0].value;
    }

    void update(const float t)
    {
        if (keyframes.size() >= 2)
        {
            AnimationKeyframe<T>& a = keyframes[interval];
            AnimationKeyframe<T>& b = keyframes[interval + 1];
            float normalizedTime = (time - a.time) / (b.time - a.time);
            value = (1.0f - normalizedTime) * a.value + normalizedTime * b.value;

            time += t;

            if (time >= b.time)
            {
                interval++;
                if (interval >= keyframes.size() - 1)
                {
                    expired = true;
                }
            }
        }
    }

    const T& getValue()
    {
        return value;
    }

    bool isExpired()
    {
        return expired;
    }
private:
    bool expired;
    unsigned int interval;
    float time;
    T value;
    std::vector<AnimationKeyframe<T>> keyframes;
};

class SimpleEmitter : public IEmitter
{
public:
    SimpleEmitter(
        IParticle2DRenderer& particleRenderer,
        const unsigned int particleCount,
        const float spawnInterval);
    ~SimpleEmitter();
    virtual void updateParticles(const float t);
    virtual void updateParticles(const float t, const bool beat);
    const std::vector<IParticle2D*>& getParticles();
    unsigned int getAliveParticlesCount();
    float getScale();
private:
    void addAnimation();
    void calculateScale();
    IParticle2DRenderer& particleRenderer;
    std::vector<IParticle2D*> particles;
    const unsigned int particleCount;
    const float spawnInterval;
    unsigned int aliveParticles;
    float nextSpawn;
    float scale;
    std::vector<LinearAnimation<float>> animations;
};