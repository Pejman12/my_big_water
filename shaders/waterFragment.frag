#version 450

in vec3 out_color;
in vec4 clipSpace;
in vec2 texCoord;
in vec3 toCameraVector;

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

    vec2 distortion1 = (texture(dudv, vec2(texCoord.x + time, texCoord.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion2 = (texture(dudv, vec2(-texCoord.x + time, texCoord.y + time)).rg * 2.0 - 1.0) * waveStrength;
    vec2 totalDistortion = distortion1 + distortion2;

    uvRefraction += totalDistortion;
    uvRefraction = clamp(uvRefraction, 0.0001, 0.9999);
    uvReflection += totalDistortion;
    uvReflection.x = clamp(uvReflection.x, 0.0001, 0.9999);
    uvReflection.y = clamp(uvReflection.y, -0.9999, -0.0001);

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
    refractiveFactor = pow(refractiveFactor, 3.0);

    output_color = mix(texture(reflection, uvReflection), texture(refraction, uvRefraction), refractiveFactor);
    output_color = mix(output_color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}