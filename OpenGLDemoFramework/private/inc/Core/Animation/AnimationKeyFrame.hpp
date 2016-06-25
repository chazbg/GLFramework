#pragma once

template<class T>
struct AnimationKeyframe
{
    AnimationKeyframe(const float time, const T& value);
    bool operator<(const AnimationKeyframe& rhs) const;
    bool operator<=(const AnimationKeyframe& rhs) const;
    float time;
    T value;
};

template <class T>
AnimationKeyframe<T>::AnimationKeyframe(const float time, const T& value) : time(time), value(value)
{

}

template <class T>
bool AnimationKeyframe<T>::operator<(const AnimationKeyframe& rhs) const
{
    return time < rhs.time;
}

template <class T>
bool AnimationKeyframe<T>::operator<=(const AnimationKeyframe& rhs) const
{
    return time <= rhs.time;
}