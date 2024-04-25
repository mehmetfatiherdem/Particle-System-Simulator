#version 440 core

layout (location = 0) in vec3 textureCoords;

out vec3 texCoords;

uniform mat4 view_projection;

void main()
{
    texCoords = textureCoords;
    vec4 pos = view_projection * vec4(texCoords, 1.0);
    gl_Position = pos.xyww;
}