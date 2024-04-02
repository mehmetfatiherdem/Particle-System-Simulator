#version 440 core

in vec3 texCoords;

out vec3 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, texCoords).rgb;
}