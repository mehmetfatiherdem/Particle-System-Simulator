#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in mat4 model;

out VS_OUT {
    vec3 worldPosition;
    vec3 worldNormal;
    vec2 texCoords;
} vs_out;

layout (std140, binding = 0) uniform GlobalMatrices {
    mat4 view;
    mat4 projection;
};

void main() {
    vec4 worldPos = model * vec4(vertexPosition, 1.0);
    gl_Position = projection * view * worldPos;

    vs_out.worldPosition = worldPos.xyz;
    vs_out.worldNormal = mat3(transpose(inverse(model))) * vertexNormal;
    vs_out.texCoords = textureCoordinates;
}
