#ifndef NOAENGINE_TILEMAPINFO_H
#define NOAENGINE_TILEMAPINFO_H

#include <stack>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>
#include <functional>
#include <list>
#include <map>

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Resource.h"
#include "Core.h"

namespace noa {

	//µØÍ¼µÄ²ã¼¶
	class NOA_API MapLayer
	{
	public:
		std::vector<int> layer;
		noa::Vector<float> position;
		uint32_t w = 0;
		uint32_t h = 0;
	public:
		MapLayer();
		MapLayer(const LayerFile& layer);
		virtual ~MapLayer();

	public:
		int GetTileID(int x, int y) const;
		void SetTileID(int x, int y, int value);

	};

	class NOA_API TileMapInfo
	{
	private:
		void Construct(const std::vector<MapLayer>& layer);
	public:
		TileMapInfo();
		virtual ~TileMapInfo();

		TileMapInfo& LoadTileSet(const std::string& file);
		TileMapInfo& LoadTileLayer(const std::vector<std::string>& file);
		TileMapInfo& SetCollisionTileID(const std::vector<int>& collisionTileIDs);
		TileMapInfo& SetCollisionTileID(const int tileID);

		TileMapInfo* Apply();

		int GetLayerTileID(const int layerIndex, const int x, const int y) const;
		void SetLayerTileID(const int layerIndex, const int x, const int y, const int tileID);
		Tile* GetTile(const int id);
		bool IsTile(const int code) const;
		bool IsCollisionTile(int tileID) const;
		bool IsCollisionTile(const int x, const int y) const;
		TileSet& GetTileSet();

	public:
		TileSet tileSet;
		std::unordered_map<int, bool> collisionTiles;
		std::vector<MapLayer> layers;
		uint32_t w = 0;
		uint32_t h = 0;
	};
}

#endif // !NOAENGINE_TILEMAPINFO_H



