#pragma once

#include "Core/Animation/IAnimation.hpp"
#include "Core/Animation/AnimationKeyFrame.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

template<class T>
class LinearAnimation : public IAnimation
{
public:
    LinearAnimation();
    void addKeyframe(const AnimationKeyframe<T>& keyframe);
    virtual void update(const float t);
    const T& getValue() const;
    bool isExpired() const;
private:
    bool expired;
    unsigned int interval;
    float time;
    T value;
    std::vector<AnimationKeyframe<T>> keyframes;
};

template<class T>
LinearAnimation<T>::LinearAnimation() : expired(false), interval(0), time(0.0f)
{
}

template<class T>
void LinearAnimation<T>::addKeyframe(const AnimationKeyframe<T>& keyframe)
{
    keyframes.push_back(keyframe);
    std::sort(keyframes.begin(), keyframes.end());
    value = keyframes[0].value;
}

template<class T>
void LinearAnimation<T>::update(const float t)
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

template<class T>
const T& LinearAnimation<T>::getValue() const
{
    return value;
}

template<class T>
bool LinearAnimation<T>::isExpired() const
{
    return expired;
}