#version 450

in vec3 position;
in vec3 normal;

layout (std140) uniform Matrix
{
    mat4 projection;
    mat4 view;
    vec3 lightPosition;
};
uniform vec3 camPos;

out vec3 out_color;
out vec4 clipSpace;
out vec2 texCoord;
out vec3 toCameraVector;
out vec3 fromLightVector;

const float tiling = 0.3;

void main()
{
    clipSpace = projection * view * vec4(position, 1.0);
    gl_Position = clipSpace;
    texCoord = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * tiling;
    toCameraVector = camPos - position;
    fromLightVector = position - lightPosition;
}