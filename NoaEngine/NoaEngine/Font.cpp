#include "Font.h"
#include "Sprite.h"
#include "Resource.h"

noa::Font::Font() {

}

noa::Font::Font(const noa::SpriteFile& spriteFile)
{
	this->w = spriteFile.width;
	this->h = spriteFile.height;
	spriteGPU = SpriteGPU::Create(spriteFile, 64, 64);
	spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
}