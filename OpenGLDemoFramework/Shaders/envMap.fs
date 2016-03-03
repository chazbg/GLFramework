#version 330 core

uniform samplerCube envMap;

// Input data
smooth in vec3 iPos;

// Ouput data
layout(location = 0) out vec3 outColor;

vec3 linearToSRGB(vec3 linearRGB) 
{
    const vec3 INVERSE_GAMMA = vec3(1.0 / 2.4);
    const vec3 K0 = vec3(0.0031308);
    const float ALPHA = 0.055;
    const float PHI = 12.92;
    
    bvec3 lessThanK0 = lessThanEqual(linearRGB, K0);
    
    vec3 sRGBLessThanK0 = PHI * linearRGB;
    vec3 sRGBLargerThanK0 = (1.0 + ALPHA) * pow(linearRGB, vec3(INVERSE_GAMMA)) - ALPHA;
    
    vec3 sRGB = mix(sRGBLargerThanK0, sRGBLessThanK0, vec3(lessThanK0));
    return sRGB;
}

void main()
{
    outColor = linearToSRGB(texture(envMap, iPos).bgr);
}