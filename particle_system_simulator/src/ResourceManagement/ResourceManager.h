#pragma once

#include <map>
#include <string>
#include "RenderPipeline/Texture/Texture.h"

using RM_Texture_iter = std::map<std::string, Texture>::iterator;

class ResourceManager
{
private:
	static std::map<std::string, Texture> textures;

	ResourceManager() = delete;
public:

	static const Texture& addTexture(Texture&& texture, std::string&& resourceName);

	static const Texture* getTexture(const std::string& name);
	static RM_Texture_iter texturesBegin();
	static RM_Texture_iter texturesEnd();
};
