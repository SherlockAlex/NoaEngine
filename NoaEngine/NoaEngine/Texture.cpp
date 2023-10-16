#include "Texture.h"
#include "Sprite.h"
#include <vector>

extern std::vector<std::vector<noa::SpriteGPUInstance>> rendererInstanceLayer;

noa::Texture::Texture()
{

}

noa::Texture::~Texture()
{
	for (auto & layer:rendererInstanceLayer) 
	{
		for (auto& instance : layer)
		{
			if (instance.texture == this)
			{
				instance.texture = nullptr;
			}
		}
	}
	
}
