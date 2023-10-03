#include "Camera.h"
#include "NoaEngine.h"
#include <thread>
#include "Graphic.h"
#include "SpriteRenderer.h"

/*
* 绘制Sprite的新思路
* 使用SpriteGPU进行绘制
* 但是SpriteGPU的数量又不能太多
*/

namespace noa {

	Camera::Camera()
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::SetFollow(Transform* follow)
	{
		this->follow = follow;
	}


	TileMapCamera::TileMapCamera()
	{
	}

	void TileMapCamera::SetTileScale(Vector<int> tileScale)
	{
		this->tileScale = tileScale;
		visibleTiles = Vector<float>(static_cast<float>(int(pixelWidth / tileScale.x)), static_cast<float>(int(pixelHeight / tileScale.y)));
	}

	Vector<float> tileOffset;
	Vector<int> TileMapCamera::Render(TileMap& tileMap,const Vector<float>& frontDelta,const Vector<float>& endDelta)
	{
		//完整的渲染瓦片地图
		if (follow == nullptr)
		{
			return {0,0};
		}
		for (auto& object:objectBufferWithRay) 
		{
			object = nullptr;
		}

		position = follow->position;

		//检测相机的边界
		offset = std::move(position - visibleTiles * 0.5);
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
				const int tileID = tileMap.GetTileID(static_cast<int>(x + offset.x), static_cast<int>(y + offset.y));
				if (tileID == -1)
				{
					renderer->DrawRect(
						Vector<int>(static_cast<int>((x - 0) * tileScale.x - tileOffset.x), static_cast<int>((y - 0) * tileScale.y - tileOffset.y)),
						Vector<int>(static_cast<int>((x + 1) * tileScale.x - tileOffset.x), static_cast<int>((y + 1) * tileScale.y - tileOffset.y)),
						BLACK
					);
					continue;
				}

				//Debug("tileID:" + to_string(tileID));
				Tile* tile = tileMap.GetTile(tileID);
				if (tile == nullptr)
				{
					renderer->DrawRect(
						Vector<int>(static_cast<int>(x * tileScale.x - tileOffset.x), static_cast<int>(y * tileScale.y - tileOffset.y)),
						Vector<int>(static_cast<int>((x + 1) * tileScale.x - tileOffset.x), static_cast<int>((y + 1) * tileScale.y - tileOffset.y)),
						LIGHTRED
					);
					continue;
				}
				renderer->DrawRect(
					Vector<int>(static_cast<int>(x * tileScale.x - tileOffset.x), static_cast<int>(y * tileScale.y - tileOffset.y)),
					Vector<int>(static_cast<int>((x + 1) * tileScale.x - tileOffset.x), static_cast<int>((y + 1) * tileScale.y - tileOffset.y)),
					*tileMap.GetTile(tileID)->sprite
				);

			}
		}


		followPositionOnScreen = std::move(Vector<int>(static_cast<int>((follow->position.x - offset.x) * tileScale.x), static_cast<int>((follow->position.y - offset.y) * tileScale.y)));
		
		//绘制游戏物品
		for (const auto& instance:spriteRendererInstances) 
		{

			if (instance.actor==nullptr)
			{
				
				continue;
			}

			Vector<int> objPos = Vector<int>(
				static_cast<int>((instance.actor->transform.position.x - offset.x) * tileScale.x),
				static_cast<int>((instance.actor->transform.position.y - offset.y) * tileScale.y)
				);
			instance.sprite->DrawSprite(objPos.x, objPos.y, true);
			if (objPos.y<pixelHeight&&objPos.y>=0
				&&objPos.x < pixelWidth && objPos.x >= 0
				) {
				objectBufferWithRay[objPos.y * pixelWidth + objPos.x] = instance.actor;
			}

		}

		return followPositionOnScreen;

	}

	

	FreeCamera::FreeCamera()
	{
		wallDistanceBuffer = std::vector<float>(pixelWidth, 0.0);
		objectBufferWithRay = std::vector<void*>(pixelWidth, nullptr);
	}

	
	void FreeCamera::RenderFloor(TileMap& map, uint32_t multiColor)
	{
		//FLOOR CASTING
		
		const float angle = follow->eulerAngle - halfFOV;
		const float dirX = sinf(follow->eulerAngle);
		const float dirY = cosf(follow->eulerAngle);
		
		const float eyeRayX = normalEyeRay * sinf(angle);
		const float eyeRayY = normalEyeRay * cosf(angle);
		const float planeX = -eyeRayX + dirX;
		const float planeY = -eyeRayY + dirY;

		const float rayDirX0 = eyeRayX;
		const float rayDirY0 = eyeRayY;
		const float rayDirX1 = dirX + planeX;
		const float rayDirY1 = dirY + planeY;

		for (int y = static_cast<int>(pixelHeight * 0.5); y < pixelHeight; y++)
		{
			// Current y position compared to the center of the screen (the horizon)
			//const int p = y - pixelHeight * 0.5;

			// Vertical position of the camera.
			//const float posZ = 0.5 * pixelHeight;

			// Horizontal distance from the camera to the floor for the current row.
			const float rowDistance = pixelHeight/(2.0f*y - pixelHeight);

			// calculate the real world step vector we have to add for each x (parallel to camera plane)
			// adding step by step avoids multiplications with a weight in the inner loop
			const float floorStepX = rowDistance * (2 * planeX) / pixelWidth;
			const float floorStepY = rowDistance * (2 * planeY) / pixelWidth;

			// real world coordinates of the leftmost column. This will be updated as we step to the right.
			float floorX = follow->position.x + rowDistance * rayDirX0 + 0.5f;
			float floorY = follow->position.y + rowDistance * rayDirY0 + 0.5f;

			for (int x = 0; x < pixelWidth; ++x)
			{
				// the cell coord is simply got from the integer parts of floorX and floorY
				const int cellX = (int)(floorX);
				const int cellY = (int)(floorY);

				const float simpleX = floorX - cellX;
				const float simpleY = floorY - cellY;

				floorX += floorStepX;
				floorY += floorStepY;

				// choose texture and draw the pixel
				const int floorTileID = map.GetTileID(cellX, cellY);
				const Tile* floorTile = map.GetTile(floorTileID);
				if (floorTileID == -1 || floorTile == nullptr)
				{
					DRAWPIXEL(x, y, LIGHTRED);
					//renderer.DrawPixel(x, y, LIGHTRED);
					continue;
				}

				// floor
				Uint32 color = floorTile->sprite->GetColor(simpleX, simpleY);

				color = MULTICOLOR(color, multiColor);

				DRAWPIXEL(x, y, color);
				//renderer.DrawPixel(x, y, color);

				//ceiling (symmetrical, at screenHeight - y - 1 instead of y)
				//color = texture[ceilingTexture][texWidth * ty + tx];
				//color = (color >> 1) & 8355711; // make a bit darker
				//buffer[screenHeight - y - 1][x] = color;
			}
		}
	}

	void FreeCamera::Render(TileMap& map, bool renderFloor, Sprite* skybox, uint32_t multiColor)
	{

		

		if (follow == nullptr) 
		{
			return;
		}

		////采用画家画图法和射线投射算法绘制

		//FLOOR CASTING

		if (renderFloor)
		{
			RenderFloor(map, multiColor);
		}

		//绘制墙壁
		for (int x = 0; x < pixelWidth; x++)
		{
			//Ray ray = move(RaycastHit(x, map));
			Ray ray = std::move(RaycastHit(x, map));

			wallDistanceBuffer[x] = ray.distance;
			rayResult[x] = ray;

			//绘制墙壁
			const float ceiling = pixelHeight * 0.5f - pixelHeight / ray.distance;
			const float floor = pixelHeight - ceiling;
			uint32_t color = ERROR;

			//绘制地板与天花板

			for (int y = 0; y < pixelHeight; y++)
			{
				if (y <= ceiling)
				{
					
					if (skybox==nullptr)
					{
						color = RGBA(63, 63, 63,255);
						color = MULTICOLOR(color, multiColor);
						DRAWPIXEL(x, y, color);
						continue;
					}
					const float dx = (x + 200 * follow->eulerAngle) / pixelWidth;
					const float dy = y / (pixelHeight*2.0f);

					color = skybox->GetColor(dy, dx);

					DRAWPIXEL(x, y, color);
					
				}
				else if (y > ceiling && y <= floor )
				{
					ray.simple.y = (y - ceiling)
						/ (floor - ceiling);

					const Tile* tile = map.GetTile(ray.hitTile);
					if (tile != nullptr)
					{
						color = tile->sprite->GetColor(ray.simple.y, ray.simple.x);
					}

				}
				else if (!renderFloor)
				{
					color = RGBA(128,128,128,255);
					
				}

				color = MULTICOLOR(color, multiColor);

				DRAWPIXEL(x, y, color);

			}

		}

		RenderGameObject(multiColor);

	}
	

	Ray FreeCamera::RaycastHit(int pixelX,const TileMap& map)
	{
		Ray ray;
		ray.distance = 0.0f;
		ray.angle = follow->eulerAngle - FOV * (0.5f - (float)pixelX / pixelWidth);
		const float rayForwordStep = 0.02f;
		const Vector<float> eye = std::move(Vector<float>(sinf(ray.angle), cosf(ray.angle)));

		while (!map.IsCollisionTile(ray.hitTile)&&ray.distance<viewDepth) 
		{
			//直到射线集中障碍物
			ray.distance += rayForwordStep;

			const Vector<float> floatHitPoint = std::move(follow->position + eye * ray.distance+Vector<float>(0.5,0.5));
			const Vector<int> intHitPoint = std::move(Vector<int>(static_cast<int>(floatHitPoint.x), static_cast<int>(floatHitPoint.y)));

			if (intHitPoint.x < 0||intHitPoint.x >= map.w || intHitPoint.y < 0|| intHitPoint.y >= map.h)
			{
				ray.hitTile = -1;
				ray.distance = viewDepth;
				continue;
			}

			ray.hitTile = map.GetTileID(intHitPoint.x, intHitPoint.y);
			ray.tilePosition = { intHitPoint.x,intHitPoint.y };

			if (map.IsCollisionTile(ray.hitTile)) 
			{

				const float blockMidX = (float)intHitPoint.x + 0.5f;
				const float blockMidY = (float)intHitPoint.y + 0.5f;

				const float testAngle = atan2f((floatHitPoint.y - blockMidY), (floatHitPoint.x - blockMidX));

				if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
				{
					ray.simple.x = floatHitPoint.y - intHitPoint.y;
				}
				else if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.x - intHitPoint.x;
				}
				else if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.x - intHitPoint.x;
				}
				else if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
				{
					ray.simple.x = floatHitPoint.y - intHitPoint.y;
				}
			}

		}

		ray.distance = ray.distance * cosf(follow->eulerAngle - ray.angle);

		return ray;
	}


	// 快速排序的分区函数
	inline int Partition(std::vector<SpriteRendererInstance>& arr, int low, int high) {
		const float pivot = arr[high].distanceToPlayer;
		int i = (low - 1);

		for (int j = low; j <= high - 1; j++) {
			if (arr[j].distanceToPlayer >= pivot) {
				i++;
				std::swap(arr[i], arr[j]);
			}
		}
		std::swap(arr[i + 1], arr[high]);
		return (i + 1);
	}

	// 快速排序函数
	inline void QuickSort(std::vector<SpriteRendererInstance>& arr, int low, int high) 
	{
		if (low >= high) 
		{
			return;
		}

		const int pi = Partition(arr, low, high);
		QuickSort(arr, low, pi - 1);
		QuickSort(arr, pi + 1, high);

	}

	void FreeCamera::RenderGameObjectEnter()
	{
		//对游戏内的物品使用快速排序进行排序

		for (int i = 0; i < objectBufferWithRay.size(); i++)
		{
			objectBufferWithRay[i] = nullptr;
		}

		//计算gameObject和玩家之间的距离
		for (auto& instance : spriteRendererInstances)
		{
			if (instance.actor == nullptr)
			{
				continue;
			}
			instance.vecToPlayer = std::move(instance.actor->transform.position - follow->position);
			instance.distanceToPlayer = instance.vecToPlayer.Magnitude();

		}

		QuickSort(spriteRendererInstances, 0, static_cast<int>(spriteRendererInstances.size()) - 1);
	}

	void FreeCamera::RenderGameObject(uint32_t multiColor)
	{
		RenderGameObjectEnter();

		//Scene* activeScene = sceneManager.GetActiveScene();

		for (const auto & instance :spriteRendererInstances) 
		{
			if (instance.actor == nullptr)
			{
				continue;
			}

			const Vector<float>& vecToFollow = instance.vecToPlayer;

			const float distanceFromPlayer = instance.distanceToPlayer;

			const Vector<float> eye = std::move(Vector<float>(sinf(follow->eulerAngle), cosf(follow->eulerAngle)));

			float objectAngle = atan2(eye.y, eye.x) - atan2(vecToFollow.y, vecToFollow.x);
			if (objectAngle <= -PI)
			{
				objectAngle += 2.0f * PI;
			}
			if (objectAngle > PI)
			{
				objectAngle -= 2.0f * PI;
			}


			const bool isInPlayerFOV = fabs(objectAngle) < HALFPI;
			//const bool isInPlayerFOV = true;

			if (isInPlayerFOV && distanceFromPlayer >= 0.5f &&
				distanceFromPlayer < viewDepth)
			{

				//绘制物体到屏幕上
				const float objectCeiling = pixelHeight * 0.5f
					- pixelHeight / distanceFromPlayer;

				const float objectFloor = pixelHeight - objectCeiling;

				const float objectHeight = objectFloor - objectCeiling;
				const float objectWidth = objectHeight;

				const float middleOfObject = ((objectAngle / FOV + 0.5f))
					* pixelWidth;

				const float objectPosZ = 2 * instance.actor->transform.posZ / distanceFromPlayer;

				for (float lx = 0; lx < objectWidth; lx++)
				{
					const int objectColumn = static_cast<int>(middleOfObject + lx - objectWidth * 0.5f);
					const float objSimpleX = lx / objectWidth;
					if (
						 objectColumn < 0 || objectColumn >= pixelWidth
						|| wallDistanceBuffer[objectColumn] < distanceFromPlayer

						)
					{
						continue;
					}
					for (float ly = 0; ly < objectHeight; ly++)
					{
						const float objSimpleY = ly / objectHeight;
						Uint32 objColor = instance.sprite->GetColor(objSimpleX, objSimpleY);
						if (
							GetAValue(objColor) == 0
							|| (int)(objectCeiling + ly) < 0 || (int)(objectCeiling + ly) >= pixelHeight
							)
						{
							continue;
						}
						objColor = MULTICOLOR(objColor, multiColor);
						DRAWPIXEL(objectColumn, (int)(objectCeiling + ly + objectPosZ), objColor);
						//renderer.DrawPixel(objectColumn, (int)(objectCeiling + ly + objectPosZ), objColor);
						if (instance.actor->isRaycasted)
						{
							objectBufferWithRay[objectColumn] = (void*)instance.actor;
						}
						wallDistanceBuffer[objectColumn] = distanceFromPlayer;
						rayResult[objectColumn].hitTile = -1;

					}
				}
			}
		}

		

		////绘制物品
		//for (int i = 0;i< spriteRendererInstances.size();i++)
		//{

		//	

		//}
	}

}


