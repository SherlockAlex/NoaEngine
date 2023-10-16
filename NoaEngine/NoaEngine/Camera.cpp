#include "Actor.h"
#include "ActorComponent.h"
#include "Camera.h"
#include "Renderer.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Graphic.h"

#include <thread>

namespace noa {



	Camera::Camera(Scene* scene)
	{
		scene->AddCamera(this);
	}

	Camera::~Camera()
	{

	}

	void Camera::Delete(Camera *& ptr)
	{
		delete this;
		if (ptr!=nullptr) 
		{
			ptr = nullptr;
		}
	}

	void Camera::SetFollow(Transform* follow)
	{
		this->follow = follow;
	}


	TileMapCamera::TileMapCamera(Scene * scene):Camera(scene)
	{

	}

	TileMapCamera* TileMapCamera::Create(Scene* scene)
	{
		return new TileMapCamera(scene);
	}

	void TileMapCamera::SetTileScale(Vector<int> tileScale)
	{
		this->tileScale = tileScale;
		visibleTiles = Vector<float>(static_cast<float>(int(pixelWidth / tileScale.x)), static_cast<float>(int(pixelHeight / tileScale.y)));
	}

	Vector<float> tileOffset;
	void TileMapCamera::Render()
	{
		if (follow == nullptr||tileMap == nullptr)
		{
			return;
		}
		for (auto& object:objectBufferWithRay) 
		{
			object = nullptr;
		}

		position = follow->position;

		offset = std::move(position - visibleTiles * 0.5);
		if (offset.x < frontDelta.x) {
			offset.x = frontDelta.x;
		}
		else if (offset.x > tileMap->w - visibleTiles.x + endDelta.x) {
			offset.x = tileMap->w - visibleTiles.x + endDelta.x;
		}

		if (offset.y < frontDelta.y) {
			offset.y = frontDelta.y;
		}
		else if (offset.y > tileMap->h - visibleTiles.y + endDelta.y) {
			offset.y = tileMap->h - visibleTiles.y + endDelta.y;
		}

		tileOffset.x = (offset.x - (int)offset.x) * tileScale.x;
		tileOffset.y = (offset.y - (int)offset.y) * tileScale.y;

		for (int x = -2; x < visibleTiles.x+2; x++)
		{
			for (int y = -2; y < visibleTiles.y+2 ; y++)
			{
				const int tileID = tileMap->GetTileID(static_cast<int>(x + offset.x), static_cast<int>(y + offset.y));
				if (tileID == -1)
				{
					renderer->DrawRect(
						Vector<int>(static_cast<int>((x - 0) * tileScale.x - tileOffset.x), static_cast<int>((y - 0) * tileScale.y - tileOffset.y)),
						Vector<int>(static_cast<int>((x + 1) * tileScale.x - tileOffset.x), static_cast<int>((y + 1) * tileScale.y - tileOffset.y)),
						BLACK
					);
					continue;
				}

				Tile* tile = tileMap->GetTile(tileID);
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
					*tileMap->GetTile(tileID)->sprite
				);

			}
		}

		//玩家在屏幕上的位置
		followPositionOnScreen = std::move(Vector<int>(static_cast<int>((follow->position.x - offset.x) * tileScale.x), static_cast<int>((follow->position.y - offset.y) * tileScale.y)));
		
		//渲染物品到屏幕上
		for (const auto& instance:spriteRendererInstances) 
		{

			if (instance.actor==nullptr)
			{				
				continue;
			}

			const Vector<int> & objPos = Vector<int>(
				static_cast<int>((instance.actor->transform.position.x - offset.x) * tileScale.x),
				static_cast<int>((instance.actor->transform.position.y - offset.y) * tileScale.y)
				);
			instance.spriteGPU->DrawSprite(static_cast<float>(objPos.x), static_cast<float>(objPos.y), true);
			if (objPos.y<pixelHeight&&objPos.y>=0
				&&objPos.x < pixelWidth && objPos.x >= 0
				) {
				objectBufferWithRay[objPos.y * pixelWidth + objPos.x] = instance.actor;
			}

		}
		spriteRendererInstances.clear();

	}

	void TileMapCamera::SetTileMap(TileMap* tileMap)
	{
		this->tileMap = tileMap;
	}

	void TileMapCamera::SetDelta(const Vector<float>& frontDelta, const Vector<float>& endDelta)
	{
		this->frontDelta = frontDelta;
		this->endDelta = endDelta;
	}

	

	FreeCamera::FreeCamera(Scene* scene):Camera(scene)
	{
		wallDistanceBuffer = std::vector<float>(pixelWidth, 0.0);
		objectBufferWithRay = std::vector<NOAObject*>(pixelWidth, nullptr);
	}

	
	FreeCamera* FreeCamera::Create(Scene* scene)
	{
		return new FreeCamera(scene);
	}

	void FreeCamera::RenderFloor()
	{
		
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
			const float rowDistance = pixelHeight/(2.0f*y - pixelHeight);

			const float floorStepX = rowDistance * (2 * planeX) / pixelWidth;
			const float floorStepY = rowDistance * (2 * planeY) / pixelWidth;

			float floorX = follow->position.x + rowDistance * rayDirX0 + 0.5f;
			float floorY = follow->position.y + rowDistance * rayDirY0 + 0.5f;

			for (int x = 0; x < pixelWidth; ++x)
			{
				const int cellX = (int)(floorX);
				const int cellY = (int)(floorY);

				const float simpleX = floorX - cellX;
				const float simpleY = floorY - cellY;

				floorX += floorStepX;
				floorY += floorStepY;

				const int floorTileID = map->GetTileID(cellX, cellY);
				const Tile* floorTile = map->GetTile(floorTileID);
				if (floorTileID == -1 || floorTile == nullptr)
				{
					DRAWPIXEL(x, y, LIGHTRED);
					continue;
				}

				Uint32 color = floorTile->sprite->GetColor(simpleX, simpleY);

				//color = MULTICOLOR(color, multiColor);

				DRAWPIXEL(x, y, color);

			}
		}
	}

	void FreeCamera::Render()
	{
		if (follow == nullptr||map == nullptr) 
		{
			return;
		}

		if (renderFloor)
		{
			RenderFloor();
		}

		for (int x = 0; x < pixelWidth; x++)
		{
			Ray ray = std::move(RaycastHit(x));

			wallDistanceBuffer[x] = std::move(ray.distance);
			rayResult[x] = std::move(ray);

			const float ceiling = pixelHeight * 0.5f - pixelHeight / ray.distance;
			const float floor = pixelHeight - ceiling;
			uint32_t color = ERRORCOLOR;

			for (int y = 0; y < pixelHeight; y++)
			{
				if (y <= ceiling)
				{
					
					if (skybox==nullptr)
					{
						color = RGBA(63, 63, 63,255);
						//color = MULTICOLOR(color, multiColor);
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

					const Tile* tile = map->GetTile(ray.hitTile);
					if (tile != nullptr)
					{
						color = tile->sprite->GetColor(ray.simple.y, ray.simple.x);
					}

				}
				else if (!renderFloor)
				{
					color = RGBA(128,128,128,255);
					
				}

				DRAWPIXEL(x, y, color);

			}

		}

		RenderGameObject();

	}
	

	Ray FreeCamera::RaycastHit(int pixelX)
	{
		Ray ray;
		ray.distance = 0.0f;
		ray.angle = follow->eulerAngle - FOV * (0.5f - (float)pixelX / pixelWidth);
		const float rayForwordStep = 0.05f;
		const Vector<float> & eye = Vector<float>(sinf(ray.angle), cosf(ray.angle));
		bool isHitCollisionTile = map->IsCollisionTile(ray.hitTile);
		while (!isHitCollisionTile && ray.distance < viewDepth)
		{
			ray.distance += rayForwordStep;

			const Vector<float> & floatHitPoint = follow->position + eye * ray.distance+Vector<float>(0.5f,0.5f);
			const Vector<int> & intHitPoint = Vector<int>(static_cast<int>(floatHitPoint.x), static_cast<int>(floatHitPoint.y));

			if (intHitPoint.x < 0||intHitPoint.x >= map->w || intHitPoint.y < 0|| intHitPoint.y >= map->h)
			{
				ray.hitTile = -1;
				ray.distance = viewDepth;
				continue;
			}

			ray.hitTile = map->GetTileID(intHitPoint.x, intHitPoint.y);
			ray.tilePosition = { intHitPoint.x,intHitPoint.y };

			isHitCollisionTile = map->IsCollisionTile(ray.hitTile);

			if (isHitCollisionTile)
			{

				const float blockMidX = intHitPoint.x + 0.5f;
				const float blockMidY = intHitPoint.y + 0.5f;

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

	void FreeCamera::SetTileMap(TileMap* map)
	{
		this->map = map;
	}

	void FreeCamera::SetSkybox(Sprite* skybox)
	{
		this->skybox = skybox;
	}

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

		if (spriteRendererInstances.empty())
		{
			return;
		}

		for (int i = 0; i < objectBufferWithRay.size(); i++)
		{
			objectBufferWithRay[i] = nullptr;
		}

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

	void FreeCamera::RenderGameObject()
	{
		RenderGameObjectEnter();

		for (const auto & instance :spriteRendererInstances) 
		{
			if (instance.actor == nullptr)
			{
				continue;
			}

			const Vector<float>& vecToFollow = instance.vecToPlayer;

			const float distanceFromPlayer = instance.distanceToPlayer;

			const Vector<float>& eye = Vector<float>(sinf(follow->eulerAngle), cosf(follow->eulerAngle));

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

			if (isInPlayerFOV && distanceFromPlayer >= 0.5f &&
				distanceFromPlayer < viewDepth)
			{

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
						//objColor = MULTICOLOR(objColor, multiColor);
						DRAWPIXEL(objectColumn, (int)(objectCeiling + ly + objectPosZ), objColor);
						if (instance.actor->isRaycasted)
						{
							objectBufferWithRay[objectColumn] = instance.actor;
						}
						wallDistanceBuffer[objectColumn] = distanceFromPlayer;
						rayResult[objectColumn].hitTile = -1;

					}
				}
			}
		}

		spriteRendererInstances.clear();

	}

	StaticCamera::StaticCamera(Scene* scene):Camera(scene)
	{
		
	}

	StaticCamera* StaticCamera::Create(Scene* scene)
	{
		return new StaticCamera(scene);
	}

	void StaticCamera::SetBackground(Sprite* sprite)
	{
		if (sprite == nullptr)
		{
			return;
		}
		background = new SpriteGPU(sprite);
	}

	void StaticCamera::SetTileScale(const Vector<int>& tileScale)
	{
		this->tileScale = tileScale;
		visibleTiles = Vector<float>(static_cast<float>(int(pixelWidth / tileScale.x)), static_cast<float>(int(pixelHeight / tileScale.y)));
	}

	void StaticCamera::Render()
	{
		(background != nullptr) ? background->DrawSprite(0.0f, 0.0f, static_cast<float>(pixelWidth), static_cast<float>(pixelHeight), WHITE, false,0.0f) : renderer->FullScreen(BLUE);

		for (const auto & instance:spriteRendererInstances) 
		{
			//绘制Actor到屏幕位置
			if (instance.actor == nullptr)
			{
				continue;
			}

			const float posX = (instance.actor->transform.position.x) * tileScale.x;
			const float posY = (instance.actor->transform.position.y) * tileScale.y;
			instance.spriteGPU->DrawSprite(posX, posY, static_cast<float>(tileScale.x), static_cast<float>(tileScale.y), WHITE,false,0.0f);
		}

		spriteRendererInstances.clear();

	}

}


