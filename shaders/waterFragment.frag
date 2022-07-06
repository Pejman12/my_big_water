#version 450

in vec3 out_color;
in vec4 clipSpace;

layout(location = 0) out vec4 output_color;

uniform sampler2D refraction;
uniform sampler2D reflection;

void main()
{
    vec2 ndc = (clipSpace.xy / clipSpace.w) * 0.5 + 0.5;
    vec2 uvRefraction = vec2(ndc.x, ndc.y);
    vec2 uvReflection = vec2(ndc.x, -ndc.y);

    output_color = mix(texture(reflection, uvReflection), texture(refraction, uvRefraction), 0.5);
}