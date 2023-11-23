#ifndef NOAENGINE_TILEMAP_H
#define NOAENGINE_TILEMAP_H

#include <unordered_map>
#include "Actor.h"

namespace noa {
	class TileMapRenderer;

	class TileMap:public noa::Actor
	{
	private:
		TileMap(noa::Scene* scene);
		~TileMap();
	public:
		static TileMap* Create(noa::Scene* scene);
		void AddTileMapRenderer(noa::TileMapRenderer* layerRenderer);
		bool CheckCollision(int x, int y);
	private:
		ACTOR(TileMap)
	private:
		std::vector<noa::TileMapRenderer*> layerRenderers;
	};
}


#endif // !NOAENGINE_TILEMAP_H

