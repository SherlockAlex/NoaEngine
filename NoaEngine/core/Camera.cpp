#include "Camera.h"
#include "NoaEngine.h"

namespace noa {

	extern unordered_map<int, bool> collisionTiles;
	extern bool IsCollisionTile(int tileID);
	extern vector<GameObject*> gameObjects;
	static vector<float> wallDistanceBuffer;

	Camera::Camera()
	{
	}
	Camera::Camera(Transform* follow)
	{
		this->follow = follow;
	}

	Camera::~Camera()
	{

	}

	TileMapCamera::TileMapCamera()
	{
	}

	TileMapCamera::TileMapCamera(Vector<int> tileScale, Transform* follow):
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
		
		//绘制游戏物品
		for (auto gameObject:gameObjects) 
		{

			if (gameObject==nullptr)
			{
				continue;
			}

			Vector<int> objPos = Vector<int>(
				(gameObject->transform.position.x - offset.x) * tileScale.x,
				(gameObject->transform.position.y - offset.y) * tileScale.y
				);
			gameObject->sprite->DrawSprite(objPos.x, objPos.y, true);

		}
		
		return followPositionOnScreen;

	}

	

	FreeCamera::FreeCamera()
	{
		wallDistanceBuffer = vector<float>(pixelWidth, 0.0);
		objectBufferWithRay = vector<void*>(pixelWidth, nullptr);
	}
	FreeCamera::FreeCamera(Transform* follow):Camera(follow)
	{
		wallDistanceBuffer = vector<float>(pixelWidth, 0.0);
		objectBufferWithRay = vector<void*>(pixelWidth, nullptr);
	}

	
	void FreeCamera::RenderFloor(TileMap& map)
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
			const int p = y - pixelHeight * 0.5;

			// Vertical position of the camera.
			const float posZ = 0.5 * pixelHeight;

			// Horizontal distance from the camera to the floor for the current row.
			const float rowDistance = posZ / p;

			// calculate the real world step vector we have to add for each x (parallel to camera plane)
			// adding step by step avoids multiplications with a weight in the inner loop
			const float floorStepX = rowDistance * (2 * planeX) / pixelWidth;
			const float floorStepY = rowDistance * (2 * planeY) / pixelWidth;

			// real world coordinates of the leftmost column. This will be updated as we step to the right.
			float floorX = follow->position.x + 0.5 + rowDistance * rayDirX0;
			float floorY = follow->position.y + 0.5 + rowDistance * rayDirY0;

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
					renderer.DrawPixel(x, y, LIGHTRED);
					continue;
				}

				// floor
				Uint32 color = floorTile->sprite->GetColor(simpleX, simpleY);
				//color = BRIGHTER(color, 50);

				//color = DARKER(color, rowDistance * rowDistance*0.3);

				//color = (color >> 1) & 8355711; // make a bit darker
				renderer.DrawPixel(x, y, color);

				//ceiling (symmetrical, at screenHeight - y - 1 instead of y)
				//color = texture[ceilingTexture][texWidth * ty + tx];
				//color = (color >> 1) & 8355711; // make a bit darker
				//buffer[screenHeight - y - 1][x] = color;
			}
		}
	}

	void FreeCamera::Render(TileMap& map, bool renderFloor,const Sprite & skybox)
	{

		////采用画家画图法和射线投射算法绘制

		//FLOOR CASTING

		if (renderFloor)
		{
			RenderFloor(map);
		}

		//绘制墙壁
		for (int x = 0; x < pixelWidth; x++)
		{
			//Ray ray = move(RaycastHit(x, map));
			Ray ray = move(RaycastHit(x, map));

			wallDistanceBuffer[x] = ray.distance;

			//绘制墙壁
			const float ceiling = pixelHeight * 0.5 - pixelHeight / ray.distance;
			const float floor = pixelHeight - ceiling;
			uint32_t color = 980088;
			const float shadowOfWall = 1 / (1 +
				ray.distance * ray.distance * ray.distance * ray.distance * ray.distance * 0.00002);
			float sharkCamera = 75 * (sinf(1.5 * (follow->position.x)) + sinhf(1.5 * (follow->position.y)));
			sharkCamera = sharkCamera / ray.distance;
			sharkCamera = 0;

			//绘制地板与天花板

			for (int y = 0; y < floor + sharkCamera; y++)
			{
				if (y <= ceiling + sharkCamera)
				{
					const float dx = (x + 200 * follow->eulerAngle) / pixelWidth;
					const float dy = (y + 0.0) / (pixelHeight*2);

					const ColorRef color = skybox.GetColor(dy, dx);
					renderer.DrawPixel(x, y, color);
					//color = RGB(63, 63, 63);
					continue;
				}
				else if (y > ceiling + sharkCamera && y <= floor + sharkCamera)
				{
					ray.simple.y = ((float)y - (float)(ceiling + sharkCamera))
						/ ((float)floor - (float)ceiling);

					const Tile* tile = map.GetTile(ray.hitTile);
					if (tile != nullptr)
					{
						color = tile->sprite->GetColor(ray.simple.y, ray.simple.x);
						//color = RGB(GetRValue(color) * shadowOfWall*10, GetGValue(color) * shadowOfWall*10, GetBValue(color) * shadowOfWall*10);
					}

					//color = DARKER(color, ray.distance * ray.distance *0.3);

				}
				else
				{
					if (renderFloor)
					{
						continue;
					}
					color = WHITE;
				}

				renderer.DrawPixel(x, y, color);

			}

		}

		RenderGameObject();

	}

	void FreeCamera::Render(TileMap& map, bool renderFloor)
	{

		////采用画家画图法和射线投射算法绘制
		
		//FLOOR CASTING

		if (renderFloor)
		{
			RenderFloor(map);
		}

		//绘制墙壁
		for (int x = 0;x<pixelWidth;x++) 
		{
			//Ray ray =move(RaycastHit(x, map));
			
			Ray ray = move(RaycastHit(x, map));

			wallDistanceBuffer[x] = ray.distance;

			//绘制墙壁
			const float ceiling = pixelHeight * 0.5 - (float)(pixelHeight) / ray.distance;
			const float floor = pixelHeight - ceiling;
			uint32_t color = 980088;
			float sharkCamera = 75 * (sinf(1.5 * (follow->position.x)) + sinhf(1.5 * (follow->position.y)));
			sharkCamera = sharkCamera / ray.distance;
			sharkCamera = 0;

			//绘制地板与天花板

			for (int y = ceiling + sharkCamera;y<floor + sharkCamera;y++)
			{
				if (y<=ceiling + sharkCamera)
				{
					//color = RGB(63, 63, 63);
					continue;
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
				//else 
				//{
				//	/*const float b = 1.0 - (((float)y - pixelHeight * 0.5) / (float)pixelHeight * 0.5);
				//	const float deltaRayShine = (1 - b) * (1 - b);
				//	const float depth = (1 - b);
				//	color = WHITE;*/
				//	//color = WHITE;
				//	if (floor!=-1)
				//	{
				//		continue;
				//	}
				//	color = WHITE;
				//}

				renderer.DrawPixel(x, y, color);

			}

		}

		RenderGameObject();

	}

	Ray FreeCamera::RaycastHit(int pixelX,const TileMap& map)
	{
		Ray ray;
		ray.distance = 0.0;
		ray.angle = follow->eulerAngle - FOV * (0.5 - (float)pixelX / pixelWidth);
		const float rayForwordStep = 0.03;
		const Vector<float> eye = move(Vector<float>(sinf(ray.angle), cosf(ray.angle)));

		while (!IsCollisionTile(ray.hitTile)&&ray.distance<viewDepth) 
		{
			//直到射线集中障碍物
			ray.distance += rayForwordStep;

			const Vector<float> floatHitPoint = move(follow->position + Vector<float>(0.5,0.5) + eye * ray.distance);
			const Vector<int> intHitPoint = move(Vector<int>(floatHitPoint.x, floatHitPoint.y));

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

		ray.distance = ray.distance * cosf(follow->eulerAngle - ray.angle);

		return ray;
	}

	void FreeCamera::RenderSkybox(const Sprite& skybox)
	{
		//渲染天空到屏幕上
		for (int x = 0;x<pixelWidth;x++) 
		{
			for (int y = 0;y<pixelHeight/2;y++)
			{
				const float dx = (x + 200*follow->eulerAngle) / pixelWidth;
				const float dy = (y + 0.0) / (pixelHeight);

				const ColorRef color = skybox.GetColor(dy, dx);
				renderer.DrawPixel(x, y, color);
			}
		}
	}
	void FreeCamera::RenderGameObject()
	{
		for (int i=0;i<objectBufferWithRay.size();i++) 
		{
			objectBufferWithRay[i] = nullptr;
		}

		//绘制物品
		for (auto object : gameObjects)
		{

			if (object == nullptr)
			{
				continue;
			}


			const Vector<float> vecToFollow = move(object->transform.position - follow->position);

			const float distanceFromPlayer = vecToFollow.Magnitude();

			const Vector<float> eye = move(Vector<float>(sinf(follow->eulerAngle), cosf(follow->eulerAngle)));

			

			float objectAngle = atan2(eye.y, eye.x) - atan2(vecToFollow.y, vecToFollow.x);
			if (objectAngle < -3.14159f)
			{
				objectAngle += 2.0f * 3.14159f;
			}
			if (objectAngle > 3.14159)
			{
				objectAngle -= 2.0f * 3.14159f;
			}
			const bool isInPlayerFOV = fabs(objectAngle) < halfFOV;

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

				const float middleOfObject = (0.5f * (objectAngle / halfFOV) + 0.5f)
					* (float)pixelWidth;

				for (float lx = 0; lx < objectWidth; lx++)
				{
					const int objectColumn = (int)(middleOfObject + lx - (objectWidth * 0.5f));
					const float objSimpleX = lx / objectWidth;
					for (float ly = 0; ly < objectHeight; ly++)
					{
						const float objSimpleY = ly / objectHeight;
						const Uint32 objColor = object->sprite->GetTransposeColor(objSimpleY, objSimpleX);
						if (objectColumn >= 0 && objectColumn < pixelWidth)
						{
							if ((int)(objectCeiling + ly) < 0 || (int)(objectCeiling + ly) >= pixelHeight
								|| (objectColumn < 0) || (objectColumn >= pixelWidth)) 
							{
								continue;
							}
							if (objColor == ERRORCOLOR || wallDistanceBuffer[objectColumn] < distanceFromPlayer)
							{
								continue;
							}
							wallDistanceBuffer[objectColumn] = distanceFromPlayer;
							renderer.DrawPixel(objectColumn, (int)(objectCeiling + ly), objColor);
							objectBufferWithRay[objectColumn] = (void*)object;
						}
					}
				}
			}

		}
	}
}


