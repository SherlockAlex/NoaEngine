#include "Camera.h"
#include "NoaEngine.h"

namespace noa {
	Camera::Camera()
	{
	}
	Camera::Camera(Vector<float>* follow)
	{
		this->follow = follow;
	}

	Camera::~Camera()
	{

	}

	TileMapCamera::TileMapCamera()
	{
	}

	TileMapCamera::TileMapCamera(Vector<int> tileScale, Vector<float>* follow):Camera(follow)
	{
		this->tileScale = tileScale;
	}

	Vector<float> TileMapCamera::Render(TileMap& tileMap)
	{
		//完整的渲染2d游戏

		position = *follow;

		//Draw level
		Vector<float> visibleTiles(int(pixelWidth / tileScale.x), int(pixelHeight / tileScale.y));

		//calculate Top-Leftmost visible tile
		Vector<float> offset = move(position - visibleTiles * 0.5);

		//Clamp camera to game boundaries
		if (offset.x < 0)
		{
			offset.x = 0;
		}
		if (offset.y < 0)
		{
			offset.y = 0;
		}
		if (offset.x > tileMap.w - visibleTiles.x)
		{
			offset.x = tileMap.w - visibleTiles.x;
		}
		if (offset.y > tileMap.h - visibleTiles.y)
		{
			offset.y = tileMap.h - visibleTiles.y;
		}

		//Get offsets for smooth movement
		Vector<float> tileOffset;
		tileOffset.x = (offset.x - (int)offset.x) * tileScale.x;
		tileOffset.y = (offset.y - (int)offset.y) * tileScale.y;

		for (int x = -2; x < visibleTiles.x + 2; x++)
		{
			for (int y = -2; y < visibleTiles.y + 2; y++)
			{
				//首先获取tile
				const int tileID = tileMap.GetTileID(x + offset.x, y + offset.y);
				if (tileID == -1)
				{
					renderer.DrawRect(Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y), Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y), BLUE);
					continue;
				}

				//Debug("tileID:" + to_string(tileID));
				Tile* tile = tileMap.GetTile(tileID);
				if (tile == nullptr)
				{
					renderer.DrawRect(Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y), Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y), LIGHTRED);
					continue;
				}
				renderer.DrawRect(Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y), Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y), *tileMap.GetTile(tileID)->sprite);

			}
		}

		return offset;

	}
}


