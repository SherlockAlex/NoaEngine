#include "TileMapInfo.h"

noa::MapLayer::MapLayer() {

}

noa::MapLayer::MapLayer(const LayerFile& layer)
{
	this->layer = layer.image;
	this->w = layer.w;
	this->h = layer.h;
}

noa::MapLayer::~MapLayer()
{

}

int noa::MapLayer::GetTileID(int x, int y) const
{
	const int width = static_cast<int>(this->w);
	const int height = static_cast<int>(this->h);
	const int index = y * width + x;
	if (index<0||index>=this->layer.size())
	{
		return -1;
	}
	return this->layer[index];
}

void noa::MapLayer::SetTileID(int x, int y, int value)
{
	const int width = static_cast<int>(this->w);
	const int height = static_cast<int>(this->h);
	const int index = y * width + x;
	if (index < 0 || index >= this->layer.size())
	{
		return;
	}
	this->layer[index] = value;
}



noa::TileMapInfo::TileMapInfo()
{

}

noa::TileMapInfo::~TileMapInfo()
{

}

void noa::TileMapInfo::Construct(
	const std::vector<MapLayer>& layer) 
{
	this->layers = layer;
	//返回地图的最大尺寸
	for (auto& layer : layers)
	{
		if (this->w < layer.w)
		{
			this->w = layer.w;
		}
		if (this->h < layer.h)
		{
			this->h = layer.h;
		}
	}
}

noa::TileMapInfo& noa::TileMapInfo::LoadTileSet(
	const std::string& file)
{
	this->tileSet = Resource::LoadTileSet(file);
	return *this;
}

noa::TileMapInfo& noa::TileMapInfo::LoadTileLayer(
	const std::vector<std::string>& layerFile
)
{
	//读取本地文件
	std::vector<MapLayer> mapLayers;
	for (const auto& file : layerFile)
	{
		MapLayer mapLayer(Resource::LoadMapLayer(file));
		mapLayers.push_back(mapLayer);
	}

	this->Construct(mapLayers);
	return *this;
}

noa::TileMapInfo& noa::TileMapInfo::SetCollisionTileID(
	const std::vector<int>& collisionTileIDs)
{
	//设置Collision Tiles
	const int collisionTilesCount = static_cast<int>(collisionTileIDs.size());
	for (int i = 0; i < collisionTilesCount; i++)
	{
		collisionTiles[collisionTileIDs[i]] = true;
	}
	return *this;
}

noa::TileMapInfo& noa::TileMapInfo::SetCollisionTileID(
	const int tileID)
{
	this->collisionTiles[tileID] = true;
	return *this;
}

noa::TileMapInfo* noa::TileMapInfo::Apply() {
	return this;
}

int noa::TileMapInfo::GetLayerTileID(
	const int layerIndex, const int x, const int y) const
{
	if (layerIndex < 0 || layerIndex >= this->layers.size())
	{
		return -1;
	}
	return layers[layerIndex].GetTileID(x, y);
}

void noa::TileMapInfo::SetLayerTileID(
	const int layerIndex, const int x, const int y, const int tileID)
{
	if (layerIndex < 0 || layerIndex >= this->layers.size())
	{
		return;
	}
	layers[layerIndex].SetTileID(x, y, tileID);
}

bool noa::TileMapInfo::IsTile(const int code) const
{
	return tileSet.ContainTileID(code);
}

bool noa::TileMapInfo::IsCollisionTile(int tileID) const
{
	return collisionTiles.count(tileID) > 0;
}

bool noa::TileMapInfo::IsCollisionTile(const int x, const int y) const
{
	//会去遍历所有的layer
	//只要有一个layer是，那么就是
	bool isCollision = false;
	for (const auto& layer : this->layers)
	{
		if (x < 0 || x >= static_cast<int>(layer.w) || y < 0 || y >= static_cast<int>(layer.h))
		{
			continue;
		}
		isCollision = collisionTiles.count(layer.GetTileID(x, y)) > 0;
		if (isCollision)
		{
			return true;
		}
	}
	return isCollision;

}

noa::TileSet& noa::TileMapInfo::GetTileSet() {
	return *&this->tileSet;
}

noa::Tile* noa::TileMapInfo::GetTile(const int id)
{
	return tileSet.GetTileByID(id);
}