#include "Texture.h"
#include "NoaEngine.h"

noa::Texture::Texture()
{

}

noa::Texture::~Texture()
{
	for (auto & instance:spriteInstances) 
	{
		if (instance.texture == this) 
		{
			instance.texture = nullptr;
		}
	}
}
