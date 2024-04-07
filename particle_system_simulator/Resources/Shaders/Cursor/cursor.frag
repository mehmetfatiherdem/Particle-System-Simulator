#version 440 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D cursor;

void main()
{    
    color = texture(cursor, texCoords);
}