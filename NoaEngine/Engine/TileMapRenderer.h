#ifndef NOAENGINE_TILEMAPRENDERER_H
#define NOAENGINE_TILEMAPRENDERER_H

#include "ActorComponent.h"
#include "Sprite.h"
#include "TileMapInfo.h"
#include "Core.h"

#include <unordered_map>

namespace noa
{

	class NOA_API TileMapRenderer;
	class NOA_API SpriteRenderer;
	class NOA_API TileMap;

	class NOA_API TileMapRenderer :public noa::ActorComponent
	{
	private:
		ACTOR_COMPONENT(TileMapRenderer)
	private:
		TileMapRenderer(noa::Actor* actor);
		~TileMapRenderer();
	public:
		static TileMapRenderer* Create(noa::Actor* actor);

		TileMapRenderer& SetCollision(int tileID);
		TileMapRenderer& SetTileMap(
			noa::MapLayer& layer
			, noa::TileSet& tileSet
		);
		TileMapRenderer& SetOffset(int x, int y);
		noa::Vector<int> GetOffset();
		TileMapRenderer& SetActive(bool value);
		TileMapRenderer& SetVisiable(bool value);
		TileMapRenderer* Apply();

		bool CheckCollision(int x, int y);
		int GetTileID(int x,int y);

		noa::Vector<int> GetSize();

	private:
		noa::Vector<int> tileScale = { 32,32 };
		noa::SpriteRenderer* spriteRenderer = nullptr;
		std::unordered_map<int, bool> collisions;
		noa::Vector<int> offset;
		noa::MapLayer layer;
		noa::Vector<int> size;
	};

}


#endif // !NOAENGINE_TILEMAPRENDERER_H