#include "TileMapRenderer.h"
#include "Sprite.h"
#include "Scene.h"
#include "Graphic.h"
#include "SpriteRenderer.h"

noa::TileMapRenderer::TileMapRenderer(noa::Actor* actor)
	:noa::ActorComponent(actor)
{
	this->spriteRenderer = noa::SpriteRenderer::Create(actor);
	this->spriteRenderer->SetLayer(InstanceLayer::MAP_LAYER);
}

noa::TileMapRenderer::~TileMapRenderer() {

}

noa::TileMapRenderer* noa::TileMapRenderer::Create(
	noa::Actor* actor) 
{
	return noa::NObject<noa::TileMapRenderer>::Create(actor);
}

noa::TileMapRenderer& noa::TileMapRenderer::SetTileMap(
	noa::MapLayer* layer
	, noa::TileSet& tileSet
)
{
	if (layer == nullptr||spriteRenderer == nullptr)
	{
		return *this;
	}
	noa::Sprite sprite;
	sprite.w = layer->w*tileScale.x;
	sprite.h = layer->h*tileScale.y;
	sprite.ResizeAndFull(sprite.w,sprite.h,noa::RGBA(255,255,255,0));
	for (int x = 0; x < sprite.w; x++)
	{
		for (int y = 0; y < sprite.h; y++)
		{
			const int tileX = x / tileScale.x;
			const int tileY = y / tileScale.y;
			const int tileID = layer->GetTileID(tileX, tileY);
			Tile* tile = tileSet.GetTileByID(tileID);
			if (tile == nullptr)
			{
				continue;
			}

			const float simpleX =
				static_cast<float>(x % tileScale.x) / tileScale.x;
			const float simpleY =
				static_cast<float>(y % tileScale.y) / tileScale.y;

			const uint32_t color = tile->sprite->GetColor(simpleX, simpleY);
			sprite.SetPixelColor(x, y, color);
		}
	}

	spriteRenderer->SetSprite(&sprite);
	return *this;
}

noa::TileMapRenderer& noa::TileMapRenderer::SetOffset(
	float x,float y) 
{
	this->spriteRenderer->SetOffset(x,y);
	return *this;
}

noa::TileMapRenderer* noa::TileMapRenderer::Apply() 
{
	return this;
}

