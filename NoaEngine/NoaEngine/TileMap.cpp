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
		if (!layer||!layer->GetActive()) 
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

noa::Vector<float> noa::TileMap::GetTilePosition(int tileID)
{
	noa::Vector<float> result = this->transform.position;
	if (tileID==-1) 
	{
		return result;
	}
	//查找TileID第一次出现的
	
	//tileID的索引坐标
	int posX = 0;
	int posY = 0;

	bool isFind = false;
	for (auto& layer:layerRenderers) 
	{
		if (!layer||!layer->GetActive()) 
		{
			continue;
		}

		const noa::Vector<int>& offset = layer->GetOffset();
		const noa::Vector<int>& size = layer->GetSize();
		int id = -1;
		for (int x = 0;x<size.x&&!isFind;x++)
		{
			for (int y = 0;y<size.y&&!isFind;y++) 
			{
				id = layer->GetTileID(x,y);
				if (id == tileID)
				{
					posX = x + offset.x;
					posY = y + offset.y;
					isFind = true;
					break;
				}
			}
		}

		if (isFind)
		{
			break;
		}

	}

	result.x = static_cast<float>(posX + transform.position.x);
	result.y = static_cast<float>(posY + transform.position.y);
	return result;

	
}
