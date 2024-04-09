#version 440 core

in vec3 textureCoords;

out vec3 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, textureCoords).rgb;
}