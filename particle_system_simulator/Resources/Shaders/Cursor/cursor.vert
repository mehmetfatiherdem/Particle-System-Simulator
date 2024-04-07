#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoords;

uniform vec2 position;
uniform float size;

out vec2 texCoords;

void main()
{
    texCoords = textureCoords;
    gl_Position = vec4(position + vertexPosition.xy * size, 1.0, 1.0);
}