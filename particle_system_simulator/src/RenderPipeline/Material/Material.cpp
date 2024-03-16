#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "Material.h"

Material::Material(const Shader* shader, Texture* diffuseMap, Texture* specularMap, const Color4& color,
    float ambientStrength, float shininess) : shader(shader), diffuseMap(diffuseMap), specularMap(specularMap), color(color),
    ambientStrength(ambientStrength), shininess(shininess) 
{
    assert(shader != nullptr && "Shader can't be null!");

    if(diffuseMap)
        ((Texture*)diffuseMap)->setTextureUnit(0);
    if(specularMap)
        ((Texture*)specularMap)->setTextureUnit(1);
}

void Material::useMaterial(const glm::mat4& model) const
{
    shader->setMatrix4("model", model);
    shader->setVector("material.color.ambient", color.ambient);
    shader->setVector("material.color.diffuse", color.diffuse);
    shader->setVector("material.color.specular", color.specular);
    shader->setFloat("material.ambientStrength", ambientStrength);
    shader->setFloat("material.shininess", shininess);

    shader->setBool("material.useDiffuseMap", diffuseMap);
    shader->setBool("material.useSpecularMap", specularMap);

    if(diffuseMap)
    {
        diffuseMap->useTexture();
        shader->setInt("material.diffuseMap", 0);
    }
    if(specularMap)
    {
        specularMap->useTexture();
        shader->setInt("material.specularMap", 1);
    }

    shader->useShader();
}