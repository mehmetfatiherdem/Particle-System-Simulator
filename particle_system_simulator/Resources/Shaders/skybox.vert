#version 440 core

layout (location = 0) in vec3 textureCoords;

out vec3 texCoords;

layout (std140, binding = 0) uniform GlobalMatrices {
    mat4 view;
    mat4 projection;
};

void main()
{
    texCoords = textureCoords;
    vec4 pos = projection * view * vec4(texCoords, 1.0);
    gl_Position = pos.xyww;
}