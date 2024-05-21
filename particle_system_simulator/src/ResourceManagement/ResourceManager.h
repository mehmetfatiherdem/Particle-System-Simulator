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

	static void cleanup();
	
	static const Texture* getTexture(const std::string& name);
	static std::string getTextureName(const Texture* texture);
	static uint32_t getTextureCount() { return textures.size(); }
	static RM_Texture_iter texturesBegin() { return textures.begin(); }
	static RM_Texture_iter texturesEnd() { return textures.end(); }
};
