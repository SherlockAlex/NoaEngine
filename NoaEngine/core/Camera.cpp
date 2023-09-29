#include "Camera.h"
#include "NoaEngine.h"
#include <thread>
#include "Graphic.h"

namespace noa {

	//extern unordered_map<int, bool> collisionTiles;
	//extern vector<GameObjectBuffer> gameObjects;

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
		visibleTiles = Vector<float>(int(pixelWidth / tileScale.x), int(pixelHeight / tileScale.y));
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
						BLACK
					);
					continue;
				}

				//Debug("tileID:" + to_string(tileID));
				Tile* tile = tileMap.GetTile(tileID);
				if (tile == nullptr)
				{
					renderer.DrawRect(
						Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y),
						Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y),
						LIGHTRED
					);
					continue;
				}
				renderer.DrawRect(
					Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y),
					Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y),
					*tileMap.GetTile(tileID)->sprite
				);

			}
		}


		followPositionOnScreen = move(Vector<int>((follow->position.x - offset.x) * tileScale.x, (follow->position.y - offset.y) * tileScale.y));
		
		//绘制游戏物品
		for (const auto& gameObject:sceneManager.GetActiveScene()->gameObjects) 
		{

			if (gameObject.object==nullptr)
			{
				continue;
			}

			Vector<int> objPos = Vector<int>(
				(gameObject.object->transform.position.x - offset.x) * tileScale.x,
				(gameObject.object->transform.position.y - offset.y) * tileScale.y
				);
			gameObject.object->sprite->DrawSprite(objPos.x, objPos.y, true);
			if (objPos.y<pixelHeight&&objPos.y>=0
				&&objPos.x < pixelWidth && objPos.x >= 0
				) {
				objectBufferWithRay[objPos.y * pixelWidth + objPos.x] = gameObject.object;
			}
			

		}
		
		return followPositionOnScreen;

	}

	

	FreeCamera::FreeCamera()
	{
		wallDistanceBuffer = vector<float>(pixelWidth, 0.0);
		objectBufferWithRay = vector<void*>(pixelWidth, nullptr);
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

		for (int y = pixelHeight * 0.5; y < pixelHeight; y++)
		{
			// Current y position compared to the center of the screen (the horizon)
			//const int p = y - pixelHeight * 0.5;

			// Vertical position of the camera.
			//const float posZ = 0.5 * pixelHeight;

			// Horizontal distance from the camera to the floor for the current row.
			const float rowDistance = pixelHeight/(2.0*y - pixelHeight);

			// calculate the real world step vector we have to add for each x (parallel to camera plane)
			// adding step by step avoids multiplications with a weight in the inner loop
			const float floorStepX = rowDistance * (2 * planeX) / pixelWidth;
			const float floorStepY = rowDistance * (2 * planeY) / pixelWidth;

			// real world coordinates of the leftmost column. This will be updated as we step to the right.
			float floorX = follow->position.x + rowDistance * rayDirX0 + 0.5;
			float floorY = follow->position.y + rowDistance * rayDirY0 + 0.5;

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
			Ray ray = move(RaycastHit(x, map));

			wallDistanceBuffer[x] = ray.distance;
			rayResult[x] = ray;

			//绘制墙壁
			const float ceiling = pixelHeight * 0.5 - pixelHeight / ray.distance;
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
					const float dy = y / (pixelHeight*2.0);

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
		ray.distance = 0.0;
		ray.angle = follow->eulerAngle - FOV * (0.5 - (float)pixelX / pixelWidth);
		const float rayForwordStep = 0.02;
		const Vector<float> eye = move(Vector<float>(sinf(ray.angle), cosf(ray.angle)));

		while (!map.IsCollisionTile(ray.hitTile)&&ray.distance<viewDepth) 
		{
			//直到射线集中障碍物
			ray.distance += rayForwordStep;

			const Vector<float> floatHitPoint = move(follow->position + eye * ray.distance+Vector<float>(0.5,0.5));
			const Vector<int> intHitPoint = move(Vector<int>(floatHitPoint.x, floatHitPoint.y));

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

				const float blockMidX = (float)intHitPoint.x + 0.5;
				const float blockMidY = (float)intHitPoint.y + 0.5;

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
	inline int Partition(std::vector<GameObjectBuffer>& arr, int low, int high) {
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
	inline void QuickSort(std::vector<GameObjectBuffer>& arr, int low, int high) 
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


		for (auto& object : sceneManager.GetActiveScene()->gameObjects)
		{
			//auto& object = gameObjects[i].object;
			if (object.object == nullptr)
			{
				continue;
			}
			object.vecToPlayer = move(object.object->transform.position - follow->position);
			object.distanceToPlayer = object.vecToPlayer.Magnitude();

			//const float distanceFromPlayer = vecToFollow.Magnitude();
		}

		QuickSort(sceneManager.GetActiveScene()->gameObjects, 0, sceneManager.GetActiveScene()->gameObjects.size() - 1);
	}

	void FreeCamera::RenderGameObject(uint32_t multiColor)
	{
		RenderGameObjectEnter();

		Scene* activeScene = sceneManager.GetActiveScene();

		//绘制物品
		for (int i = 0;i< activeScene->gameObjects.size();i++)
		{

			auto object = activeScene->gameObjects[i].object;
			if (object == nullptr)
			{
				continue;
			}


			const Vector<float> & vecToFollow = activeScene->gameObjects[i].vecToPlayer;

			const float distanceFromPlayer = activeScene->gameObjects[i].distanceToPlayer;

			const Vector<float> eye = move(Vector<float>(sinf(follow->eulerAngle),cosf(follow->eulerAngle)));

			float objectAngle = atan2(eye.y,eye.x) - atan2(vecToFollow.y, vecToFollow.x);
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

			if (isInPlayerFOV && distanceFromPlayer >= 0.5 &&
				distanceFromPlayer < viewDepth)
			{

				//绘制物体到屏幕上
				const float objectCeiling = pixelHeight * 0.5
					- pixelHeight / distanceFromPlayer;

				const float objectFloor = pixelHeight - objectCeiling;

				const float objectHeight = objectFloor - objectCeiling;
				const float objectWidth = objectHeight;

				const float middleOfObject = ((objectAngle / FOV + 0.5))
					* pixelWidth;

				const float objectPosZ = 2*object->transform.posZ/ distanceFromPlayer;

				for (float lx = 0; lx < objectWidth; lx++)
				{
					const int objectColumn = middleOfObject + lx - objectWidth * 0.5f;
					const float objSimpleX = lx / objectWidth;
					if (
						 objectColumn < 0 || objectColumn >= pixelWidth
						||wallDistanceBuffer[objectColumn] < distanceFromPlayer
						
						)
					{
						continue;
					}
					for (float ly = 0; ly < objectHeight; ly++)
					{
						const float objSimpleY = ly / objectHeight;
						Uint32 objColor = object->sprite->GetTransposeColor(objSimpleY, objSimpleX);
						if (
							   objColor == ERRORCOLOR
							|| (int)(objectCeiling + ly) < 0 || (int)(objectCeiling + ly) >= pixelHeight
							)
						{
							continue;
						}
						objColor = MULTICOLOR(objColor, multiColor);
						DRAWPIXEL(objectColumn, (int)(objectCeiling + ly + objectPosZ), objColor);
						//renderer.DrawPixel(objectColumn, (int)(objectCeiling + ly + objectPosZ), objColor);
						if (object->isRaycasted)
						{
							objectBufferWithRay[objectColumn] = (void*)object;
						}
						wallDistanceBuffer[objectColumn] = distanceFromPlayer;
						rayResult[objectColumn].hitTile = -1;

					}
				}
			}

		}
	}

}


