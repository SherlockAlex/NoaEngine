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
			Ray ray =move(RaycastHit(x, map));
			
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

					const Tile* tile = map.GetTile(ray.hitTile);
					if (tile!=nullptr) 
					{
						color = tile->sprite->GetColor(ray.simple.y, ray.simple.x);
					}
				}
				else 
				{
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

			const Vector<float> vecToFollow = move(object->position - follow->position);

			const float distanceFromPlayer = vecToFollow.Magnitude();

			const Vector<float> eye = move(Vector<float>(sinf(follow->angle), cosf(follow->angle)));

			float objectAngle = atan2(eye.y, eye.x) - atan2(vecToFollow.y, vecToFollow.x);
			if (objectAngle < -3.14159f)
			{
				objectAngle += 2.0f * 3.14159f;
			}
			if (objectAngle > 3.14159)
			{
				objectAngle -= 2.0f * 3.14159f;
			}
			const bool isInPlayerFOV = fabs(objectAngle) < FOV * 0.5f;

			if (isInPlayerFOV && distanceFromPlayer >= 0.5f &&
				distanceFromPlayer < viewDepth)
			{

				//绘制物体到屏幕上

				const float objectCeiling = (float)(pixelHeight * 0.5)
					- pixelHeight / (float)distanceFromPlayer;
				const float objectFloor = pixelHeight - objectCeiling;

				const float objectHeight = objectFloor - objectCeiling;
				const float objectAspectRatio = (float)object->sprite->h / (float)object->sprite->w;
				const float objectWidth = objectHeight / objectAspectRatio;

				const float middleOfObject = (0.5f * (objectAngle / (FOV * 0.5f)) + 0.5f)
					* (float)pixelWidth;

				for (float lx = 0; lx < objectWidth; lx++)
				{
					for (float ly = 0; ly < objectHeight; ly++)
					{
						const float objSimpleX = lx / objectWidth;
						const float objSimpleY = ly / objectHeight;

						const Uint32 objColor = object->sprite->GetTransposeColor(objSimpleY, objSimpleX);
						const int objectColumn = (int)(middleOfObject + lx - (objectWidth * 0.5f));
						if (objectColumn >= 0 && objectColumn < pixelWidth)
						{
							if ((int)(objectCeiling + ly) < 0 || (int)(objectCeiling + ly) >= pixelHeight
								|| (objectColumn < 0) || (objectColumn >= pixelWidth)) {
								continue;
							}
							if (objColor == BLACK || wallDistanceBuffer[objectColumn] < distanceFromPlayer)
							{
								continue;
							}
							wallDistanceBuffer[objectColumn] = distanceFromPlayer;
							renderer.DrawPixel(objectColumn, (int)(objectCeiling + ly), objColor);
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
		const Vector<float> eye = Vector<float>(sinf(ray.angle), cosf(ray.angle));

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
				const float blockMidX = (float)intHitPoint.x + 0.5f;
				const float blockMidY = (float)intHitPoint.y + 0.5f;

				const float testAngle = atan2f((floatHitPoint.y - blockMidY), (floatHitPoint.x - blockMidX));

				if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
				{
					ray.simple.x = floatHitPoint.y - (float)intHitPoint.y;

				}
				if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.x - (float)intHitPoint.x;
				}
				if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.x - (float)intHitPoint.x;
				}
				if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.y - (float)intHitPoint.y;
				}
			}

		}

		ray.distance = ray.distance * cosf(follow->angle - ray.angle);

		return ray;
	}
}


