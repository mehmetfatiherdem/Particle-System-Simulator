#include "ResourceManager.h"

std::map<std::string, Texture> ResourceManager::textures;

const Texture& ResourceManager::addTexture(Texture&& texture, std::string&& resourceName)
{
	textures.emplace(std::move(resourceName), std::move(texture));
	return std::prev(textures.end())->second;
}

const Texture* ResourceManager::getTexture(const std::string& name)
{
	RM_Texture_iter iter = textures.find(name);
	return (iter == textures.end() ? nullptr : &iter->second);
}

std::string ResourceManager::getTextureName(const Texture* texture)
{
	for (RM_Texture_iter iter = texturesBegin(); iter != texturesEnd(); ++iter)
	{
		if (&iter->second == texture)
		{
			return iter->first;
		}
	}

	return "None";
}
