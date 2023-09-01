#include "Camera.h"
#include "NoaEngine.h"

namespace noa {

	Camera::Camera()
	{
	}
	Camera::Camera(Behaviour* follow)
	{
		this->follow = follow;
	}

	Camera::~Camera()
	{

	}

	TileMapCamera::TileMapCamera()
	{
	}

	TileMapCamera::TileMapCamera(Vector<int> tileScale, Behaviour* follow):
		Camera(follow)
	{
		this->tileScale = tileScale;
		visibleTiles = Vector<float>(int(pixelWidth / tileScale.x), int(pixelHeight / tileScale.y));
	}

	Vector<float> tileOffset;
	Vector<int> TileMapCamera::Render(TileMap& tileMap, Vector<float>& frontDelta, Vector<float>& endDelta)
	{
		//完整的渲染瓦片地图
		
		position = follow->position;

		//检测相机的边界
		offset = move(position - visibleTiles * 0.5);
		if (offset.x < frontDelta.x) {
			offset.x = frontDelta.x;
		}
		else if (offset.x > tileMap.w - visibleTiles.x + endDelta.x) {
			offset.x = tileMap.w - visibleTiles.x + endDelta.x;
		}

		if (offset.y < frontDelta.y) {
			offset.y = frontDelta.y;
		}
		else if (offset.y > tileMap.h - visibleTiles.y + endDelta.y) {
			offset.y = tileMap.h - visibleTiles.y + endDelta.y;
		}

		//平滑相机窗口的移动
		tileOffset.x = (offset.x - (int)offset.x) * tileScale.x;
		tileOffset.y = (offset.y - (int)offset.y) * tileScale.y;

		for (int x = -2; x < visibleTiles.x+2; x++)
		{
			for (int y = -2; y < visibleTiles.y+2 ; y++)
			{
				//首先获取tile
				const int tileID = tileMap.GetTileID(x + offset.x, y + offset.y);
				if (tileID == -1)
				{
					renderer.DrawRect(
						Vector<int>((x- 0) * tileScale.x - tileOffset.x, (y- 0)*tileScale.y - tileOffset.y),
						Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y),
						BLUE
					);
					continue;
				}

				//Debug("tileID:" + to_string(tileID));
				Tile* tile = tileMap.GetTile(tileID);
				if (tile == nullptr)
				{
					renderer.DrawRect(
						Vector<int>((x- 0) * tileScale.x - tileOffset.x, (y- 0) * tileScale.y - tileOffset.y),
						Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y),
						LIGHTRED
					);
					continue;
				}
				renderer.DrawRect(
					Vector<int>((x- 0) * tileScale.x - tileOffset.x, (y- 0) * tileScale.y - tileOffset.y),
					Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y),
					*tileMap.GetTile(tileID)->sprite
				);

			}
		}


		followPositionOnScreen = move(Vector<int>((follow->position.x - offset.x) * tileScale.x, (follow->position.y - offset.y) * tileScale.y));
		return followPositionOnScreen;

	}

	extern unordered_map<int, bool> collisionTiles;
	extern bool IsCollisionTile(int tileID);
	extern vector<GameObject*> gameObjects;
	static vector<float> wallDistanceBuffer;

	FreeCamera::FreeCamera()
	{
		wallDistanceBuffer = vector<float>(pixelWidth, 0.0);
	}
	FreeCamera::FreeCamera(Behaviour* follow):Camera(follow)
	{
		wallDistanceBuffer = vector<float>(pixelWidth, 0.0);
	}

	void FreeCamera::Render(TileMap& map)
	{
		for (int x = 0;x<pixelWidth;x++) 
		{
			Ray ray = RaycastHit(x, map);
			
			wallDistanceBuffer[x] = ray.distance;

			const float ceiling = pixelHeight * 0.5 - (float)(pixelHeight) / ray.distance;
			const float floor = pixelHeight - ceiling;

			uint32_t color = BLACK;

			const float shadowOfWall = 1 / (1 +
				ray.distance * ray.distance * ray.distance * ray.distance * ray.distance * 0.00002);

			float sharkCamera = 75 * (sinf(1.5 * (follow->position.x)) + sinhf(1.5 * (follow->position.y)));
			sharkCamera = sharkCamera / ray.distance;

			sharkCamera = 0;

			for (int y = 0;y<pixelHeight;y++)
			{
				if (y<=ceiling + sharkCamera)
				{
					color = RGB(63, 63, 63);
				}
				else if (y>ceiling + sharkCamera && y<=floor + sharkCamera)
				{
					ray.simple.y = ((float)y - (float)(ceiling + sharkCamera))
						/ ((float)floor - (float)ceiling);

					Tile* tile = map.GetTile(ray.hitTile);
					if (tile!=nullptr) 
					{
						color = tile->sprite->GetColor(ray.simple.y, ray.simple.x);
					}
				}
				else {
					const float b = 1.0 - (((float)y - pixelHeight * 0.5) / (float)pixelHeight * 0.5);
					const float deltaRayShine = (1 - b) * (1 - b);
					const float depth = (1 - b);
					color = WHITE;
				}

				renderer.DrawPixel(x, y, color);

			}

		}

		//绘制物品
		for (auto object : gameObjects)
		{

			const float fVecX = object->position.x - follow->position.x;
			const float fVecY = object->position.y - follow->position.y;

			const float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

			const float fEyeX = sinf(follow->angle);
			const float fEyeY = cosf(follow->angle);
			float fObjectAngle = atan2(fEyeY, fEyeX) - atan2(fVecY, fVecX);
			if (fObjectAngle < -3.14159f)
			{
				fObjectAngle += 2.0f * 3.14159f;
			}
			if (fObjectAngle > 3.14159)
			{
				fObjectAngle -= 2.0f * 3.14159f;
			}
			const bool bInPlayerFOV = fabs(fObjectAngle) < FOV * 0.5f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f &&
				fDistanceFromPlayer < viewDepth)
			{

				//绘制物体到屏幕上

				const float fObjectCeiling = (float)(pixelHeight * 0.5)
					- pixelHeight / (float)fDistanceFromPlayer;
				const float fObjectFloor = pixelHeight - fObjectCeiling;

				const float fObjectHeight = fObjectFloor - fObjectCeiling;
				const float fObjectAspectRatio = (float)object->sprite->h / (float)object->sprite->w;
				const float fObjectWidth = fObjectHeight / fObjectAspectRatio;

				const float fMiddleOfObject = (0.5f * (fObjectAngle / (FOV * 0.5f)) + 0.5f)
					* (float)pixelWidth;

				for (float lx = 0; lx < fObjectWidth; lx++)
				{
					for (float ly = 0; ly < fObjectHeight; ly++)
					{
						const float objSimpleX = lx / fObjectWidth;
						const float objSimpleY = ly / fObjectHeight;

						const Uint32 objColor = object->sprite->GetTransposeColor(objSimpleY, objSimpleX);
						const int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth * 0.5f));
						if (nObjectColumn >= 0 && nObjectColumn < pixelWidth)
						{
							if ((int)(fObjectCeiling + ly) < 0 || (int)(fObjectCeiling + ly) >= pixelHeight
								|| (nObjectColumn < 0) || (nObjectColumn >= pixelWidth)) {
								continue;
							}
							if (objColor == BLACK || wallDistanceBuffer[nObjectColumn] < fDistanceFromPlayer)
							{
								continue;
							}
							wallDistanceBuffer[nObjectColumn] = fDistanceFromPlayer;
							renderer.DrawPixel(nObjectColumn, (int)(fObjectCeiling + ly), objColor);
						}
					}
				}
			}

		}

	}

	Ray FreeCamera::RaycastHit(int pixelX, TileMap& map)
	{
		Ray ray;
		ray.distance = 0.0;
		ray.angle = follow->angle - FOV * (0.5 - (float)pixelX / pixelWidth);
		const float rayForwordStep = 0.03;
		Vector<float> eye = Vector<float>(sinf(ray.angle), cosf(ray.angle));

		while (!IsCollisionTile(ray.hitTile)&&ray.distance<viewDepth) 
		{
			//直到射线集中障碍物
			ray.distance += rayForwordStep;

			const Vector<float> floatHitPoint = follow->position + Vector<float>(0.5,0.5) + eye * ray.distance;
			const Vector<int> intHitPoint = Vector<int>(floatHitPoint.x, floatHitPoint.y);

			if (intHitPoint.x < 0||intHitPoint.x >= map.w || intHitPoint.y < 0|| intHitPoint.y >= map.h)
			{
				ray.hitTile = -1;
				ray.distance = viewDepth;
				continue;
			}

			ray.hitTile = map.GetTileID(intHitPoint.x, intHitPoint.y);

			if (IsCollisionTile(ray.hitTile)) 
			{
				const float fBlockMidX = (float)intHitPoint.x + 0.5f;
				const float fBlockMidY = (float)intHitPoint.y + 0.5f;
				const float fTestAngle = atan2f((floatHitPoint.y - fBlockMidY), (floatHitPoint.x - fBlockMidX));

				if (fTestAngle >= -PI * 0.25f && fTestAngle < PI * 0.25f)
				{
					ray.simple.x = floatHitPoint.y - (float)intHitPoint.y;

				}
				if (fTestAngle >= PI * 0.25f && fTestAngle < PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.x - (float)intHitPoint.x;
				}
				if (fTestAngle < -PI * 0.25f && fTestAngle >= -PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.x - (float)intHitPoint.x;
				}
				if (fTestAngle >= PI * 0.75f || fTestAngle < -PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.y - (float)intHitPoint.y;
				}
			}

		}

		ray.distance = ray.distance * cosf(follow->angle - ray.angle);

		return ray;
	}
}


