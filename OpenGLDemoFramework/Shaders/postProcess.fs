#version 330 core
uniform uint time;
in vec2 interpolatedCoords;
in vec2 texCoords;

// Ouput data
layout(location = 0) out vec3 color;

uniform sampler2D sampler;

#ifndef FXAA_REDUCE_MIN
    #define FXAA_REDUCE_MIN   (1.0/ 128.0)
#endif
#ifndef FXAA_REDUCE_MUL
    #define FXAA_REDUCE_MUL   (1.0 / 8.0)
#endif
#ifndef FXAA_SPAN_MAX
    #define FXAA_SPAN_MAX     8.0
#endif

//optimized version for mobile, where dependent 
//texture reads can be a bottleneck
vec4 fxaa(sampler2D tex, vec2 fragCoord, vec2 inverseVP,
            vec2 v_rgbNW, vec2 v_rgbNE, 
            vec2 v_rgbSW, vec2 v_rgbSE, 
            vec2 v_rgbM) {
    vec4 color;
    vec3 rgbNW = texture2D(tex, v_rgbNW).xyz;
    vec3 rgbNE = texture2D(tex, v_rgbNE).xyz;
    vec3 rgbSW = texture2D(tex, v_rgbSW).xyz;
    vec3 rgbSE = texture2D(tex, v_rgbSE).xyz;
    vec4 texColor = texture2D(tex, v_rgbM);
    vec3 rgbM  = texColor.xyz;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    
    mediump vec2 dir;
    dir.x = (lumaSW + lumaSE) - (lumaNW + lumaNE);
    dir.y = (lumaNW + lumaSW) - (lumaNE + lumaSE);
    
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                          (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
              max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
              dir * rcpDirMin)) * inverseVP;
    
    vec3 rgbA = 0.5 * (
        texture2D(tex, fragCoord * inverseVP + dir * (- 1.0 / 6.0)).xyz +
        texture2D(tex, fragCoord * inverseVP + dir * (  1.0 / 6.0)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture2D(tex, fragCoord * inverseVP + dir * -0.5).xyz +
        texture2D(tex, fragCoord * inverseVP + dir * 0.5).xyz);

    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        color = vec4(rgbA, texColor.a);
    else
        color = vec4(rgbB, texColor.a);
    return color;
}

void main()
{
	//color = texture2D(sampler, texCoords).rgb + vec3(0, 0.1, 0);
    
    vec2 resolution = vec2(1024,1024);
    vec2 inverseVP = 1.0 / resolution.xy;  

	vec2 v_rgbNW = (gl_FragCoord.xy + vec2(-1.0, 1.0)) * inverseVP;
	vec2 v_rgbNE = (gl_FragCoord.xy + vec2(1.0, 1.0)) * inverseVP;
	vec2 v_rgbSW = (gl_FragCoord.xy + vec2(-1.0, -1.0)) * inverseVP;
	vec2 v_rgbSE = (gl_FragCoord.xy + vec2(1.0, -1.0)) * inverseVP;
	vec2 v_rgbM = vec2(gl_FragCoord.xy * inverseVP);
    
    color = fxaa(sampler, gl_FragCoord.xy, inverseVP, 
                v_rgbNW,
                v_rgbNE,
                v_rgbSW,
                v_rgbSE,
                v_rgbM).xyz;
}