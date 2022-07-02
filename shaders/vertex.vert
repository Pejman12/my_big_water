#version 450

in vec3 position;
in vec3 normal;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

out vec3 out_color;

void main()
{
    gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);
    vec3 n = normalize(normal);
    out_color = Kd;
}