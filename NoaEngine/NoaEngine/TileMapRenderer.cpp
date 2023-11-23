#include "TileMapRenderer.h"
#include "Sprite.h"
#include "Scene.h"
#include "Graphic.h"
#include "SpriteRenderer.h"
#include "TileMapInfo.h"
#include "TileMap.h"

noa::TileMapRenderer::TileMapRenderer(noa::TileMap* actor)
	:noa::ActorComponent(actor)
{
	this->spriteRenderer = noa::SpriteRenderer::Create(actor);
	this->spriteRenderer->SetLayer(InstanceLayer::MAP_LAYER);
	actor->AddTileMapRenderer(this);
}

noa::TileMapRenderer::~TileMapRenderer() {

}

noa::TileMapRenderer* noa::TileMapRenderer::Create(
	noa::TileMap* actor)
{
	return noa::NObject<noa::TileMapRenderer>::Create<noa::TileMap*>(actor);
}

noa::TileMapRenderer& noa::TileMapRenderer::SetCollision(int tileID) {
	this->collisions[tileID] = true;
	return *this;
}

noa::TileMapRenderer& noa::TileMapRenderer::SetTileMap(
	noa::MapLayer& layer
	, noa::TileSet& tileSet
)
{

	this->layer = layer;

	noa::Sprite sprite;
	sprite.w = layer.w*tileScale.x;
	sprite.h = layer.h*tileScale.y;
	sprite.ResizeAndFull(sprite.w,sprite.h,noa::RGBA(255,255,255,0));
	for (int x = 0; x < sprite.w; x++)
	{
		for (int y = 0; y < sprite.h; y++)
		{
			const int tileX = x / tileScale.x;
			const int tileY = y / tileScale.y;
			const int tileID = layer.GetTileID(tileX, tileY);
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
	this->offset.x = x;
	this->offset.y = y;

	this->spriteRenderer->SetOffset(offset.x,offset.y);
	return *this;
}

noa::TileMapRenderer* noa::TileMapRenderer::Apply() 
{
	return this;
}

bool noa::TileMapRenderer::CheckCollision(float x,float y)
{

	const int tileX = static_cast<int>(x - offset.x);
	const int tileY = static_cast<int>(y - offset.y);

	const int tileID = this->layer.GetTileID(tileX, tileY);
	return collisions.count(tileID) > 0;

}

