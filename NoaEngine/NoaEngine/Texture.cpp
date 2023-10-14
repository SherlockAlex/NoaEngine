#include "Texture.h"
#include "NoaEngine.h"

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
