#version 450

in vec3 position;
in vec3 normal;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
layout (std140) uniform Matrix
{
    mat4 projection;
    mat4 view;
};

out vec3 out_color;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0);
    vec3 n = normalize(normal);
    out_color = Kd;
}