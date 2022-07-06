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
    vec3 lightPosition;
};

out vec3 out_color;
out vec4 clipSpace;
out vec2 texCoord;

const float tiling = 6.0;

void main()
{
    clipSpace = projection * view * vec4(position, 1.0);
    gl_Position = clipSpace;
    texCoord = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * tiling;
    vec3 n = normalize(normal);
    vec3 l = normalize(lightPosition - position);
    vec3 r = reflect(l, n);
    float diffuse = max(dot(l, n), 0.0);
    float specular = pow(max(dot(r, l), 0.0), Ns);
    out_color = Ka + Kd * diffuse + Ks * specular;
}