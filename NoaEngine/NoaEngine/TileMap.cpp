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

bool noa::TileMap::CheckCollision(float x,float y)
{
	bool result = false;
	for (auto& layer:layerRenderers) 
	{
		if (!layer) 
		{
			continue;
		}

		const float posX = x - transform.position.x;
		const float posY = y - transform.position.y;

		bool flag = layer->CheckCollision(posX, posY);
		if (flag)
		{
			result = true;
		}
	}
	return result;
}
