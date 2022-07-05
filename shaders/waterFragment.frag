#version 450

in vec3 out_color;
in vec2 out_uv;

layout(location = 0) out vec4 output_color;

uniform sampler2D refraction;
uniform sampler2D reflection;

void main()
{
    output_color = vec4(out_color, 1.0) * texture(refraction, out_uv);
}