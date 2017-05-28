#version 330 core

uniform sampler2D colorMap;
uniform sampler2D shadowMapPointSampler;
uniform sampler2DShadow shadowMapSampler;

uniform float time;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 cameraPos;
uniform mat4 depthModelView;
uniform float lightSize;

// Input data
smooth in vec3 inNormal;
smooth in vec4 shadowCoord;
smooth in vec2 inUVs;
smooth in vec4 lightViewPos;

// Ouput data
layout(location = 0) out vec3 outColor;

#define BLOCKER_SEARCH_NUM_SAMPLES 32
#define PCF_NUM_SAMPLES            32
#define NEAR_PLANE                 20.0
#define FAR_PLANE                  50.0

const vec2 poissonDisk[32] = vec2[](
    vec2(-0.975402, -0.0711386),
    vec2(-0.920347, -0.41142),
    vec2(-0.883908, 0.217872),
    vec2(-0.884518, 0.568041),
    vec2(-0.811945, 0.90521),
    vec2(-0.792474, -0.779962),
    vec2(-0.614856, 0.386578),
    vec2(-0.580859, -0.208777),
    vec2(-0.53795, 0.716666),
    vec2(-0.515427, 0.0899991),
    vec2(-0.454634, -0.707938),
    vec2(-0.420942, 0.991272),
    vec2(-0.261147, 0.588488),
    vec2(-0.211219, 0.114841),
    vec2(-0.146336, -0.259194),
    vec2(-0.139439, -0.888668),
    vec2(0.0116886, 0.326395),
    vec2(0.0380566, 0.625477),
    vec2(0.0625935, -0.50853),
    vec2(0.125584, 0.0469069),
    vec2(0.169469, -0.997253),
    vec2(0.320597, 0.291055),
    vec2(0.359172, -0.633717),
    vec2(0.435713, -0.250832),
    vec2(0.507797, -0.916562),
    vec2(0.545763, 0.730216),
    vec2(0.56859, 0.11655),
    vec2(0.743156, -0.505173),
    vec2(0.736442, -0.189734),
    vec2(0.843562, 0.357036),
    vec2(0.865413, 0.763726),
    vec2(0.872005, -0.927)
);

float borderDepthTexture(sampler2D tex, vec2 uv)
{
	return ((uv.x <= 1.0) && (uv.y <= 1.0) &&
	 (uv.x >= 0.0) && (uv.y >= 0.0)) ? texture(tex, uv).z : 1.0;
}

float myPCF(sampler2D tex, vec3 uvz)
{
    float sum = 0.0;
    
    vec2 offsets[4];
    float delta = 1.0 / 1024.0;
    offsets[0] = vec2(delta, 0.0);
    offsets[1] = vec2(-delta, 0.0);
    offsets[2] = vec2(0.0, delta);
    offsets[3] = vec2(0.0, -delta);
    
    for (int i = 0; i < 4; ++i)
    {
        sum += float(texture(tex, uvz.xy + offsets[i]).z > uvz.z);
    }
    
    return sum / 4.0;
} 
float borderPCFTexture(sampler2DShadow tex, vec3 uvz)
{
	return ((uvz.x <= 1.0) && (uvz.y <= 1.0) &&
	 (uvz.x >= 0.0) && (uvz.y >= 0.0)) ? texture(tex, uvz, 0.1) : 
	 ((uvz.z <= 1.0) ? 1.0 : 0.0);
}

// Using similar triangles from the surface point to the area light
vec2 searchRegionRadiusUV(float zWorld)
{
    return vec2(lightSize * (zWorld - NEAR_PLANE) / zWorld);
}

// Using similar triangles between the area light, the blocking plane and the surface point
vec2 penumbraRadiusUV(float zReceiver, float zBlocker)
{
    return vec2(lightSize * (zReceiver - zBlocker) / zBlocker);
}

// Project UV size to the near plane of the light
vec2 projectToLightUV(vec2 sizeUV, float zWorld)
{
    return vec2(sizeUV * NEAR_PLANE / zWorld);
}

// Derivatives of light-space depth with respect to texture2D coordinates
vec2 depthGradient(vec2 uv, float z)
{
    vec2 dz_duv = vec2(0.0, 0.0);

    vec3 duvdist_dx = dFdx(vec3(uv,z));
    vec3 duvdist_dy = dFdy(vec3(uv,z));

    dz_duv.x = duvdist_dy.y * duvdist_dx.z;
    dz_duv.x -= duvdist_dx.y * duvdist_dy.z;

    dz_duv.y = duvdist_dx.x * duvdist_dy.z;
    dz_duv.y -= duvdist_dy.x * duvdist_dx.z;

    float det = (duvdist_dx.x * duvdist_dy.y) - (duvdist_dx.y * duvdist_dy.x);
    dz_duv /= det;

    return dz_duv;
}

float biasedZ(float z0, vec2 dz_duv, vec2 offset)
{
    return z0 + dot(dz_duv, offset);
}

float zClipToEye(float z)
{
    return FAR_PLANE * NEAR_PLANE / (FAR_PLANE - z * (FAR_PLANE - NEAR_PLANE));   
}

// Returns average blocker depth in the search region, as well as the number of found blockers.
// Blockers are defined as shadow-map samples between the surface point and the light.
void findBlocker(
    out float accumBlockerDepth, 
    out float numBlockers,
    vec2 uv,
    float z0,
    vec2 dz_duv,
    vec2 searchRegionRadiusUV)
{
    accumBlockerDepth = 0.0;
    numBlockers       = 0.0;
    for (int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; ++i)
    {
        vec2 offset = poissonDisk[i] * searchRegionRadiusUV;
        float shadowMapDepth = borderDepthTexture(shadowMapPointSampler, uv + offset);
        float z = biasedZ(z0, dz_duv, offset);
        if (shadowMapDepth < z)
        {
            accumBlockerDepth += shadowMapDepth;
            numBlockers += 1.0;
        }
    }
}

// Performs PCF filtering on the shadow map using multiple taps in the filter region.
float pcfFilter(vec2 uv, float z0, vec2 dz_duv, vec2 filterRadiusUV)
{
    float sum = 0.0;
    for (int i = 0; i < PCF_NUM_SAMPLES; ++i)
    {
        vec2 offset = poissonDisk[i] * filterRadiusUV;
        float z = biasedZ(z0, dz_duv, offset);
        //sum += borderPCFTexture(shadowMapSampler, vec3(uv + offset, z));
        sum += myPCF(shadowMapPointSampler, vec3(uv + offset, z));
    }
    return sum / float(PCF_NUM_SAMPLES);
}

float pcssShadow(vec2 uv, float z, vec2 dz_duv, float zEye, inout vec3 dbg)
{
    // ------------------------
    // STEP 1: blocker search
    // ------------------------
    float accumBlockerDepth, numBlockers;
    vec2 searchRegionRadiusUV = searchRegionRadiusUV(zEye);
    findBlocker(accumBlockerDepth, numBlockers, uv, z, dz_duv, searchRegionRadiusUV);

    // Early out if not in the penumbra
    if (numBlockers == 0.0)
        return 1.0;

    // ------------------------
    // STEP 2: penumbra size
    // ------------------------
    float avgBlockerDepth      = accumBlockerDepth / numBlockers;
    float avgBlockerDepthWorld = zClipToEye(avgBlockerDepth);
    vec2  penumbraRadius       = penumbraRadiusUV(zEye, avgBlockerDepthWorld);
    vec2  filterRadius         = projectToLightUV(penumbraRadius, zEye);
    
    // DBG
    dbg = vec3(avgBlockerDepth);
    
    // ------------------------
    // STEP 3: filtering
    // ------------------------
    return pcfFilter(uv, z, dz_duv, filterRadius) * 0.5 + 0.5;
}
void main()
{
    vec3 light    = normalize(vec3(0, 0.15, 0.25));
    vec3 n        = normalize(inNormal);
    float NoL     = max(0.0, dot(n, light));
    vec3 texColor = texture(colorMap, inUVs + vec2(time * 0.01, sin(time * 0.01))).xyz;

    vec2 uv       = (shadowCoord.xy / shadowCoord.w) * 0.5 + 0.5;
    float z       = (shadowCoord.z  / shadowCoord.w) * 0.5 + 0.5;
    vec2 dz_duv   = depthGradient(uv, z);
    float zEye    = -lightViewPos.z;
    
    vec3 dbg = vec3(1.0, 0.0, 0.0);
    outColor = diffuse * NoL * pcssShadow(uv, z, dz_duv, zEye, dbg);
    //outColor = dbg;
}