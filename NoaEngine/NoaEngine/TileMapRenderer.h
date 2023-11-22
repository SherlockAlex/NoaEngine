#ifndef NOAENGINE_TILEMAPRENDERER_H
#define NOAENGINE_TILEMAPRENDERER_H

#endif // !NOAENGINE_TILEMAPRENDERER_H

#include "ActorComponent.h"
#include "Sprite.h"

namespace noa 
{
	class TileMapRenderer;
	class TileMapActor;
	class SpriteRenderer;
	class TileLayer;

	class TileMapRenderer :public noa::ActorComponent
	{
	private:
		ACTOR_COMPONENT(TileMapRenderer)
	private:
		TileMapRenderer(noa::Actor* actor);
		~TileMapRenderer();
	public:
		static TileMapRenderer* Create(noa::Actor* actor);

		TileMapRenderer& SetTileMap(
			noa::MapLayer* layer
			,noa::TileSet& tileSet
		);
		TileMapRenderer& SetOffset(float x,float y);
		TileMapRenderer* Apply();
	private:
		noa::Vector<int> tileScale = { 32,32 };
		noa::SpriteRenderer* spriteRenderer = nullptr;
	};

}
