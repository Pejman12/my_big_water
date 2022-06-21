#version 450

in vec3 position;
in vec3 normalFlat;
in vec3 normalSmooth;

uniform vec3 color;
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

out vec3 out_color;

void main()
{
    gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);
    vec3 normal = normalize(normalSmooth);
    out_color = color * normal;
}