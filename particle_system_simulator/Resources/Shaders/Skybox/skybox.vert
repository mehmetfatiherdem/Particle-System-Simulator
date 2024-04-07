#version 440 core

layout (location = 0) in vec3 vertexPosition;

out vec3 textureCoords;

uniform mat4 view_projection;

void main()
{
    textureCoords = vertexPosition;
    gl_Position = (view_projection * vec4(textureCoords, 1.0)).xyww;
}