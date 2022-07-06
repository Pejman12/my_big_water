#version 450

in vec3 out_color;
in vec4 clipSpace;
in vec2 texCoord;
in vec3 toCameraVector;
in vec3 fromLightVector;

layout(location = 0) out vec4 output_color;

uniform sampler2D refraction;
uniform sampler2D reflection;
uniform sampler2D dudv;
uniform sampler2D depth;

uniform float time;

const float waveStrength = 0.01;
const float reflectivity = 0.6;
const float Ns = 20.0;

void main()
{
    vec2 ndc = (clipSpace.xy / clipSpace.w) * 0.5 + 0.5;
    vec2 uvRefraction = vec2(ndc.x, ndc.y);
    vec2 uvReflection = vec2(ndc.x, -ndc.y);

    float near = 0.1;
    float far = 200.0;
    float depthValue = texture(depth, uvRefraction).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depthValue - 1.0) * (far - near));

    depthValue = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depthValue - 1.0) * (far - near));
    float waterHeight = floorDistance - waterDistance;

    vec2 distortion = texture(dudv, vec2(texCoord.x + time, texCoord.y)).rg * 0.1;
    distortion = texCoord + vec2(distortion.x, distortion.y + time);
    vec2 totalDistortion = (texture(dudv, distortion).rg * 2.0 - 1.0) * waveStrength * clamp(waterHeight/3.0, 0.0, 1.0);

    uvRefraction += totalDistortion;
    uvRefraction = clamp(uvRefraction, 0.0001, 0.9999);
    uvReflection += totalDistortion;
    uvReflection.x = clamp(uvReflection.x, 0.0001, 0.9999);
    uvReflection.y = clamp(uvReflection.y, -0.9999, -0.0001);

    vec4 normalMapColour = texture(dudv, distortion);
    vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, normalMapColour.b * 3.0, normalMapColour.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 3.0);

    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    float specularFactor = max(dot(viewVector, reflectedLight), 0.0);
    specularFactor = pow(specularFactor, Ns);
    vec3 specularColor = reflectivity * specularFactor * vec3(1.0, 1.0, 1.0) * clamp(waterHeight * 1.2, 0.0, 1.0);

    output_color = mix(texture(reflection, uvReflection), texture(refraction, uvRefraction), refractiveFactor);
    output_color = mix(output_color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularColor, 0.0);
    output_color.a = clamp(waterHeight * 1.2, 0.0, 1.0);
}