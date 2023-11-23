#include "TileMap.h"
#include "TileMapRenderer.h"

noa::TileMap::TileMap(noa::Scene* scene)
	:noa::Actor(scene)
{

}

noa::TileMap::~TileMap() {

}

noa::TileMap* noa::TileMap::Create(noa::Scene* scene)
{
	return noa::NObject<noa::TileMap>::Create(scene);
}

void noa::TileMap::AddTileMapRenderer(noa::TileMapRenderer* layerRenderer)
{
	if (layerRenderer == nullptr) 
	{
		return;
	}
	this->layerRenderers.push_back(layerRenderer);
}

bool noa::TileMap::CheckCollision(int x, int y)
{
	for (auto& layer:layerRenderers) 
	{
		if (!layer) 
		{
			continue;
		}

		const int posX = static_cast<int>(x - transform.position.x);
		const int posY = static_cast<int>(y - transform.position.y);
		const bool flag = layer->CheckCollision(posX, posY);
		if (flag)
		{
			return true;
		}
	}
	return false;
}
