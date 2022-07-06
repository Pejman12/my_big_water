#version 450

in vec3 out_color;
in vec4 clipSpace;
in vec2 texCoord;

layout(location = 0) out vec4 output_color;

uniform sampler2D refraction;
uniform sampler2D reflection;
uniform sampler2D dudv;

uniform float time;

const float waveStrength = 0.01;

void main()
{
    vec2 ndc = (clipSpace.xy / clipSpace.w) * 0.5 + 0.5;
    vec2 uvRefraction = vec2(ndc.x, ndc.y);
    vec2 uvReflection = vec2(ndc.x, -ndc.y);

    vec2 distortion = (texture(dudv, vec2(texCoord.x + time, texCoord.y)).rg * 2.0 - 1.0) * waveStrength;

    uvRefraction += distortion;
    uvRefraction = clamp(uvRefraction, 0.0001, 0.9999);
    uvReflection += distortion;
    uvReflection.x = clamp(uvReflection.x, 0.0001, 0.9999);
    uvReflection.y = clamp(uvReflection.y, -0.9999, -0.0001);

    output_color = mix(texture(reflection, uvReflection), texture(refraction, uvRefraction), 0.5);
}