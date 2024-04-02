#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 textureCoordinates;

out VS_OUT {
    vec3 worldPosition;
    vec3 worldNormal;
    vec2 texCoords;
} vs_out;

uniform mat4 model;

layout (std140, binding = 0) uniform GlobalMatrices {
    mat4 view;
    mat4 projection;
};

void main() {
    vec4 worldPos = model * vec4(vertexPosition, 1.0);
    gl_Position = projection * view * worldPos;

    vs_out.worldPosition = worldPos.xyz;
    vs_out.worldNormal = (transpose(inverse(model)) * vec4(vertexNormal, 0.0)).xyz;
    vs_out.texCoords = textureCoordinates;
}
