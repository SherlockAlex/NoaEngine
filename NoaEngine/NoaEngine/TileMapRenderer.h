#ifndef NOAENGINE_TILEMAPRENDERER_H
#define NOAENGINE_TILEMAPRENDERER_H

#include "ActorComponent.h"
#include "Sprite.h"
#include "TileMapInfo.h"

#include <unordered_map>

namespace noa
{

	class TileMapRenderer;
	class SpriteRenderer;
	class TileMap;

	class TileMapRenderer :public noa::ActorComponent
	{
	private:
		ACTOR_COMPONENT(TileMapRenderer)
	private:
		TileMapRenderer(noa::TileMap* actor);
		~TileMapRenderer();
	public:
		static TileMapRenderer* Create(noa::TileMap* actor);

		TileMapRenderer& SetCollision(int tileID);
		TileMapRenderer& SetTileMap(
			noa::MapLayer& layer
			, noa::TileSet& tileSet
		);
		TileMapRenderer& SetOffset(int x, int y);
		TileMapRenderer* Apply();

		bool CheckCollision(int x, int y);

	private:
		noa::Vector<int> tileScale = { 32,32 };
		noa::SpriteRenderer* spriteRenderer = nullptr;
		std::unordered_map<int, bool> collisions;
		noa::Vector<int> offset;
		noa::MapLayer layer;

	};

}


#endif // !NOAENGINE_TILEMAPRENDERER_H