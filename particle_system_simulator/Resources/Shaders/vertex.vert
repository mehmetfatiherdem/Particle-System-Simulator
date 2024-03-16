#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 textureCoordinates;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 textureCoords;

uniform mat4 model;

layout (std140, binding = 0) uniform GlobalMatrices {
    mat4 view;
    mat4 projection;
};

void main() {
    vec4 worldPos = model * vec4(vertexPosition, 1.0);
    gl_Position = projection * view * model * worldPos;

    worldPosition = worldPos.xyz;
    worldNormal = normalize(mat3(transpose(inverse(model))) * vertexNormal); //We need to move this logic outside
    //the shader because the inverse operation is expensive and can be avoided in the CPU
    textureCoords = textureCoordinates;
}
